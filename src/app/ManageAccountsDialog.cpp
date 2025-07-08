#include "ManageAccountsDialog.h"

#include "app/DataManager.h"
#include "app/RenameCategoryDialog.h"
#include "app/AddAccountDialog.h"
#include "app/EditAccountDialog.h"

ManageAccountsDialog::ManageAccountsDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle("Comptes bancaires");

	m_accountsList = new QListWidget();

	m_addButton = new QPushButton("Ajouter un compte");

	connect(m_addButton, &QPushButton::released, this, &ManageAccountsDialog::HandleAccountAdd);

	m_defaultButton = new QPushButton("Terminé");
	m_defaultButton->setDefault(true);

	connect(m_defaultButton, &QPushButton::released, this, &ManageAccountsDialog::accept);

	m_mainLayout = new QVBoxLayout(this);
	m_mainLayout->addWidget(m_accountsList);
	m_mainLayout->addWidget(m_addButton);
	m_mainLayout->addWidget(m_defaultButton);

	UpdateUI();
}

void ManageAccountsDialog::ResetUI()
{
	for (QListWidgetItem* item : m_accountItems) {
		delete item;
	}

	for (QWidget* widget : m_accountWidgets) {
		delete widget;
	}

	m_accountItems.clear();
	m_accountWidgets.clear();
}

void ManageAccountsDialog::UpdateUI()
{
	ResetUI();

	for (int i = 0; i < s_DataManager.r_CurrentProfile().bankAccounts.size(); i++) {
		BankAccount account = s_DataManager.r_CurrentProfile().bankAccounts[i];

		QListWidgetItem* accountItem = new QListWidgetItem();
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

		m_accountItems.push_back(accountItem);
		m_accountWidgets.push_back(accountWidget);

		accountLayout->addWidget(accountLabel);
		accountLayout->addWidget(accountEditButton);
		accountLayout->addWidget(accountDeleteButton);

		accountItem->setSizeHint(accountWidget->sizeHint());
		m_accountsList->addItem(accountItem);
		m_accountsList->setItemWidget(accountItem, accountWidget);
	}
}

ManageAccountsDialog::~ManageAccountsDialog() {}

void ManageAccountsDialog::HandleAccountAdd()
{
	AddAccountDialog dialog;

	if (dialog.exec()) {
		UpdateUI();
		s_DataManager.SaveProfiles();
	}
}

void ManageAccountsDialog::HandleAccountEdit(int index)
{
	EditAccountDialog dialog(index);

	if (dialog.exec()) {
		UpdateUI();
		s_DataManager.SaveProfiles();
	}
}

void ManageAccountsDialog::HandleAccountDelete(int index)
{
	if (s_DataManager.r_CurrentProfile().DeleteAccount(index)) {
		UpdateUI();
		s_DataManager.SaveProfiles();
	}
}