#include "MonthlyReviewDialog.h"

#include <iostream>

#include <qdatetime.h>
#include <qpushbutton.h>

#include "DataManager.h"
#include "core/Accountant.h"

MonthlyReviewDialog::MonthlyReviewDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle("Bilans mensuels");

	QDate currentDate = QDate::currentDate();

	m_month = currentDate.month();
	m_year = currentDate.year();

	m_monthSelectorWidget = new QWidget();

	m_monthLabel = new QLabel("Mois");
	m_monthComboBox = new QComboBox();
	for (int i = 0; i < 12; i++) {
		m_monthComboBox->addItem(QString::fromStdString(std::to_string(i + 1)));
	}
	m_monthComboBox->setCurrentIndex(m_month - 1);
	connect(m_monthComboBox, &QComboBox::currentIndexChanged, this, &MonthlyReviewDialog::HandleMonthSelectorChange);

	m_yearLabel = new QLabel("Année");
	m_yearComboBox = new QComboBox();
	for (int i = 0; i < 3; i++) {
		m_yearComboBox->addItem(QString::fromStdString(std::to_string(m_year - i)));
	}
	connect(m_yearComboBox, &QComboBox::currentIndexChanged, this, &MonthlyReviewDialog::HandleMonthSelectorChange);

	m_monthSelectorLayout = new QFormLayout(m_monthSelectorWidget);
	m_monthSelectorLayout->addRow(m_yearLabel, m_yearComboBox);
	m_monthSelectorLayout->addRow(m_monthLabel, m_monthComboBox);

	m_categoriesList = new QListWidget();

	m_totalLabel = new QLabel();

	m_savingsLabel = new QLabel();

	m_defaultButton = new QPushButton("Fermer");
	m_defaultButton->setDefault(true);
	connect(m_defaultButton, &QPushButton::released, this, &MonthlyReviewDialog::accept);

	m_mainLayout = new QVBoxLayout(this);
	
	UpdateUI();
}

void MonthlyReviewDialog::UpdateUI()
{
	m_categoriesList->clear();

	Accountant accountant(s_DataManager.r_CurrentProfile().bankAccounts);

	for (int i = 1; i < s_DataManager.r_CurrentProfile().categories.size(); i++) {
		m_categoriesList->addItem(QString::fromStdString(
			s_DataManager.r_CurrentProfile().categories[i] + " : " + accountant.GetMonthlyAmount(m_year, m_month, i).GetString()
		));
	}

	m_totalLabel->setText(QString::fromStdString("Total : " + accountant.GetMonthlyAmount(m_year, m_month).GetString()));

	m_savingsLabel->setText(QString::fromStdString("Montant épargné : " + accountant.GetMonthlySavings(m_year, m_month).GetString()));
	
	m_mainLayout->addWidget(m_monthSelectorWidget);
	m_mainLayout->addWidget(m_categoriesList);
	m_mainLayout->addWidget(m_totalLabel);
	m_mainLayout->addWidget(m_savingsLabel);
	m_mainLayout->addWidget(m_defaultButton);
}

void MonthlyReviewDialog::HandleMonthSelectorChange()
{
	m_month = m_monthComboBox->currentIndex() + 1;
	m_year = m_yearComboBox->currentText().toInt();

	UpdateUI();
}