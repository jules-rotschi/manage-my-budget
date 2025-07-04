﻿#include "OperationsList.h"

#include "DataManager.h"

#include "app/EditOperationDialog.h"

OperationsList::OperationsList(QWidget* parent)
	: QWidget(parent)
{
	m_totalLabel = new QLabel(QString::fromStdString("Total : " + s_DataManager.bankAccount.GetTotalAmount().GetString()));

	m_operationsView = new QWidget();

	m_operationsLayout = new QVBoxLayout(m_operationsView);

	m_mainLayout = new QVBoxLayout(this);
	m_mainLayout->addWidget(m_totalLabel);
	m_mainLayout->addWidget(m_operationsView);
}

OperationsList::~OperationsList() {}

void OperationsList::UpdateUI()
{
	ResetUI();

	m_totalLabel->setText("Total : " + QString::fromStdString(s_DataManager.bankAccount.GetTotalAmount().GetString()));

	for (const Operation& operation : s_DataManager.bankAccount.operations)
	{
		QString descriptionString = operation.description.size() > 0
			? " (" + QString::fromStdString(operation.description) + ")"
			: "";

		QString operationString =
			QString::fromStdString(std::to_string(operation.id)) + " " +
			QString::fromStdString(std::to_string(operation.month))
			+ '/'
			+ QString::fromStdString(std::to_string(operation.year))
			+ " : "
			+ QString::fromStdString(operation.amount.GetString())
			+ " | "
			+ QString::fromStdString(std::to_string(operation.categoryIndex)) + " "
			+ QString::fromStdString(s_DataManager.categories[operation.categoryIndex])
			+ descriptionString;

		QLabel* operationLabel = new QLabel(operationString);
		m_operationLabels.push_back(operationLabel);

		QPushButton* editOperationButton = new QPushButton("Modifier");
		m_editOperationButtons.push_back(editOperationButton);
		QPushButton* deleteOperationButton = new QPushButton("Supprimer");
		m_deleteOperationButtons.push_back(deleteOperationButton);

		connect(editOperationButton, &QPushButton::released, [this, operation]() {
			HandleOperationEdit(operation);
			});

		connect(deleteOperationButton, &QPushButton::released, [this, operation]() {
			HandleOperationDelete(operation.id);
			});

		QWidget* operationWidget = new QWidget();
		m_operationWidgets.push_back(operationWidget);
		QHBoxLayout* operationLayout = new QHBoxLayout(operationWidget);
		m_operationLayouts.push_back(operationLayout);

		operationLayout->addWidget(operationLabel);
		operationLayout->addWidget(editOperationButton);
		operationLayout->addWidget(deleteOperationButton);

		m_operationsLayout->addWidget(operationWidget);
	}
}

void OperationsList::ResetUI()
{
	for (QWidget* widget : m_operationWidgets) {
		delete widget;
	}

	m_operationWidgets.clear();
	m_operationLayouts.clear();
	m_operationLabels.clear();
	m_editOperationButtons.clear();
	m_deleteOperationButtons.clear();
}

void OperationsList::HandleOperationEdit(const Operation& operation)
{
	EditOperationDialog dialog(operation);
	if (dialog.exec())
	{
		UpdateUI();
		s_DataManager.SaveOperations();
	}
}

void OperationsList::HandleOperationDelete(int id)
{
	s_DataManager.bankAccount.DeleteOperation(id);
	UpdateUI();
	s_DataManager.SaveOperations();
}