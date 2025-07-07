#pragma once

#include <vector>

#include <qdatastream.h>

#include <core/BankAccount.h>

class DataManager
{
public:
	std::vector<std::string> categories;
	std::vector<BankAccount> bankAccounts;

	BankAccount& r_CurrentBankAccount();
	int GetCurrentAccountIndex() const;
	void SetCurrentAccountIndex(int index);

	void SaveAccounts() const;
	void SaveCategories() const;

	void LoadData();

	bool AddCategory(const std::string& category);
	bool RenameCategory(int index, const std::string& newName);
	bool DeleteCategory(int index);

	bool AddAccount(const BankAccount& account);
	bool EditAccount(int index, const BankAccount& account, const std::string& oldAccountName);
	bool DeleteAccount(int index);

	bool AddOperation(const Operation& operation);
	bool EditOperation(const Operation& operation);

private:
	int currentBankAccountIndex;

	void LoadCategories();
	void LoadAccounts();
	void LoadOperations();

	void SaveOperations() const;

	void LoadInternalCategory();
	void LoadDefaultAccount();

	std::string ToFileName(std::string str) const;

	bool RemoveDirectory(const std::string& name) const;
};

QDataStream& operator<<(QDataStream& stream, const BankAccount& account);
QDataStream& operator>>(QDataStream& stream, BankAccount& account);

QDataStream& operator<<(QDataStream& stream, const Operation& operation);
QDataStream& operator>>(QDataStream& stream, Operation& operation);

inline DataManager s_DataManager;