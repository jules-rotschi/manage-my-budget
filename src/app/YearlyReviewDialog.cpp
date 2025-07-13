#include "YearlyReviewDialog.h"

#include <iostream>

#include <qdatetime.h>
#include <qpushbutton.h>

#include "DataManager.h"
#include "core/Accountant.h"
#include "StringFormatter.h"

YearlyReviewDialog::YearlyReviewDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle("Bilans annuels");

	setMinimumWidth(360);

	QDate currentDate = QDate::currentDate();

	m_year = currentDate.year();

	m_yearSelectorWidget = new QWidget();

	m_yearLabel = new QLabel("Année");
	m_yearComboBox = new QComboBox();
	for (int i = 0; i < 3; i++) {
		m_yearComboBox->addItem(QString::fromStdString(std::to_string(m_year - i)));
	}
	connect(m_yearComboBox, &QComboBox::currentIndexChanged, this, &YearlyReviewDialog::HandleYearSelectorChange);

	m_yearSelectorLayout = new QFormLayout(m_yearSelectorWidget);
	m_yearSelectorLayout->addRow(m_yearLabel, m_yearComboBox);

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
	connect(m_defaultButton, &QPushButton::released, this, &YearlyReviewDialog::accept);

	m_mainLayout = new QVBoxLayout(this);

	UpdateUI();
}

void YearlyReviewDialog::UpdateUI()
{
	m_categoriesList->clear();

	Accountant accountant(s_DataManager.r_CurrentProfile());

	Amount budgetTotal = accountant.GetBudgetTotal() * 12;

	m_totalTitleLabel->setText(QString::fromStdString("Total (Prévu : " + budgetTotal.GetString() + ")"));

	for (int i = 1; i < s_DataManager.r_CurrentProfile().categories.size(); i++) {
		const Category& category = s_DataManager.r_CurrentProfile().categories[i];

		Amount yearlyAmount = accountant.GetYearlyAmount(m_year, i);
		Amount yearlyBudget = category.monthlyBudget * 12;
		Amount remainingAmount = -(yearlyBudget - yearlyAmount);

		QWidget* categoryReviewWidget = new QWidget();

		QLabel* categoryLabel = new QLabel(QString::fromStdString(LimitLength(category.name, 20) + " (Budget : " + yearlyBudget.GetString() + ")"));
		QLabel* amountLabel = new QLabel(QString::fromStdString(accountant.GetYearlyAmount(m_year, i).GetString() + " (Différence budget : " + remainingAmount.GetString() + ")"));

		QHBoxLayout* categoryReviewLayout = new QHBoxLayout(categoryReviewWidget);
		categoryReviewLayout->addWidget(categoryLabel);
		categoryReviewLayout->addStretch();
		categoryReviewLayout->addWidget(amountLabel);

		QListWidgetItem* categoryReviewItem = new QListWidgetItem();
		categoryReviewItem->setSizeHint(categoryReviewWidget->sizeHint());
		m_categoriesList->addItem(categoryReviewItem);
		m_categoriesList->setItemWidget(categoryReviewItem, categoryReviewWidget);
	}

	Amount total = accountant.GetYearlyAmount(m_year);
	Amount remaingAmount = -(budgetTotal - total);

	m_totalValueLabel->setText(QString::fromStdString(total.GetString() + " (Différence budget : " + remaingAmount.GetString() + ")"));

	m_savingsValueLabel->setText(QString::fromStdString(accountant.GetYearlySavings(m_year).GetString()));

	m_mainLayout->addWidget(m_yearSelectorWidget);
	m_mainLayout->addWidget(m_categoriesList);
	m_mainLayout->addWidget(m_totalWidget);
	m_mainLayout->addWidget(m_savingsWidget);
	m_mainLayout->addWidget(m_defaultButton);
}

void YearlyReviewDialog::HandleYearSelectorChange()
{
	m_year = m_yearComboBox->currentText().toInt();

	UpdateUI();
}