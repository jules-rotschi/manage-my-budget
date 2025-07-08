#include "Profile.h"

#include "app/DataManager.h"

Profile::Profile(bool addDefaultCategory, bool addDefaultAccount)
{
	if (addDefaultCategory) {
		categories.push_back("Opération interne");
	}

	if (addDefaultAccount) {
		bankAccounts.push_back(BankAccount::Default());
	}
}

BankAccount& Profile::r_CurrentBankAccount()
{
	return bankAccounts[currentBankAccountIndex];
}

int Profile::GetCurrentAccountIndex() const
{
	return currentBankAccountIndex;
}

void Profile::SetCurrentAccountIndex(int index)
{
	currentBankAccountIndex = index;
}

bool Profile::AddCategory(const std::string& category)
{
	if (category.empty()) {
		return false;
	}

	for (const std::string& existingCategory : categories) {
		if (category == existingCategory) {
			return false;
		}
	}

	categories.push_back(category);
	return true;
}

bool Profile::RenameCategory(int index, const std::string& newName)
{
	if (categories[index] == "Opération interne") {
		return false;
	}
	
	if (newName.empty()) {
		return false;
	}

	for (const std::string& existingCategoryName : categories) {
		if (newName == existingCategoryName) {
			return false;
		}
	}

	categories[index] = newName;
	return true;
}

bool Profile::DeleteCategory(int index)
{
	if (categories[index] == "Opération interne") {
		return false;
	}

	if (categories.size() == 1) {
		return false;
	}

	for (Operation& operation : r_CurrentBankAccount().operations) {
		if (operation.categoryIndex == index) {
			return false;
		}

		if (operation.categoryIndex > index) {
			operation.categoryIndex--;
		}
	}

	categories.erase(categories.begin() + index);
	return true;
}

bool Profile::AddAccount(const BankAccount& account)
{
	if (s_DataManager.ToFileName(account.name).empty()) {
		return false;
	}

	for (const BankAccount& existingAccount : bankAccounts) {
		if (s_DataManager.ToFileName(existingAccount.name) == s_DataManager.ToFileName(account.name)) {
			return false;
		}
	}

	bankAccounts.push_back(account);
	return true;
}

bool Profile::EditAccount(int index, const BankAccount& account, const std::string& oldAccountName)
{
	if (s_DataManager.ToFileName(account.name).empty()) {
		return false;
	}

	for (int i = 0; i < bankAccounts.size(); i++) {
		const BankAccount& existingAccount = bankAccounts[i];

		bool isNameEdited = account.name != oldAccountName;

		if (isNameEdited && s_DataManager.ToFileName(existingAccount.name) == s_DataManager.ToFileName(account.name)) {
			return false;
		}
	}

	bankAccounts[index].Edit(account);
	return true;
}

bool Profile::DeleteAccount(int index)
{
	if (bankAccounts.size() == 1) {
		return false;
	}

	if (currentBankAccountIndex > index) {
		currentBankAccountIndex--;
	}

	if (currentBankAccountIndex == index) {
		SetCurrentAccountIndex(0);
	}

	bankAccounts.erase(bankAccounts.begin() + index);
	return true;
}

void Profile::Rename(const std::string& newName)
{
	name = newName;
}