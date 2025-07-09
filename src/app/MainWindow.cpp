#include "MainWindow.h"

#include <iostream>

#include <qstring.h>
#include <qdatetime.h>
#include <qevent.h>

#include "DataManager.h"
#include "EditOperationDialog.h"
#include "ManageCategoriesDialog.h"
#include "ManageProfilesDialog.h"
#include "ManageAccountsDialog.h"
#include "MonthlyReviewDialog.h"
#include "YearlyReviewDialog.h"
#include "CustomException.h"
#include "ExceptionHandler.h"

MainWindow::MainWindow(QWidget* parent)
	: QWidget(parent)
{
	setWindowTitle("Manage my budget");

	s_DataManager.InitializeData();

	m_manageProfilesButton = new QPushButton("Gérer les profils");
	connect(m_manageProfilesButton, &QPushButton::released, this, &MainWindow::HandleManageProfiles);

	m_currentProfileForm = new QWidget();
	
	m_currentProfileLabel = new QLabel("Profil");

	m_currentProfileComboBox = new QComboBox();
	m_currentProfileComboBox->addItem("");
	LoadProfilesToComboBox();
	connect(m_currentProfileComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::HandleCurrentProfileChange);

	m_currentProfileFormLayout = new QFormLayout(m_currentProfileForm);
	m_currentProfileFormLayout->addRow(m_currentProfileLabel, m_currentProfileComboBox);

	m_manageCategoriesButton = new QPushButton("Gérer les catégories");
	connect(m_manageCategoriesButton, &QPushButton::released, this, &MainWindow::HandleManageCategories);
	
	m_manageAccountsButton = new QPushButton("Gérer les comptes");
	connect(m_manageAccountsButton, &QPushButton::released, this, &MainWindow::HandleManageAccounts);

	m_monthlyReviewButton = new QPushButton("Bilans mensuels");
	connect(m_monthlyReviewButton, &QPushButton::released, this, &MainWindow::HandleMonthlyReview);
	
	m_yearlyReviewButton = new QPushButton("Bilans annuels");
	connect(m_yearlyReviewButton, &QPushButton::released, this, &MainWindow::HandleYearlyReview);

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

	m_mainLayout = new QVBoxLayout(this);
	m_mainLayout->addWidget(m_manageProfilesButton);
	m_mainLayout->addWidget(m_currentProfileForm);
	m_mainLayout->addWidget(m_manageCategoriesButton);
	m_mainLayout->addWidget(m_manageAccountsButton);
	m_mainLayout->addWidget(m_monthlyReviewButton);
	m_mainLayout->addWidget(m_yearlyReviewButton);
	m_mainLayout->addWidget(m_currentAccountForm);
	m_mainLayout->addWidget(m_operationsList);
	m_mainLayout->addWidget(m_addOperationForm);

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
		0, QString::fromStdString(s_DataManager.profiles[0].name)
	);

	for (int i = 1; i < s_DataManager.profiles.size(); i++) {
		const Profile& profile = s_DataManager.profiles[i];
		m_currentProfileComboBox->addItem(QString::fromStdString(profile.name));
	}
}

void MainWindow::LoadAccountsToComboBox()
{
	for (int i = 1; i < m_currentAccountComboBox->count();) {
		m_currentAccountComboBox->removeItem(i);
	}

	m_currentAccountComboBox->setItemText(
		0, QString::fromStdString(s_DataManager.r_CurrentProfile().bankAccounts[0].name + " (" + s_DataManager.r_CurrentProfile().bankAccounts[0].GetTypeString() + ")")
	);

	for (int i = 1; i < s_DataManager.r_CurrentProfile().bankAccounts.size(); i++) {
		const BankAccount& account = s_DataManager.r_CurrentProfile().bankAccounts[i];
		m_currentAccountComboBox->addItem(QString::fromStdString(account.name + " (" + account.GetTypeString() + ")"));
	}
}

void MainWindow::HandleManageProfiles()
{
	int profileIndex = s_DataManager.GetCurrentProfileIndex();

	ManageProfilesDialog dialog;
	dialog.exec();

	LoadProfilesToComboBox();
	m_currentProfileComboBox->setCurrentIndex(profileIndex);

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
	int accountIndex = s_DataManager.r_CurrentProfile().GetCurrentAccountIndex();

	ManageAccountsDialog dialog;
	dialog.exec();

	LoadAccountsToComboBox();
	m_currentAccountComboBox->setCurrentIndex(accountIndex);
	
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
	s_DataManager.r_CurrentProfile().SetCurrentAccountIndex(m_currentAccountComboBox->currentIndex());
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