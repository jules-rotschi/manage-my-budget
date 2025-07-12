#include "MainWindow.h"

#include <iostream>

#include <qstring.h>
#include <qdatetime.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <qfiledialog.h>
#include <qmessagebox.h>

#include "DataManager.h"
#include "EditOperationDialog.h"
#include "ManageCategoriesDialog.h"
#include "ManageProfilesDialog.h"
#include "ManageAccountsDialog.h"
#include "MonthlyReviewDialog.h"
#include "YearlyReviewDialog.h"
#include "ExceptionHandler.h"
#include "StringFormatter.h"
#include "ConfirmationRequiser.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	setWindowTitle("Manage my budget");

	setMinimumWidth(720);

	QMessageBox introBox;
	introBox.setText("Veuillez sélectionner l'emplacement des données de l'application.");
	QPushButton boxDefaultButton("Ouvrir le gestionnaire de fichiers");
	boxDefaultButton.setFixedWidth(200);
	introBox.addButton(&boxDefaultButton, QMessageBox::AcceptRole);
	introBox.setDefaultButton(&boxDefaultButton);
	introBox.exec();

	QString selectedDirectoryPath;

	QFileDialog dialog;
	dialog.setWindowTitle("Emplacement des données");
	dialog.setOption(QFileDialog::ShowDirsOnly);
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setDirectory(QDir::homePath());
	if (dialog.exec()) {
		QStringList selectedDirectories = dialog.selectedFiles();

		if (selectedDirectories.empty()) {
			return;
		}

		selectedDirectoryPath = selectedDirectories.first();
	}
	else {
		selectedDirectoryPath = QDir::homePath();
	}

	QDir dataDirectory(selectedDirectoryPath);

	if (dataDirectory.dirName() == "manage-my-budget-data") {
		dataDirectory.cdUp();
	}

	QMessageBox dataDirectoryInfoBox;
	dataDirectoryInfoBox.setText(QString::fromStdString("Les données seront automatiquement enregistrées dans le dossier \"" + dataDirectory.path().toStdString() + "\"."));
	dataDirectoryInfoBox.exec();

	s_DataManager.InitializeData(dataDirectory);

	m_centralWidget = new QWidget();

	m_profileMenu = menuBar()->addMenu("Profil");
	m_reviewMenu = menuBar()->addMenu("Bilans");
	m_dataMenu = menuBar()->addMenu("Données");

	m_manageCategoriesAction = new QAction("Gérer les catégories du profil");
	m_profileMenu->addAction(m_manageCategoriesAction);
	connect(m_manageCategoriesAction, &QAction::triggered, this, &MainWindow::HandleManageCategories);

	m_manageAccountsAction = new QAction("Gérer les comptes du profil");
	m_profileMenu->addAction(m_manageAccountsAction);
	connect(m_manageAccountsAction, &QAction::triggered, this, &MainWindow::HandleManageAccounts);

	m_profileMenu->addSeparator();

	m_manageProfilesAction = new QAction("Gérer les profils");
	m_profileMenu->addAction(m_manageProfilesAction);
	connect(m_manageProfilesAction, &QAction::triggered, this, &MainWindow::HandleManageProfiles);

	m_monthlyReviewAction = new QAction("Bilans mensuels");
	m_reviewMenu->addAction(m_monthlyReviewAction);
	connect(m_monthlyReviewAction, &QAction::triggered, this, &MainWindow::HandleMonthlyReview);

	m_yearlyReviewAction = new QAction("Bilans annuels");
	m_reviewMenu->addAction(m_yearlyReviewAction);
	connect(m_yearlyReviewAction, &QAction::triggered, this, &MainWindow::HandleYearlyReview);

	m_backUpAction = new QAction("Créer une sauvegarde");
	m_dataMenu->addAction(m_backUpAction);
	connect(m_backUpAction, &QAction::triggered, this, &MainWindow::HandleBackUp);

	m_backUpLoadAction = new QAction("Charger une sauvegarde");
	m_dataMenu->addAction(m_backUpLoadAction);
	connect(m_backUpLoadAction, &QAction::triggered, this, &MainWindow::HandleBackUpLoad);

	m_dataMenu->addSeparator();

	m_resetDataAction = new QAction("Réinitialiser les données");
	m_dataMenu->addAction(m_resetDataAction);
	connect(m_resetDataAction, &QAction::triggered, this, &MainWindow::HandleDataReset);

	m_currentProfileForm = new QWidget();
	
	m_currentProfileLabel = new QLabel("Profil");

	m_currentProfileComboBox = new QComboBox();
	m_currentProfileComboBox->addItem("");
	LoadProfilesToComboBox();
	connect(m_currentProfileComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::HandleCurrentProfileChange);

	m_currentProfileFormLayout = new QFormLayout(m_currentProfileForm);
	m_currentProfileFormLayout->addRow(m_currentProfileLabel, m_currentProfileComboBox);

	m_currentAccountForm = new QWidget();
	
	m_currentAccountLabel = new QLabel("Compte bancaire sélectionné");
	
	m_currentAccountComboBox = new QComboBox();
	m_currentAccountComboBox->addItem("");
	LoadAccountsToComboBox();
	connect(m_currentAccountComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::HandleCurrentAccountChange);

	m_currentAccountFormLayout = new QFormLayout(m_currentAccountForm);
	m_currentAccountFormLayout->addRow(m_currentAccountLabel, m_currentAccountComboBox);

	m_operationsList = new OperationsList();

	m_addOperationForm = new AddOperationForm();
	connect(m_addOperationForm, &AddOperationForm::OperationAdd, this, &MainWindow::HandleOperationAdd);

	m_mainLayout = new QVBoxLayout(m_centralWidget);
	m_mainLayout->addWidget(m_currentProfileForm);
	m_mainLayout->addWidget(m_currentAccountForm);
	m_mainLayout->addWidget(m_operationsList);
	m_mainLayout->addWidget(m_addOperationForm);

	setCentralWidget(m_centralWidget);

	UpdateUI(true);
}

