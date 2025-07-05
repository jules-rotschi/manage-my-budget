#include "EditOperationDialog.h"

#include <qdatetime.h>

#include "DataManager.h"

EditOperationDialog::EditOperationDialog(const Operation& operation, QWidget* parent)
	: operation(operation), QDialog(parent)
{
	setWindowTitle("Modifier l'opération");

	QDate currentDate = QDate::currentDate();

	m_yearLabel = new QLabel("Année");

	m_yearCombobox = new QComboBox();

	int yearsToDisplay = std::max(currentDate.year() - operation.year, 2);

	for (int i = 0; i <= yearsToDisplay; i++) {
		m_yearCombobox->addItem(QString::fromStdString(std::to_string(currentDate.year() - i)));
	}
	m_yearCombobox->setCurrentIndex(currentDate.year() - operation.year);

	m_monthLabel = new QLabel("Mois");

	m_monthCombobox = new QComboBox();
	for (int i = 0; i < 12; i++) {
		m_monthCombobox->addItem(QString::fromStdString(std::to_string(i + 1)));
	}
	m_monthCombobox->setCurrentIndex(m_monthCombobox->findText(QString::fromStdString(std::to_string(operation.month))));

	m_amountLabel = new QLabel("Montant");
	m_amountLineEdit = new QLineEdit(QString::fromStdString(std::to_string(operation.amount.value / 100)));

	m_amountValidator = new QDoubleValidator();
	m_amountValidator->setDecimals(2);
	m_amountValidator->setNotation(QDoubleValidator::StandardNotation);
	m_amountValidator->setLocale(QLocale::system());

	m_amountLineEdit->setValidator(m_amountValidator);

	m_categoryLabel = new QLabel("Catégorie");

	m_categoryCombobox = new QComboBox();
	for (std::string category : s_DataManager.categories) {
		m_categoryCombobox->addItem(QString::fromStdString(category));
	}
	m_categoryCombobox->setCurrentIndex(operation.categoryIndex);

	m_descriptionLabel = new QLabel("Description");

	m_descriptionLineEdit = new QLineEdit(QString::fromStdString(operation.description));

	m_editButton = new QPushButton("Modifier");
	m_cancelButton = new QPushButton("Annuler");

	m_editButton->isDefault();

	connect(m_editButton, &QPushButton::released, this, &EditOperationDialog::HandleConfirm);
	connect(m_cancelButton, &QPushButton::released, this, &EditOperationDialog::reject);

	m_formLayout = new QFormLayout();
	m_formLayout->addRow(m_yearLabel, m_yearCombobox);
	m_formLayout->addRow(m_monthLabel, m_monthCombobox);
	m_formLayout->addRow(m_categoryLabel, m_categoryCombobox);
	m_formLayout->addRow(m_amountLabel, m_amountLineEdit);
	m_formLayout->addRow(m_descriptionLabel, m_descriptionLineEdit);
	m_formLayout->addWidget(m_editButton);
	m_formLayout->addWidget(m_cancelButton);

	setLayout(m_formLayout);
}

EditOperationDialog::~EditOperationDialog() {}

void EditOperationDialog::HandleConfirm()
{
	bool isYearOk = false;
	bool isMonthOk = false;
	bool isAmountOk = false;

	int year = m_yearCombobox->currentText().toInt(&isYearOk);
	int month = m_monthCombobox->currentText().toInt(&isMonthOk);
	int amount = m_amountLineEdit->text().toDouble(&isAmountOk) * 100;
	int categoryIndex = m_categoryCombobox->currentIndex();
	std::string description = m_descriptionLineEdit->text().toStdString();

	if (!isYearOk || !isMonthOk || !isAmountOk) {
		return;
	}

	Operation newOperation = s_DataManager.bankAccount.GetNewOperation(
		year,
		month,
		amount,
		categoryIndex,
		description
	);

	s_DataManager.bankAccount.EditOperation(operation.id, newOperation);

	accept();
}