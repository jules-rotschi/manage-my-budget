#include "AddAccountDialog.h"

#include <qlineedit.h>
#include <qpushbutton.h>

#include "app/DataManager.h"
#include "core/BankAccount.h"

AddAccountDialog::AddAccountDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle("Ajouter un compte bancaire");

	m_nameLabel = new QLabel("Nom du compte");
	m_nameLineEdit = new QLineEdit();

	m_initialAmountLabel = new QLabel("Solde initial");
	m_initialAmountLineEdit = new QLineEdit("0");

	m_initialAmountValidator = new QDoubleValidator();
	m_initialAmountValidator->setDecimals(2);
	m_initialAmountValidator->setNotation(QDoubleValidator::StandardNotation);
	m_initialAmountValidator->setLocale(QLocale::system());

	m_initialAmountLineEdit->setValidator(m_initialAmountValidator);

	m_typeLabel = new QLabel("Type");

	m_typeCombobox = new QComboBox();
	m_typeCombobox->addItem("Compte courant");
	m_typeCombobox->addItem("Épargne");

	m_addButton = new QPushButton("Ajouter le compte");
	m_cancelButton = new QPushButton("Annuler");

	m_addButton->setDefault(true);

	connect(m_addButton, &QPushButton::released, this, &AddAccountDialog::HandleConfirm);
	connect(m_cancelButton, &QPushButton::released, this, &AddAccountDialog::reject);

	m_formLayout = new QFormLayout();
	m_formLayout->addRow(m_nameLabel, m_nameLineEdit);
	m_formLayout->addRow(m_initialAmountLabel, m_initialAmountLineEdit);
	m_formLayout->addRow(m_typeLabel, m_typeCombobox);
	m_formLayout->addRow(m_addButton, m_cancelButton);

	setLayout(m_formLayout);
}

void AddAccountDialog::HandleConfirm()
{
	bool isInitialAmountOk = false;
	int initialAmount = m_initialAmountLineEdit->text().toDouble(&isInitialAmountOk) * 100;

	if (!isInitialAmountOk) {
		return;
	}

	BankAccount newAccount;
	newAccount.name = m_nameLineEdit->text().toStdString();
	newAccount.initialAmount = initialAmount;
	
	switch (m_typeCombobox->currentIndex()) {
	case 1:
		newAccount.type = AccountType::SAVING;
		break;
	case 0:
	default:
		newAccount.type = AccountType::CURRENT;
	}

	if (!s_DataManager.r_CurrentProfile().AddAccount(newAccount)) {
		return;
	}
	
	accept();
}