void MainWindow::UpdateUI(bool scrollDown)
{
	m_operationsList->UpdateUI(scrollDown);
}

void MainWindow::LoadProfilesToComboBox()
{
	for (int i = 1; i < m_currentProfileComboBox->count();) {
		m_currentProfileComboBox->removeItem(i);
	}

	m_currentProfileComboBox->setItemText(
		0, QString::fromStdString(LimitLength(s_DataManager.r_Profiles()[0].name, 20))
	);

	for (int i = 1; i < s_DataManager.r_Profiles().size(); i++) {
		const Profile& profile = s_DataManager.r_Profiles()[i];
		m_currentProfileComboBox->addItem(QString::fromStdString(LimitLength(profile.name, 20)));
	}
}

void MainWindow::LoadAccountsToComboBox()
{
	for (int i = 1; i < m_currentAccountComboBox->count();) {
		m_currentAccountComboBox->removeItem(i);
	}

	m_currentAccountComboBox->setItemText(
		0, QString::fromStdString(LimitLength(s_DataManager.r_CurrentProfile().bankAccounts[0].name, 20) + " (" + s_DataManager.r_CurrentProfile().bankAccounts[0].GetTypeString() + ")")
	);

	for (int i = 1; i < s_DataManager.r_CurrentProfile().bankAccounts.size(); i++) {
		const BankAccount& account = s_DataManager.r_CurrentProfile().bankAccounts[i];
		m_currentAccountComboBox->addItem(QString::fromStdString(LimitLength(account.name, 20) + " (" + account.GetTypeString() + ")"));
	}
}

void MainWindow::HandleManageProfiles()
{
	ManageProfilesDialog dialog;
	dialog.exec();

	disconnect(m_currentProfileComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::HandleCurrentProfileChange);

	LoadProfilesToComboBox();

	connect(m_currentProfileComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::HandleCurrentProfileChange);
	
	m_currentProfileComboBox->setCurrentIndex(s_DataManager.GetCurrentProfileIndex());

	UpdateUI();
}

