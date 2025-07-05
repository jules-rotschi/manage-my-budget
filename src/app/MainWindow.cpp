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

	m_operationsList = new OperationsList();

	m_addOperationForm = new AddOperationForm();

	connect(m_addOperationForm, &AddOperationForm::OperationAdd, this, &MainWindow::HandleOperationAdd);

	m_mainLayout = new QVBoxLayout(this);
	m_mainLayout->addWidget(m_manageCategoriesButton);
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
}

void MainWindow::HandleOperationAdd(const Operation& operation)
{
	s_DataManager.bankAccount.operations.push_back(operation);
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