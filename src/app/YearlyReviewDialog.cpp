#include "YearlyReviewDialog.h"

#include <iostream>

#include <qdatetime.h>
#include <qpushbutton.h>

#include "app/DataManager.h"
#include "core/Accountant.h"

YearlyReviewDialog::YearlyReviewDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle("Bilans annuels");

	QDate currentDate = QDate::currentDate();

	m_year = currentDate.year();

	Accountant accountant(s_DataManager.bankAccounts);

	m_mainLayout = new QVBoxLayout();

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

	m_totalLabel = new QLabel();

	m_savingsLabel = new QLabel();

	m_defaultButton = new QPushButton("Fermer");
	m_defaultButton->setDefault(true);

	connect(m_defaultButton, &QPushButton::released, this, &YearlyReviewDialog::accept);

	UpdateUI();

	setLayout(m_mainLayout);
}

void YearlyReviewDialog::UpdateUI()
{
	ResetUI();

	Accountant accountant(s_DataManager.bankAccounts);

	for (int i = 1; i < s_DataManager.categories.size(); i++) {
		m_categoriesList->addItem(QString::fromStdString(
				s_DataManager.categories[i] + " : " + accountant.GetYearlyAmount(m_year, i).GetString()
			));

	}

	m_totalLabel->setText(QString::fromStdString("Total : " + accountant.GetYearlyAmount(m_year).GetString()));

	m_savingsLabel->setText(QString::fromStdString("Montant épargné : " + accountant.GetYearlySavings(m_year).GetString()));

	m_mainLayout->addWidget(m_yearSelectorWidget);
	m_mainLayout->addWidget(m_categoriesList);
	m_mainLayout->addWidget(m_totalLabel);
	m_mainLayout->addWidget(m_savingsLabel);
	m_mainLayout->addWidget(m_defaultButton);
}

void YearlyReviewDialog::ResetUI()
{
	m_categoriesList->clear();
}

void YearlyReviewDialog::HandleYearSelectorChange()
{
	m_year = m_yearComboBox->currentText().toInt();

	UpdateUI();
}