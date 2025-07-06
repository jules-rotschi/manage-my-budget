#include "ManageAccountsDialog.h"

#include "app/DataManager.h"
#include "app/RenameCategoryDialog.h"
#include "app/AddAccountDialog.h"
#include "app/EditAccountDialog.h"

ManageAccountsDialog::ManageAccountsDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle("Comptes bancaires");

	m_accountsWidget = new QWidget();
	m_accountsLayout = new QVBoxLayout(m_accountsWidget);

	m_addButton = new QPushButton("Ajouter un compte");

	connect(m_addButton, &QPushButton::released, this, &ManageAccountsDialog::HandleAccountAdd);

	m_defaultButton = new QPushButton("Terminé");
	m_defaultButton->setDefault(true);

	connect(m_defaultButton, &QPushButton::released, this, &ManageAccountsDialog::accept);

	m_mainLayout = new QVBoxLayout(this);
	m_mainLayout->addWidget(m_accountsWidget);
	m_mainLayout->addWidget(m_addButton);
	m_mainLayout->addWidget(m_defaultButton);

	UpdateUI();
}

void ManageAccountsDialog::ResetUI()
{
	for (QWidget* widget : m_accountWidgets) {
		delete widget;
	}

	m_accountWidgets.clear();
	m_accountLayouts.clear();
	m_accountLabels.clear();
	m_accountEditButtons.clear();
	m_accountDeleteButtons.clear();
}

void ManageAccountsDialog::UpdateUI()
{
	ResetUI();

	for (int i = 0; i < s_DataManager.bankAccounts.size(); i++) {
		BankAccount account = s_DataManager.bankAccounts[i];

		QWidget* accountWidget = new QWidget();
		QHBoxLayout* accountLayout = new QHBoxLayout(accountWidget);
		QLabel* accountLabel = new QLabel(QString::fromStdString(account.name + " (" + account.GetTypeString() + ") | " + account.GetTotalAmount().GetString()));
		QPushButton* accountEditButton = new QPushButton("Modifier");
		QPushButton* accountDeleteButton = new QPushButton("Supprimer");

		connect(
			accountEditButton,
			&QPushButton::released,
			[this, i]() {HandleAccountEdit(i); }
		);

		connect(
			accountDeleteButton,
			&QPushButton::released,
			[this, i]() {HandleAccountDelete(i); }
		);

		m_accountWidgets.push_back(accountWidget);
		m_accountLayouts.push_back(accountLayout);
		m_accountLabels.push_back(accountLabel);
		m_accountEditButtons.push_back(accountEditButton);
		m_accountDeleteButtons.push_back(accountDeleteButton);

		accountLayout->addWidget(accountLabel);
		accountLayout->addWidget(accountEditButton);
		accountLayout->addWidget(accountDeleteButton);

		m_accountsLayout->addWidget(accountWidget);
	}
}

ManageAccountsDialog::~ManageAccountsDialog() {}

void ManageAccountsDialog::HandleAccountAdd()
{
	AddAccountDialog dialog;

	if (dialog.exec()) {
		UpdateUI();
		s_DataManager.SaveAccounts();
	}
}

void ManageAccountsDialog::HandleAccountEdit(int index)
{
	EditAccountDialog dialog(index);

	if (dialog.exec()) {
		UpdateUI();
		s_DataManager.SaveAccounts();
	}
}

void ManageAccountsDialog::HandleAccountDelete(int index)
{
	if (s_DataManager.DeleteAccount(index)) {
		UpdateUI();
		s_DataManager.SaveAccounts();
	}
}