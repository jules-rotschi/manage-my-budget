#pragma once

#include <string>
#include <vector>

#include "core/BankAccount.h"

class Profile
{
public:
	std::string name;
	std::vector<std::string> categories;
	std::vector<BankAccount> bankAccounts;

	Profile(bool addDefaultCategory = false, bool addDefaultAccount = false);

	BankAccount& r_CurrentBankAccount();
	int GetCurrentAccountIndex() const;
	void SetCurrentAccountIndex(int index);

	bool AddCategory(const std::string& category);
	bool RenameCategory(int index, const std::string& newName);
	bool DeleteCategory(int index);

	bool AddAccount(const BankAccount& account);
	bool EditAccount(int index, const BankAccount& account, const std::string& oldAccountName);
	bool DeleteAccount(int index);

	void Rename(const std::string& newName);

private:
	int currentBankAccountIndex = 0;
};

