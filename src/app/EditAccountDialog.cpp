#include "EditAccountDialog.h"

#include "DataManager.h"

EditAccountDialog::EditAccountDialog(int index, QWidget* parent)
	: index(index), QDialog(parent)
{
	setWindowTitle("Modifier le compte");

	BankAccount account = s_DataManager.bankAccounts[index];

	m_nameLabel = new QLabel("Nom du compte");
	m_nameLineEdit = new QLineEdit(QString::fromStdString(account.name));

	m_initialAmountLabel = new QLabel("Solde initial");
	m_initialAmountLineEdit = new QLineEdit(QString::fromStdString(std::to_string(account.initialAmount.value / 100)));

	m_initialAmountValidator = new QDoubleValidator();
	m_initialAmountValidator->setDecimals(2);
	m_initialAmountValidator->setNotation(QDoubleValidator::StandardNotation);
	m_initialAmountValidator->setLocale(QLocale::system());

	m_initialAmountLineEdit->setValidator(m_initialAmountValidator);
	
	m_typeLabel = new QLabel("Type");
	m_typeCombobox = new QComboBox();

	m_typeCombobox->addItem("Compte courant");
	m_typeCombobox->addItem("Épargne");

	switch (account.type) {
	case AccountType::SAVING:
		m_typeCombobox->setCurrentIndex(1);
		break;
	case AccountType::CURRENT:
	default:
		m_typeCombobox->setCurrentIndex(0);
	}

	m_editButton = new QPushButton("Modifier");
	m_cancelButton = new QPushButton("Annuler");

	m_editButton->setDefault(true);

	connect(m_editButton, &QPushButton::released, this, &EditAccountDialog::HandleConfirm);
	connect(m_cancelButton, &QPushButton::released, this, &EditAccountDialog::reject);

	m_formLayout = new QFormLayout();
	m_formLayout->addRow(m_nameLabel, m_nameLineEdit);
	m_formLayout->addRow(m_initialAmountLabel, m_initialAmountLineEdit);
	m_formLayout->addRow(m_typeLabel, m_typeCombobox);
	m_formLayout->addRow(m_editButton, m_cancelButton);

	setLayout(m_formLayout);
}

void EditAccountDialog::HandleConfirm()
{
	bool isInitialAmountOk = false;
	int initialAmount = m_initialAmountLineEdit->text().toDouble(&isInitialAmountOk) * 100;

	if (!isInitialAmountOk) {
		return;
	}

	BankAccount account;
	account.name = m_nameLineEdit->text().toStdString();
	account.initialAmount = initialAmount;

	switch (m_typeCombobox->currentIndex()) {
	case 1:
		account.type = AccountType::SAVING;
		break;
	case 0:
	default:
		account.type = AccountType::CURRENT;
	}

	std::string oldAccountName = s_DataManager.bankAccounts[index].name;

	if (!s_DataManager.EditAccount(index, account, oldAccountName)) {
		return;
	}

	accept();
}