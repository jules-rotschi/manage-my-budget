#include "EditAccountDialog.h"

#include "domain/BankAccount.h"
#include "domain/Profile.h"

#include "data/StateManager.h"

#include "exceptions/ApplicationException.h"

#include "ui/format/AmountValueFormatter.h"
#include "ui/hooks/ExceptionHandler.h"

EditAccountDialog::EditAccountDialog(int index, QWidget* parent)
	: m_index(index), QDialog(parent)
{
	setWindowTitle("Modifier le compte");

	BankAccount account = StateManager::Instance().r_CurrentProfile().bankAccounts[m_index];

	m_formWidget = new QWidget();

	m_nameLabel = new QLabel("Nom du compte");
	m_nameLineEdit = new QLineEdit(QString::fromStdString(account.name));

	m_initialAmountLabel = new QLabel("Solde initial");
	m_initialAmountLineEdit = new QLineEdit(QString::fromStdString(FormatToLineEdit(account.initialAmount.GetValue())));

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

	switch (account.type)
	{
	case AccountType::SAVING:
		m_typeCombobox->setCurrentIndex(1);
		break;
	case AccountType::CURRENT:
	default:
		m_typeCombobox->setCurrentIndex(0);
	}

	m_formLayout = new QFormLayout(m_formWidget);
	m_formLayout->addRow(m_nameLabel, m_nameLineEdit);
	m_formLayout->addRow(m_initialAmountLabel, m_initialAmountLineEdit);
	m_formLayout->addRow(m_typeLabel, m_typeCombobox);

	m_buttonsWidget = new QWidget();

	m_editButton = new QPushButton("Modifier");
	connect(m_editButton, &QPushButton::released, this, &EditAccountDialog::HandleConfirm);
	m_editButton->setDefault(true);
	m_editButton->setMinimumWidth(100);
	
	m_cancelButton = new QPushButton("Annuler");
	connect(m_cancelButton, &QPushButton::released, this, &EditAccountDialog::reject);
	m_cancelButton->setMinimumWidth(100);

	m_buttonsLayout = new QHBoxLayout(m_buttonsWidget);
	m_buttonsLayout->addWidget(m_editButton);
	m_buttonsLayout->addWidget(m_cancelButton);

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_formWidget);
	m_layout->addWidget(m_buttonsWidget);
}

void EditAccountDialog::HandleConfirm()
{
	std::string name = m_nameLineEdit->text().toStdString();
	std::string type = m_typeCombobox->currentText().toStdString();

	bool isInitialAmountOk = false;
	long initialAmountValue = QLocale::system().toDouble(m_initialAmountLineEdit->text(), &isInitialAmountOk) * 100;

	if (!isInitialAmountOk)
	{
		return;
	}

	try
	{
		StateManager::Instance().EditAccount(m_index, name, type, initialAmountValue);
	}
	catch (const ApplicationException& e)
	{
		HandleException(e);
		return;
	}

	accept();
}