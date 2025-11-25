#include "Profile.h"

#include <assert.h>

#include "Category.h"
#include "BankAccount.h"

Profile::Profile(bool populateDefault)
{
	if (populateDefault)
	{
		m_categories.push_back(Category::Internal());
		m_bankAccounts.push_back(BankAccount::Default());
	}
}

std::string Profile::GetName() const
{
	return m_name;
}

const std::vector<Category>& Profile::r_Categories() const
{
	return m_categories;
}

const std::vector<BankAccount>& Profile::r_BankAccounts() const
{
	return m_bankAccounts;
}

BankAccount& Profile::r_CurrentBankAccount()
{
	return m_bankAccounts[m_currentBankAccountIndex];
}

const BankAccount& Profile::r_ConstCurrentBankAccount() const
{
	return m_bankAccounts[m_currentBankAccountIndex];
}

int Profile::GetCurrentAccountIndex() const
{
	return m_currentBankAccountIndex;
}

void Profile::SetCurrentAccountIndex(int index)
{
	assert((index < m_bankAccounts.size()) && "Account index must be less or equal to last account index.");

	m_currentBankAccountIndex = index;
}

void Profile::Rename(std::string_view newName)
{
	m_name = newName;
}

void Profile::AddCategory(const Category& category)
{
	m_categories.push_back(category);
}

void Profile::SetCategory(size_t index, const Category& category)
{
	m_categories[index] = category;
}

void Profile::DeleteCategory(size_t index)
{
	r_CurrentBankAccount().HandleCategoryDelete(index);
	m_categories.erase(m_categories.begin() + index);
}

void Profile::AddAccount(const BankAccount& account)
{
	m_bankAccounts.push_back(account);
}

void Profile::EditAccount(size_t index, const BankAccount& account)
{
	m_bankAccounts[index].Edit(account);
}

void Profile::DeleteAccount(size_t index)
{
	m_bankAccounts.erase(m_bankAccounts.begin() + index);
}

void Profile::AddOperation(size_t accountIndex, const Operation& operation)
{
	m_bankAccounts[accountIndex].AddOperation(operation);
}

bool operator==(const Profile& p1, const Profile& p2)
{
	return p1.m_name == p2.m_name;
}