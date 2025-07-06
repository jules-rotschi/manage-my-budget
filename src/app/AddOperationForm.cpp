#include "AddOperationForm.h"

#include <iostream>

#include <qdatetime.h>
#include <qlineedit.h>

#include "DataManager.h"
#include "core/Operation.h"

AddOperationForm::AddOperationForm(QWidget* parent)
	: QWidget(parent)
{
	QDate currentDate = QDate::currentDate();

	m_yearLabel = new QLabel("Année");

	m_yearCombobox = new QComboBox();
	for (int i = 0; i <= 2; i++) {
		m_yearCombobox->addItem(QString::fromStdString(std::to_string(currentDate.year() - i)));
	}

	m_monthLabel = new QLabel("Mois");

	m_monthCombobox = new QComboBox();
	for (int i = 0; i < 12; i++) {
		m_monthCombobox->addItem(QString::fromStdString(std::to_string(i + 1)));
	}
	m_monthCombobox->setCurrentIndex(m_monthCombobox->findText(QString::fromStdString(std::to_string(currentDate.month()))));

	m_amountLabel = new QLabel("Montant");
	m_amountLineEdit = new QLineEdit();

	m_amountValidator = new QDoubleValidator();
	m_amountValidator->setDecimals(2);
	m_amountValidator->setNotation(QDoubleValidator::StandardNotation);
	m_amountValidator->setLocale(QLocale::system());

	m_amountLineEdit->setValidator(m_amountValidator);

	m_categoryLabel = new QLabel("Catégorie");

	m_categoryCombobox = new QComboBox();
	LoadCategories();

	m_descriptionLabel = new QLabel("Description");

	m_descriptionLineEdit = new QLineEdit();

	m_addButton = new QPushButton("Ajouter l'opération");
	m_addButton->setDefault(true);

	connect(m_addButton, &QPushButton::released, this, &AddOperationForm::HandleAddButton);

	m_mainLayout = new QFormLayout(this);
	m_mainLayout->addRow(m_yearLabel, m_yearCombobox);
	m_mainLayout->addRow(m_monthLabel, m_monthCombobox);
	m_mainLayout->addRow(m_categoryLabel, m_categoryCombobox);
	m_mainLayout->addRow(m_amountLabel, m_amountLineEdit);
	m_mainLayout->addRow(m_descriptionLabel, m_descriptionLineEdit);
	m_mainLayout->addWidget(m_addButton);
}

AddOperationForm::~AddOperationForm() {}

void AddOperationForm::LoadCategories()
{
	m_categoryCombobox->clear();
	for (std::string category : s_DataManager.categories) {
		m_categoryCombobox->addItem(QString::fromStdString(category));
	}
}

void AddOperationForm::ResetForm()
{
	m_amountLineEdit->setText("");
	m_descriptionLineEdit->setText("");
}

void AddOperationForm::HandleAddButton()
{
	bool isYearOk = false;
	bool isMonthOk = false;
	bool isAmountOk = false;

	int year = m_yearCombobox->currentText().toInt(&isYearOk);
	int month = m_monthCombobox->currentText().toInt(&isMonthOk);
	int amount = m_amountLineEdit->text().toDouble(&isAmountOk) * 100;
	int categoryIndex = m_categoryCombobox->currentIndex();
	std::string description = m_descriptionLineEdit->text().toStdString();

	std::cout << categoryIndex << std::endl;

	if (!isYearOk || !isMonthOk || !isAmountOk) {
		std::cout << isYearOk << isMonthOk << isAmountOk << std::endl;
		return;
	}

	m_amountLineEdit->setText("");
	m_descriptionLineEdit->setText("");

	Operation operation = s_DataManager.r_CurrentBankAccount().GetNewOperation(
		year,
		month,
		amount,
		categoryIndex,
		description
	);

	emit OperationAdd(operation);

	ResetForm();
}