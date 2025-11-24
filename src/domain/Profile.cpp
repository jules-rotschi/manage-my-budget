#include "Profile.h"

#include <assert.h>

#include "Category.h"
#include "BankAccount.h"

Profile::Profile(bool populateDefault)
{
	if (populateDefault)
	{
		categories.push_back(Category::Internal());
		bankAccounts.push_back(BankAccount::Default());
	}
}

BankAccount& Profile::r_CurrentBankAccount()
{
	return bankAccounts[m_currentBankAccountIndex];
}

const BankAccount& Profile::r_ConstCurrentBankAccount() const
{
	return bankAccounts[m_currentBankAccountIndex];
}

int Profile::GetCurrentAccountIndex() const
{
	return m_currentBankAccountIndex;
}

void Profile::SetCurrentAccountIndex(int index)
{
	assert((index < bankAccounts.size()) && "Account index must be less or equal to last account index.");

	m_currentBankAccountIndex = index;
}

void Profile::Rename(const std::string& newName)
{
	name = newName;
}

bool operator==(const Profile& p1, const Profile& p2)
{
	return p1.name == p2.name;
}

bool operator!=(const Profile& p1, const Profile& p2)
{
	return !(p1 == p2);
}