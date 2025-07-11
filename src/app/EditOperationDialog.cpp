#include "EditOperationDialog.h"

#include <algorithm>

#include <qdatetime.h>

#include "DataManager.h"
#include "AmountValueFormatter.h"
#include "ExceptionHandler.h"
#include "StringFormatter.h"
#include "MonthString.h"

EditOperationDialog::EditOperationDialog(int id, QWidget* parent)
	: m_id(id), QDialog(parent)
{
	Operation operationToEdit;

	for (const Operation& operation : s_DataManager.r_CurrentProfile().r_ConstCurrentBankAccount().r_Operations()) {
		if (operation.id == id) {
			operationToEdit = operation;
			break;
		}
	}

	setWindowTitle("Modifier l'opération");

	QDate currentDate = QDate::currentDate();

	m_formWidget = new QWidget();

	m_yearLabel = new QLabel("Année");
	m_yearCombobox = new QComboBox();
	int yearsToDisplay = std::max<int>(currentDate.year() - operationToEdit.year, 2);
	for (int i = 0; i <= yearsToDisplay; i++) {
		m_yearCombobox->addItem(QString::fromStdString(std::to_string(currentDate.year() - i)));
	}
	m_yearCombobox->setCurrentIndex(currentDate.year() - operationToEdit.year);

	m_monthLabel = new QLabel("Mois");
	m_monthCombobox = new QComboBox();
	for (int i = 0; i < 12; i++) {
		m_monthCombobox->addItem(QString::fromStdString(MonthToString(i + 1)));
	}
	m_monthCombobox->setCurrentIndex(operationToEdit.month - 1);

	m_amountLabel = new QLabel("Montant");
	m_amountLineEdit = new QLineEdit(QString::fromStdString(FormatToLineEdit(operationToEdit.amount.GetValue())));

	m_amountValidator = new QDoubleValidator();
	m_amountValidator->setDecimals(2);
	m_amountValidator->setNotation(QDoubleValidator::StandardNotation);
	m_amountValidator->setLocale(QLocale::system());

	m_amountLineEdit->setValidator(m_amountValidator);

	m_categoryLabel = new QLabel("Catégorie");
	m_categoryCombobox = new QComboBox();
	for (std::string category : s_DataManager.r_CurrentProfile().categories) {
		m_categoryCombobox->addItem(QString::fromStdString(LimitLength(category, 20)));
	}
	m_categoryCombobox->setCurrentIndex(operationToEdit.categoryIndex);

	m_descriptionLabel = new QLabel("Description");
	m_descriptionLineEdit = new QLineEdit(QString::fromStdString(operationToEdit.description));

	m_formLayout = new QFormLayout(m_formWidget);
	m_formLayout->addRow(m_yearLabel, m_yearCombobox);
	m_formLayout->addRow(m_monthLabel, m_monthCombobox);
	m_formLayout->addRow(m_amountLabel, m_amountLineEdit);
	m_formLayout->addRow(m_categoryLabel, m_categoryCombobox);
	m_formLayout->addRow(m_descriptionLabel, m_descriptionLineEdit);

	m_buttonsWidget = new QWidget(this);

	m_editButton = new QPushButton("Modifier");
	m_editButton->setDefault(true);
	connect(m_editButton, &QPushButton::released, this, &EditOperationDialog::HandleConfirm);
	m_editButton->setMinimumWidth(100);
	
	m_cancelButton = new QPushButton("Annuler");
	connect(m_cancelButton, &QPushButton::released, this, &EditOperationDialog::reject);
	m_cancelButton->setMinimumWidth(100);

	m_buttonsLayout = new QHBoxLayout(m_buttonsWidget);
	m_buttonsLayout->addWidget(m_editButton);
	m_buttonsLayout->addWidget(m_cancelButton);
	
	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_formWidget);
	m_layout->addWidget(m_buttonsWidget);
}

void EditOperationDialog::HandleConfirm()
{
	bool isAmountOk = false;

	int year = m_yearCombobox->currentText().toInt();
	int month = m_monthCombobox->currentText().toInt();
	int amount = QLocale::system().toDouble(m_amountLineEdit->text(), &isAmountOk) * 100;
	int categoryIndex = m_categoryCombobox->currentIndex();
	std::string description = m_descriptionLineEdit->text().toStdString();

	if (!isAmountOk) {
		return;
	}

	try {
		s_DataManager.EditOperation(m_id, year, month, amount, categoryIndex, description);
	}
	catch (const ApplicationException& e) {
		HandleException(e);
		return;
	}

	accept();
}