#include "MainWindow.h"

#include <iostream>

#include <qstring.h>
#include <qdatetime.h>
#include <qevent.h>

#include "app/DataManager.h"
#include "app/EditOperationDialog.h"

MainWindow::MainWindow(QWidget* parent)
	: QWidget(parent)
{
	setWindowTitle("Manage my budget");

	InitializeData();

	m_manageCategoriesButton = new QPushButton("Gérer les catégories");

	connect(m_manageCategoriesButton, &QPushButton::released, this, &MainWindow::HandleManageCategories);

	m_manageAccountsButton = new QPushButton("Gérer les comptes");

	m_currentAccountLabel = new QLabel("Compte bancaire sélectionné");
	m_currentAccountComboBox = new QComboBox();
	for (const BankAccount& account : s_DataManager.bankAccounts) {
		m_currentAccountComboBox->addItem(QString::fromStdString(account.name));
	}
	m_currentAccountComboBox->setCurrentText(QString::fromStdString(s_DataManager.r_CurrentBankAccount().name));

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
	s_DataManager.SaveOperations();
}

void MainWindow::HandleOperationAdd(const Operation& operation)
{
	s_DataManager.r_CurrentBankAccount().operations.push_back(operation);
	s_DataManager.SaveOperations();

	UpdateUI();
}

void MainWindow::HandleManageCategories()
{
	ManageCategoriesDialog manageCategoriesDialog;
	manageCategoriesDialog.exec();
	UpdateUI();
	m_addOperationForm->LoadCategories();
}

void MainWindow::HandleCurrentAccountChange()
{
	s_DataManager.r_CurrentBankAccount() = s_DataManager.bankAccounts[m_currentAccountComboBox->currentIndex()];
	UpdateUI();
}