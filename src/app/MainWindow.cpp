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

	m_operationsList = new OperationsList();

	m_addOperationForm = new AddOperationForm();

	connect(m_addOperationForm, &AddOperationForm::OperationAdd, this, &MainWindow::HandleOperationAdd);

	m_mainLayout = new QVBoxLayout(this);
	m_mainLayout->addWidget(m_operationsList);
	m_mainLayout->addWidget(m_addOperationForm);

	InitializeData();
}

MainWindow::~MainWindow()
{}

void MainWindow::HandleOperationAdd(const Operation& operation)
{
	s_DataManager.operations.push_back(operation);
	s_DataManager.SaveData();

	UpdateUI();
}

void MainWindow::UpdateUI()
{
	m_operationsList->UpdateUI();
}

void MainWindow::InitializeData() {
	s_DataManager.LoadData();
	UpdateUI();
}