void MainWindow::HandleManageCategories()
{
	ManageCategoriesDialog dialog;
	dialog.exec();

	m_addOperationForm->LoadCategories();
	
	UpdateUI();
}

void MainWindow::HandleManageAccounts()
{
	ManageAccountsDialog dialog;
	dialog.exec();

	disconnect(m_currentAccountComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::HandleCurrentAccountChange);
	
	LoadAccountsToComboBox();
	
	connect(m_currentAccountComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::HandleCurrentAccountChange);
	
	m_currentAccountComboBox->setCurrentIndex(s_DataManager.r_CurrentProfile().GetCurrentAccountIndex());
	
	UpdateUI();
}

void MainWindow::HandleCurrentProfileChange()
{
	s_DataManager.SetCurrentProfileIndex(m_currentProfileComboBox->currentIndex());
	
	LoadAccountsToComboBox();
	m_addOperationForm->LoadCategories();
	
	UpdateUI(true);
}

void MainWindow::HandleCurrentAccountChange()
{
	s_DataManager.SetCurrentProfileCurrentAccountIndex(m_currentAccountComboBox->currentIndex());
	UpdateUI(true);
}

void MainWindow::HandleOperationAdd()
{		
	UpdateUI(true);
}

void MainWindow::HandleMonthlyReview() const
{
	MonthlyReviewDialog dialog;
	dialog.exec();
}

void MainWindow::HandleYearlyReview() const
{
	YearlyReviewDialog dialog;
	dialog.exec();
}

void MainWindow::HandleBackUp() const
{
	QFileDialog dialog;
	dialog.setWindowTitle("Créer une sauvegarde");
	dialog.setOption(QFileDialog::ShowDirsOnly);
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setDirectory(QDir::homePath());

	if (!dialog.exec()) {
		return;
	}

	QStringList selectedDirectories = dialog.selectedFiles();

	if (selectedDirectories.empty()) {
		return;
	}

	QString selectedDirectoryPath = selectedDirectories.first();

	try {
		s_DataManager.BackUp(selectedDirectoryPath.toStdString());
	}
	catch (const ApplicationException& e) {
		HandleException(e);
		return;
	}
}

void MainWindow::HandleBackUpLoad()
{
	QFileDialog dialog;
	dialog.setWindowTitle("Charger une sauvegarde");
	dialog.setOption(QFileDialog::ShowDirsOnly);
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setDirectory(QDir::homePath());

	if (!dialog.exec()) {
		return;
	}

	if (!ConfirmAction("Cette action effacera les données actuelles de l'application pour les remplacer par celles de la sauvegarde. Voulez vous continuer ?", "Écraser les données")) {
		return;
	}

	QStringList selectedDirectories = dialog.selectedFiles();

	if (selectedDirectories.empty()) {
		return;
	}

	QString selectedDirectoryPath = selectedDirectories.first();

	try {
		s_DataManager.LoadBackUp(selectedDirectoryPath.toStdString());
	}
	catch (const ApplicationException& e) {
		HandleException(e);
		return;
	}

	LoadProfilesToComboBox();
	LoadAccountsToComboBox();
	m_addOperationForm->LoadCategories();
	UpdateUI(true);
}

void MainWindow::HandleDataReset()
{
	if (!ConfirmAction("Les données actuelles seront perdues. Voulez vous vraiment continuer ?", "Réinitialiser")) {
		return;
	}

	try {
		s_DataManager.ResetData();
		s_DataManager.InitializeData();
	}
	catch (const ApplicationException& e) {
		HandleException(e);
		return;
	}

	LoadProfilesToComboBox();
	LoadAccountsToComboBox();
	m_addOperationForm->LoadCategories();
	UpdateUI(true);
}