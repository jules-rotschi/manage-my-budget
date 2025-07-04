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

	connect(m_addOperationForm, &AddOperationForm::OperationAdd, this, &MainWindow::OnOperationAdd);

	m_mainLayout = new QVBoxLayout(this);
	m_mainLayout->addWidget(m_operationsList);
	m_mainLayout->addWidget(m_addOperationForm);

	InitializeData();
}

MainWindow::~MainWindow()
{}

void MainWindow::OnOperationAdd(const Operation& operation)
{
	dataManager.operations.push_back(operation);
	dataManager.SaveData();

	Update();
}

void MainWindow::Update()
{
	m_operationsList->Update();
}

void MainWindow::InitializeData() {
	dataManager.LoadData();
	Update();
}