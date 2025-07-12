#include "AddAccountDialog.h"

#include <qlineedit.h>
#include <qpushbutton.h>

#include "DataManager.h"
#include "ExceptionHandler.h"

AddAccountDialog::AddAccountDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle("Ajouter un compte bancaire");

	m_formWidget = new QWidget();

	m_nameLabel = new QLabel("Nom du compte");
	m_nameLineEdit = new QLineEdit();

	m_initialAmountLabel = new QLabel("Solde initial");
	m_initialAmountLineEdit = new QLineEdit("0");

	m_initialAmountValidator = new QDoubleValidator();
	m_initialAmountValidator->setDecimals(2);
	m_initialAmountValidator->setNotation(QDoubleValidator::StandardNotation);
	m_initialAmountValidator->setLocale(QLocale::system());
	m_initialAmountValidator->setRange(-1000000, 1000000);

	m_initialAmountLineEdit->setValidator(m_initialAmountValidator);

	m_typeLabel = new QLabel("Type");

	m_typeCombobox = new QComboBox();
	m_typeCombobox->addItem("Compte courant");
	m_typeCombobox->addItem("Épargne");

	m_formLayout = new QFormLayout(m_formWidget);
	m_formLayout->addRow(m_nameLabel, m_nameLineEdit);
	m_formLayout->addRow(m_initialAmountLabel, m_initialAmountLineEdit);
	m_formLayout->addRow(m_typeLabel, m_typeCombobox);

	m_buttonsWidget = new QWidget();

	m_addButton = new QPushButton("Ajouter le compte");
	connect(m_addButton, &QPushButton::released, this, &AddAccountDialog::HandleConfirm);
	m_addButton->setDefault(true);
	m_addButton->setMinimumWidth(150);

	m_cancelButton = new QPushButton("Annuler");
	connect(m_cancelButton, &QPushButton::released, this, &AddAccountDialog::reject);
	m_cancelButton->setMinimumWidth(150);

	m_buttonsLayout = new QHBoxLayout(m_buttonsWidget);
	m_buttonsLayout->addWidget(m_addButton);
	m_buttonsLayout->addWidget(m_cancelButton);

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_formWidget);
	m_layout->addWidget(m_buttonsWidget);
}

void AddAccountDialog::HandleConfirm()
{
	std::string name = m_nameLineEdit->text().toStdString();
	std::string type = m_typeCombobox->currentText().toStdString();
	
	bool isInitialAmountOk = false;
	long initialAmountValue = QLocale::system().toDouble(m_initialAmountLineEdit->text(), &isInitialAmountOk) * 100;

	if (!isInitialAmountOk) {
		return;
	}

	try {
		s_DataManager.AddAccount(name, type, initialAmountValue);
	}
	catch (const ApplicationException& e) {
		HandleException(e);
		return;
	}
	
	accept();
}