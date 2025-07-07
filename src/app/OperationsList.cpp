#include "OperationsList.h"

#include <iostream>

#include <qdatetime.h>

#include "DataManager.h"

#include "app/EditOperationDialog.h"

OperationsList::OperationsList(QWidget* parent)
	: QWidget(parent)
{
	m_totalLabel = new QLabel();

	m_displaySinceWidget = new QWidget();

	m_displaySinceLabel = new QLabel("Afficher les opérations depuis");
	m_displaySinceComboBox = new QComboBox();
	m_displaySinceComboBox->addItem("2 mois");
	m_displaySinceComboBox->addItem("6 mois");
	m_displaySinceComboBox->addItem("1 an");

	switch (m_displayOperationsSinceMonths) {
	case 2:
		m_displaySinceComboBox->setCurrentIndex(0);
		break;
	case 6:
		m_displaySinceComboBox->setCurrentIndex(1);
		break;
	case 12:
		m_displaySinceComboBox->setCurrentIndex(2);
		break;
	}

	connect(m_displaySinceComboBox, &QComboBox::currentIndexChanged, this, &OperationsList::HandleDisplayFromChange);

	m_displaySinceLayout = new QFormLayout(m_displaySinceWidget);
	m_displaySinceLayout->addRow(m_displaySinceLabel, m_displaySinceComboBox);

	m_operationsList = new QListWidget();

	m_mainLayout = new QVBoxLayout(this);
	m_mainLayout->addWidget(m_totalLabel);
	m_mainLayout->addWidget(m_displaySinceWidget);
	m_mainLayout->addWidget(m_operationsList);
}

OperationsList::~OperationsList() {}

void OperationsList::UpdateUI()
{
	ResetUI();

	m_totalLabel->setText("Solde du compte : " + QString::fromStdString(s_DataManager.r_CurrentBankAccount().GetTotalAmount().GetString()));
		
	QDate currentDate = QDate::currentDate();

	for (const Operation& operation : s_DataManager.r_CurrentBankAccount().operations)
	{
		if (!IsOperationInDisplayableMonth(operation, currentDate)) {
			continue;
		}

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

		QListWidgetItem* operationItem = new QListWidgetItem();
		m_operationItems.push_back(operationItem);
		QWidget* operationWidget = new QWidget();
		m_operationWidgets.push_back(operationWidget);
		QHBoxLayout* operationLayout = new QHBoxLayout();
		m_operationLayouts.push_back(operationLayout);

		operationLayout->addWidget(operationLabel);
		operationLayout->addWidget(editOperationButton);
		operationLayout->addWidget(deleteOperationButton);

		operationWidget->setLayout(operationLayout);
		
		operationItem->setSizeHint(operationWidget->sizeHint());
		m_operationsList->addItem(operationItem);
		m_operationsList->setItemWidget(operationItem, operationWidget);
	}

	m_operationsList->scrollToBottom();
}

void OperationsList::ResetUI()
{
	for (QListWidgetItem* item : m_operationItems) {
		delete item;
	}

	for (QWidget* widget : m_operationWidgets) {
		delete widget;
	}

	m_operationItems.clear();
	m_operationWidgets.clear();
	m_operationLayouts.clear();
	m_operationLabels.clear();
	m_editOperationButtons.clear();
	m_deleteOperationButtons.clear();
}

bool OperationsList::IsOperationInDisplayableMonth(const Operation& operation, const QDate& currentDate) const
{
	int monthsInAYear = 12;

	assert(m_displayOperationsSinceMonths <= monthsInAYear && "Impossible d'afficher une opération datant de plus d'un an.");

	bool isOperationInAFutureYear = operation.year > currentDate.year();

	if (isOperationInAFutureYear) {
		return true;
	}

	bool isOperationInCurrentYear = operation.year == currentDate.year();

	bool isEveryDisplayableMonthInCurrentYear = currentDate.month() >= m_displayOperationsSinceMonths;

	if (isEveryDisplayableMonthInCurrentYear) {
		int firstDisplayableMonthOfCurrentYear = currentDate.month() - m_displayOperationsSinceMonths + 1;

		return isOperationInCurrentYear && operation.month >= firstDisplayableMonthOfCurrentYear;
	}

	// Every month of current year is displayable
	if (isOperationInCurrentYear) {
		return true;
	}

	bool isOperationInLastYear = operation.year == currentDate.year() - 1;

	int firstDisplayableMonthOfLastYear = currentDate.month() - m_displayOperationsSinceMonths + monthsInAYear + 1;

	return isOperationInLastYear && operation.month >= firstDisplayableMonthOfLastYear;
}

void OperationsList::HandleOperationEdit(const Operation& operation)
{
	EditOperationDialog dialog(operation);
	if (dialog.exec())
	{
		UpdateUI();
		s_DataManager.SaveAccounts();
	}
}

void OperationsList::HandleOperationDelete(int id)
{
	s_DataManager.r_CurrentBankAccount().DeleteOperation(id);
	UpdateUI();
	s_DataManager.SaveAccounts();
}

void OperationsList::HandleDisplayFromChange()
{
	switch (m_displaySinceComboBox->currentIndex()) {
	case 0:
		m_displayOperationsSinceMonths = 2;
		break;
	case 1:
		m_displayOperationsSinceMonths = 6;
		break;
	case 2:
		m_displayOperationsSinceMonths = 12;
		break;
	}

	UpdateUI();
}