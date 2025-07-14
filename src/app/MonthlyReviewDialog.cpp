#include "MonthlyReviewDialog.h"

#include <iostream>

#include <qdatetime.h>
#include <qpushbutton.h>

#include "StateManager.h"
#include "core/Accountant.h"
#include "StringFormatter.h"
#include "MonthString.h"

MonthlyReviewDialog::MonthlyReviewDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle("Bilans mensuels");

	setMinimumWidth(360);

	QDate currentDate = QDate::currentDate();

	m_month = currentDate.month();
	m_year = currentDate.year();

	m_monthSelectorWidget = new QWidget();

	m_monthLabel = new QLabel("Mois");
	m_monthComboBox = new QComboBox();
	for (int i = 0; i < 12; i++)
	{
		m_monthComboBox->addItem(QString::fromStdString(MonthToString(i + 1)));
	}
	m_monthComboBox->setCurrentIndex(m_month - 1);
	connect(m_monthComboBox, &QComboBox::currentIndexChanged, this, &MonthlyReviewDialog::HandleMonthSelectorChange);

	m_yearLabel = new QLabel("Année");
	m_yearComboBox = new QComboBox();
	for (int i = 0; i < 3; i++)
	{
		m_yearComboBox->addItem(QString::fromStdString(std::to_string(m_year - i)));
	}
	connect(m_yearComboBox, &QComboBox::currentIndexChanged, this, &MonthlyReviewDialog::HandleMonthSelectorChange);

	m_monthSelectorLayout = new QFormLayout(m_monthSelectorWidget);
	m_monthSelectorLayout->addRow(m_yearLabel, m_yearComboBox);
	m_monthSelectorLayout->addRow(m_monthLabel, m_monthComboBox);

	m_categoriesList = new QListWidget();

	m_totalWidget = new QWidget();
	m_totalTitleLabel = new QLabel();
	m_totalValueLabel = new QLabel();
	m_totalLayout = new QHBoxLayout(m_totalWidget);
	m_totalLayout->addWidget(m_totalTitleLabel);
	m_totalLayout->addStretch();
	m_totalLayout->addWidget(m_totalValueLabel);

	m_savingsWidget = new QWidget();
	m_savingsTitleLabel = new QLabel("Montant épargné");
	m_savingsValueLabel = new QLabel();
	m_savingsLayout = new QHBoxLayout(m_savingsWidget);
	m_savingsLayout->addWidget(m_savingsTitleLabel);
	m_savingsLayout->addStretch();
	m_savingsLayout->addWidget(m_savingsValueLabel);

	m_defaultButton = new QPushButton("Fermer");
	m_defaultButton->setDefault(true);
	connect(m_defaultButton, &QPushButton::released, this, &MonthlyReviewDialog::accept);

	m_mainLayout = new QVBoxLayout(this);

	UpdateUI();
}

void MonthlyReviewDialog::UpdateUI()
{
	m_categoriesList->clear();

	Accountant accountant(StateManager::Instance().r_CurrentProfile());

	Amount budgetTotal = accountant.GetBudgetTotal();

	m_totalTitleLabel->setText(QString::fromStdString("Total (Prévu : " + budgetTotal.GetString() + ")"));

	for (int i = 1; i < StateManager::Instance().r_CurrentProfile().categories.size(); i++)
	{
		const Category& category = StateManager::Instance().r_CurrentProfile().categories[i];

		Amount monthlyAmount = accountant.GetMonthlyAmount(m_year, m_month, i);
		Amount remainingAmount = -(category.monthlyBudget - monthlyAmount);

		QWidget* categoryReviewWidget = new QWidget();

		QLabel* categoryLabel = new QLabel(QString::fromStdString(LimitLength(category.name, 20) + " (Budget : " + category.monthlyBudget.GetString() + ")"));
		QLabel* amountLabel = new QLabel(QString::fromStdString(monthlyAmount.GetString() + " (Différence budget : " + remainingAmount.GetString() + ")"));

		QHBoxLayout* categoryReviewLayout = new QHBoxLayout(categoryReviewWidget);
		categoryReviewLayout->addWidget(categoryLabel);
		categoryReviewLayout->addStretch();
		categoryReviewLayout->addWidget(amountLabel);

		QListWidgetItem* categoryReviewItem = new QListWidgetItem();
		categoryReviewItem->setSizeHint(categoryReviewWidget->sizeHint());
		m_categoriesList->addItem(categoryReviewItem);
		m_categoriesList->setItemWidget(categoryReviewItem, categoryReviewWidget);
	}

	Amount total = accountant.GetMonthlyAmount(m_year, m_month);
	Amount remaingAmount = -(budgetTotal - total);

	m_totalValueLabel->setText(QString::fromStdString(total.GetString() + " (Différence budget : " + remaingAmount.GetString() + ")"));

	m_savingsValueLabel->setText(QString::fromStdString(accountant.GetMonthlySavings(m_year, m_month).GetString()));
	
	m_mainLayout->addWidget(m_monthSelectorWidget);
	m_mainLayout->addWidget(m_categoriesList);
	m_mainLayout->addWidget(m_totalWidget);
	m_mainLayout->addWidget(m_savingsWidget);
	m_mainLayout->addWidget(m_defaultButton);
}

void MonthlyReviewDialog::HandleMonthSelectorChange()
{
	m_month = m_monthComboBox->currentIndex() + 1;
	m_year = m_yearComboBox->currentText().toInt();

	UpdateUI();
}