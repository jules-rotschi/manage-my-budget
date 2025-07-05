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

	void SaveAccounts() const;
	void SaveOperations() const;
	void SaveCategories() const;

	void LoadAccounts();
	void LoadOperations();
	void LoadCategories();
	void LoadData();

	void AddCategory(const std::string& category);
	void RenameCategory(int index, const std::string& newName);
	void DeleteCategory(int index);

private:
	int currentBankAccountIndex;

	void LoadInternalCategory();
	void LoadDefaultAccount();

	std::string ToFileName(std::string str) const;
};

QDataStream& operator<<(QDataStream& stream, const BankAccount& account);
QDataStream& operator>>(QDataStream& stream, BankAccount& account);

QDataStream& operator<<(QDataStream& stream, const Operation& operation);
QDataStream& operator>>(QDataStream& stream, Operation& operation);

inline DataManager s_DataManager;