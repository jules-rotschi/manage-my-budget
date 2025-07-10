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

	Profile(bool populateDefault = false);

	BankAccount& r_CurrentBankAccount();
	const BankAccount& r_ConstCurrentBankAccount() const;
	int GetCurrentAccountIndex() const;
	void SetCurrentAccountIndex(int index);

	void Rename(const std::string& newName);

private:
	int m_currentBankAccountIndex = 0;
};

bool operator==(const Profile& p1, const Profile& p2);
bool operator!=(const Profile& p1, const Profile& p2);