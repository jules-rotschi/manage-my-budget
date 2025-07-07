#include "MainWindow.h"

#include <iostream>

#include <qstring.h>
#include <qdatetime.h>
#include <qevent.h>

#include "app/DataManager.h"
#include "app/EditOperationDialog.h"
#include "app/ManageCategoriesDialog.h"
#include "app/ManageAccountsDialog.h"
#include "app/MonthlyReviewDialog.h"
#include "app/YearlyReviewDialog.h"

MainWindow::MainWindow(QWidget* parent)
	: QWidget(parent)
{
	setWindowTitle("Manage my budget");

	InitializeData();

	m_manageCategoriesButton = new QPushButton("Gérer les catégories");
	m_manageAccountsButton = new QPushButton("Gérer les comptes");

	connect(m_manageCategoriesButton, &QPushButton::released, this, &MainWindow::HandleManageCategories);
	connect(m_manageAccountsButton, &QPushButton::released, this, &MainWindow::HandleManageAccounts);

	m_monthlyReviewButton = new QPushButton("Bilans mensuels");
	m_yearlyReviewButton = new QPushButton("Bilans annuels");

	connect(m_monthlyReviewButton, &QPushButton::released, this, &MainWindow::HandleMonthlyReview);
	connect(m_yearlyReviewButton, &QPushButton::released, this, &MainWindow::HandleYearlyReview);

	m_currentAccountLabel = new QLabel("Compte bancaire sélectionné");
	m_currentAccountComboBox = new QComboBox();
	m_currentAccountComboBox->addItem("");
	LoadAccountsToComboBox();

	connect(m_currentAccountComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::HandleCurrentAccountChange);

	m_currentAccountForm = new QWidget();
	m_currentAccountFormLayout = new QFormLayout(m_currentAccountForm);
	m_currentAccountFormLayout->addRow(m_currentAccountLabel, m_currentAccountComboBox);

	m_operationsList = new OperationsList();

	m_addOperationForm = new AddOperationForm();

	connect(m_addOperationForm, &AddOperationForm::OperationAdd, this, &MainWindow::HandleOperationAdd);

	m_mainLayout = new QVBoxLayout(this);
	m_mainLayout->addWidget(m_manageCategoriesButton);
	m_mainLayout->addWidget(m_manageAccountsButton);
	m_mainLayout->addWidget(m_monthlyReviewButton);
	m_mainLayout->addWidget(m_yearlyReviewButton);
	m_mainLayout->addWidget(m_currentAccountForm);
	m_mainLayout->addWidget(m_operationsList);
	m_mainLayout->addWidget(m_addOperationForm);

	UpdateUI();
}

MainWindow::~MainWindow()
{}

void MainWindow::UpdateUI()
{
	m_operationsList->UpdateUI();
}

void MainWindow::InitializeData() {
	s_DataManager.LoadData();
	s_DataManager.SaveCategories();
	s_DataManager.SaveAccounts();
}

void MainWindow::LoadAccountsToComboBox()
{
	for (int i = 1; i < m_currentAccountComboBox->count();) {
		m_currentAccountComboBox->removeItem(i);
	}

	m_currentAccountComboBox->setItemText(
		0, QString::fromStdString(s_DataManager.bankAccounts[0].name + " (" + s_DataManager.bankAccounts[0].GetTypeString() + ")")
	);

	for (int i = 1; i < s_DataManager.bankAccounts.size(); i++) {
		const BankAccount& account = s_DataManager.bankAccounts[i];
		m_currentAccountComboBox->addItem(QString::fromStdString(account.name + " (" + account.GetTypeString() + ")"));
	}
}

void MainWindow::HandleOperationAdd(const Operation& operation)
{
	s_DataManager.AddOperation(operation);
	s_DataManager.SaveAccounts();

	UpdateUI();
}

void MainWindow::HandleManageCategories()
{
	ManageCategoriesDialog dialog;
	dialog.exec();
	UpdateUI();
	m_addOperationForm->LoadCategories();
}

void MainWindow::HandleManageAccounts()
{
	int accountIndex = s_DataManager.GetCurrentAccountIndex();
	ManageAccountsDialog dialog;
	dialog.exec();
	UpdateUI();
	LoadAccountsToComboBox();
	m_currentAccountComboBox->setCurrentIndex(accountIndex);
}

void MainWindow::HandleCurrentAccountChange()
{
	s_DataManager.SetCurrentAccountIndex(m_currentAccountComboBox->currentIndex());
	UpdateUI();
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