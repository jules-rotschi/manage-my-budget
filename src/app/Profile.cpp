#include "Profile.h"

#include "app/DataManager.h"
#include "app/InvalidInputException.h"
#include "app/ForbiddenActionException.h"

Profile::Profile(bool populateDefault)
{
	if (populateDefault) {
		categories.push_back("Opération interne");
		bankAccounts.push_back(BankAccount::Default());
	}
}

BankAccount& Profile::r_CurrentBankAccount()
{
	return bankAccounts[m_currentBankAccountIndex];
}

int Profile::GetCurrentAccountIndex() const
{
	return m_currentBankAccountIndex;
}

void Profile::SetCurrentAccountIndex(int index)
{
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