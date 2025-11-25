#pragma once

#include <string>
#include <vector>

class BankAccount;
struct Category;
struct Operation;

class Profile
{
public:
	Profile(bool populateDefault = false);

	std::string GetName() const;

	const std::vector<Category>& r_Categories() const;
	const std::vector<BankAccount>& r_BankAccounts() const;

	BankAccount& r_CurrentBankAccount();
	const BankAccount& r_ConstCurrentBankAccount() const;

	int GetCurrentAccountIndex() const;
	void SetCurrentAccountIndex(int index);

	void Rename(std::string_view newName);

	void AddCategory(const Category& category);
	void SetCategory(size_t index, const Category& category);
	void DeleteCategory(size_t index);

	void AddAccount(const BankAccount& account);
	void EditAccount(size_t index, const BankAccount& account);
	void DeleteAccount(size_t index);

	void AddOperation(size_t accountIndex, const Operation& operation);
	//void EditOperation(size_t accountIndex, size_t operationIndex, const Operation& operation);
	//void DeleteOperation(size_t accountIndex, size_t operationIndex, const Operation& operation);

	friend bool operator==(const Profile& p1, const Profile& p2);

private:
	std::string m_name;
	std::vector<Category> m_categories;
	std::vector<BankAccount> m_bankAccounts;
	int m_currentBankAccountIndex = 0;
};