#include "AddOperationForm.h"

#include <iostream>

#include <qdatetime.h>
#include <qlineedit.h>

#include "StateManager.h"
#include "ExceptionHandler.h"
#include "MonthString.h"
#include "StringFormatter.h"

AddOperationForm::AddOperationForm(QWidget* parent)
	: QWidget(parent)
{
	QDate currentDate = QDate::currentDate();

	m_typeLabel = new QLabel("Type");
	m_typeCombobox = new QComboBox();
	m_typeCombobox->addItem("Crédit");
	m_typeCombobox->addItem("Débit");

	m_yearLabel = new QLabel("Année");
	m_yearCombobox = new QComboBox();
	for (int i = 0; i <= 2; i++) {
		m_yearCombobox->addItem(QString::fromStdString(std::to_string(currentDate.year() - i)));
	}

	m_monthLabel = new QLabel("Mois");
	m_monthCombobox = new QComboBox();
	for (int i = 0; i < 12; i++) {
		m_monthCombobox->addItem(QString::fromStdString(MonthToString(i + 1)));
	}
	m_monthCombobox->setCurrentIndex(currentDate.month() - 1);

	m_amountLabel = new QLabel("Montant");
	m_amountLineEdit = new QLineEdit();

	m_amountValidator = new QDoubleValidator();
	m_amountValidator->setDecimals(2);
	m_amountValidator->setNotation(QDoubleValidator::StandardNotation);
	m_amountValidator->setLocale(QLocale::system());
	m_amountValidator->setRange(0, 1000000);

	m_amountLineEdit->setValidator(m_amountValidator);

	m_categoryLabel = new QLabel("Catégorie");
	m_categoryCombobox = new QComboBox();
	LoadCategories();

	m_descriptionLabel = new QLabel("Description");
	m_descriptionLineEdit = new QLineEdit();

	m_addButton = new QPushButton("Ajouter l'opération");
	connect(m_addButton, &QPushButton::released, this, &AddOperationForm::HandleAddButton);

	m_mainLayout = new QFormLayout(this);
	m_mainLayout->addRow(m_typeLabel, m_typeCombobox);
	m_mainLayout->addRow(m_yearLabel, m_yearCombobox);
	m_mainLayout->addRow(m_monthLabel, m_monthCombobox);
	m_mainLayout->addRow(m_amountLabel, m_amountLineEdit);
	m_mainLayout->addRow(m_categoryLabel, m_categoryCombobox);
	m_mainLayout->addRow(m_descriptionLabel, m_descriptionLineEdit);
	m_mainLayout->addWidget(m_addButton);
	m_addButton->setDefault(true);
}

void AddOperationForm::LoadCategories()
{
	m_categoryCombobox->clear();
	for (const Category& category : StateManager::Instance().r_CurrentProfile().categories) {
		m_categoryCombobox->addItem(QString::fromStdString(LimitLength(category.name, 20)));
	}
}

void AddOperationForm::ResetForm()
{
	m_amountLineEdit->setText("");
	m_descriptionLineEdit->setText("");
	m_amountLineEdit->setFocus();
}

void AddOperationForm::HandleAddButton()
{
	bool isAmountOk = false;

	bool isDebit = m_typeCombobox->currentIndex();

	int year = m_yearCombobox->currentText().toInt();
	int month = m_monthCombobox->currentIndex() + 1;
	int categoryIndex = m_categoryCombobox->currentIndex();
	unsigned long absoluteAmountValue = QLocale::system().toDouble(m_amountLineEdit->text(), &isAmountOk) * 100;
	std::string description = m_descriptionLineEdit->text().toStdString();

	if (!isAmountOk) {
		return;
	}
	
	long amountValue = isDebit ? -(long)absoluteAmountValue : absoluteAmountValue;

	try {
		StateManager::Instance().AddOperation(year, month, amountValue, categoryIndex, description);
	}
	catch (const ApplicationException& e) {
		HandleException(e);
		return;
	}

	emit OperationAdd();

	ResetForm();
}