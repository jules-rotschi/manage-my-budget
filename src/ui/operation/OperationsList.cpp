#include "OperationsList.h"

#include <iostream>

#include <qdatetime.h>

#include "domain/Operation.h"
#include "domain/Profile.h"
#include "domain/BankAccount.h"
#include "domain/Category.h"

#include "data/StateManager.h"

#include "EditOperationDialog.h"

#include "ui/hooks/ExceptionHandler.h"
#include "ui/format/MonthString.h"
#include "ui/format/StringFormatter.h"

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
	m_displaySinceComboBox->addItem("Toujours");

	switch (m_displayOperationsSinceMonths)
	{
	case 2:
		m_displaySinceComboBox->setCurrentIndex(0);
		break;
	case 6:
		m_displaySinceComboBox->setCurrentIndex(1);
		break;
	case 12:
		m_displaySinceComboBox->setCurrentIndex(2);
		break;
	case -1:
		m_displaySinceComboBox->setCurrentIndex(3);
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

void OperationsList::UpdateUI(bool scrollDown)
{
	m_operationsList->clear();

	m_totalLabel->setText(
		QString::fromStdString("Solde du compte : " + StateManager::Instance().r_CurrentProfile().r_ConstCurrentBankAccount().GetTotalAmount().GetString())
	);

	QDate currentDate = QDate::currentDate();

	int lastOperationYear = 0;
	int lastOperationMonth = 0;

	for (const Operation& operation : StateManager::Instance().r_CurrentProfile().r_ConstCurrentBankAccount().r_Operations())
	{
		if (!IsOperationInDisplayableMonth(operation.year, operation.month, currentDate))
		{
			continue;
		}

		if (lastOperationYear != operation.year || lastOperationMonth != operation.month)
		{
			QLabel* monthLabel = new QLabel(QString::fromStdString(MonthToString(operation.month) + " " + std::to_string(operation.year)));
			monthLabel->setAlignment(Qt::AlignCenter);
			QListWidgetItem* newMonthIndicator = new QListWidgetItem();
			newMonthIndicator->setSizeHint(monthLabel->sizeHint());
			m_operationsList->addItem(newMonthIndicator);
			m_operationsList->setItemWidget(newMonthIndicator, monthLabel);
		}

		QWidget* operationWidget = new QWidget();

		QString descriptionString = !operation.description.empty()
			? " (" + QString::fromStdString(LimitLength(operation.description, 20)) + ")"
			: "";

		QString operationString =
			QString::fromStdString(operation.amount.GetString())
			+ " | "
			+ QString::fromStdString(LimitLength(StateManager::Instance().r_CurrentProfile().categories[operation.categoryIndex].name, 20))
			+ descriptionString;

		if (operation.amount.GetValue() > 0)
		{
			operationString = '+' + operationString;
		}

		QLabel* operationLabel = new QLabel(operationString);

		QPushButton* editOperationButton = new QPushButton("Modifier");
		connect(editOperationButton, &QPushButton::released, [this, operation]() {
			HandleOperationEdit(operation.id);
			});
		editOperationButton->setFixedWidth(100);

		QPushButton* deleteOperationButton = new QPushButton("Supprimer");
		connect(deleteOperationButton, &QPushButton::released, [this, operation]() {
			HandleOperationDelete(operation.id);
			});
		deleteOperationButton->setFixedWidth(100);

		QHBoxLayout* operationLayout = new QHBoxLayout(operationWidget);
		operationLayout->addWidget(operationLabel);
		operationLayout->addWidget(editOperationButton);
		operationLayout->addWidget(deleteOperationButton);

		QListWidgetItem* operationItem = new QListWidgetItem();
		operationItem->setSizeHint(operationWidget->sizeHint());
		m_operationsList->addItem(operationItem);
		m_operationsList->setItemWidget(operationItem, operationWidget);

		lastOperationYear = operation.year;
		lastOperationMonth = operation.month;
	}

	if (scrollDown)
	{
		m_operationsList->scrollToBottom();
	}
}

bool OperationsList::IsOperationInDisplayableMonth(int operationYear, int operationMonth, const QDate& currentDate) const
{
	if (m_displayOperationsSinceMonths < 0)
	{
		return true;
	}

	int monthsInAYear = 12;

	bool isOperationInAFutureYear = operationYear > currentDate.year();

	if (isOperationInAFutureYear)
	{
		return true;
	}

	bool isOperationInCurrentYear = operationYear == currentDate.year();

	bool isEveryDisplayableMonthInCurrentYear = currentDate.month() >= m_displayOperationsSinceMonths;

	if (isEveryDisplayableMonthInCurrentYear)
	{
		int firstDisplayableMonthOfCurrentYear = currentDate.month() - m_displayOperationsSinceMonths + 1;

		return isOperationInCurrentYear && operationMonth >= firstDisplayableMonthOfCurrentYear;
	}

	// Every month of current year is displayable
	if (isOperationInCurrentYear)
	{
		return true;
	}

	bool isOperationInLastYear = operationYear == currentDate.year() - 1;

	int firstDisplayableMonthOfLastYear = currentDate.month() - m_displayOperationsSinceMonths + monthsInAYear + 1;

	return isOperationInLastYear && operationMonth >= firstDisplayableMonthOfLastYear;
}

void OperationsList::HandleOperationEdit(int id)
{
	EditOperationDialog dialog(id);

	if (dialog.exec())
	{
		UpdateUI();
	}
}

void OperationsList::HandleOperationDelete(int id)
{
	try
	{
		StateManager::Instance().DeleteOperation(id);
	}
	catch (const ApplicationException& e)
	{
		HandleException(e);
		return;
	}

	UpdateUI();
}

void OperationsList::HandleDisplayFromChange()
{
	switch (m_displaySinceComboBox->currentIndex())
	{
	case 0:
		m_displayOperationsSinceMonths = 2;
		break;
	case 1:
		m_displayOperationsSinceMonths = 6;
		break;
	case 2:
		m_displayOperationsSinceMonths = 12;
		break;
	case 3:
		m_displayOperationsSinceMonths = -1;
		break;
	}

	UpdateUI(true);
}