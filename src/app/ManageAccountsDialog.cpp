﻿#include "ManageAccountsDialog.h"

#include "StateManager.h"
#include "EditCategoryDialog.h"
#include "AddAccountDialog.h"
#include "EditAccountDialog.h"
#include "ExceptionHandler.h"
#include "ConfirmationRequiser.h"
#include "StringFormatter.h"
#include "Converter.h"

ManageAccountsDialog::ManageAccountsDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle("Comptes bancaires");

	setMinimumWidth(640);

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

void ManageAccountsDialog::UpdateUI()
{
	m_accountsList->clear();

	for (size_t i = 0; i < StateManager::Instance().r_CurrentProfile().bankAccounts.size(); i++)
	{
		const BankAccount account = StateManager::Instance().r_CurrentProfile().bankAccounts[i];

		QWidget* accountWidget = new QWidget();

		QLabel* accountLabel = new QLabel(QString::fromStdString(LimitLength(account.name, 20) + " (" + account.GetTypeString() + ") | " + account.GetTotalAmount().GetString()));
		
		QPushButton* accountEditButton = new QPushButton("Modifier");
		connect(
			accountEditButton,
			&QPushButton::released,
			[this, i]() { HandleAccountEdit(SizeToInt(i)); }
		);
		accountEditButton->setFixedWidth(100);
		
		QPushButton* accountDeleteButton = new QPushButton("Supprimer");
		connect(
			accountDeleteButton,
			&QPushButton::released,
			[this, i]() { HandleAccountDelete(SizeToInt(i)); }
		);
		accountDeleteButton->setFixedWidth(100);

		QHBoxLayout* accountLayout = new QHBoxLayout(accountWidget);
		accountLayout->addWidget(accountLabel);
		accountLayout->addWidget(accountEditButton);
		accountLayout->addWidget(accountDeleteButton);

		QListWidgetItem* accountItem = new QListWidgetItem();
		accountItem->setSizeHint(accountWidget->sizeHint());
		m_accountsList->addItem(accountItem);
		m_accountsList->setItemWidget(accountItem, accountWidget);
	}
}

void ManageAccountsDialog::HandleAccountAdd()
{
	AddAccountDialog dialog;

	if (dialog.exec())
	{
		UpdateUI();
	}
}

void ManageAccountsDialog::HandleAccountEdit(int index)
{
	EditAccountDialog dialog(index);

	if (dialog.exec())
	{
		UpdateUI();
	}
}

void ManageAccountsDialog::HandleAccountDelete(int index)
{
	if (!ConfirmAction("Voulez-vous vraiment supprimer le compte \"" + StateManager::Instance().r_CurrentProfile().bankAccounts[index].name + "\" ?", "Supprimer"))
	{
		return;
	}

	try
	{
		StateManager::Instance().DeleteAccount(index);
	}
	catch (const ApplicationException& e)
	{
		HandleException(e);
		return;
	}

	UpdateUI();
}