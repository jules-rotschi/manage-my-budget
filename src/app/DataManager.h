#pragma once

#include <vector>

#include <qdatastream.h>

#include <core/BankAccount.h>

class DataManager
{
public:
	std::vector<std::string> categories;
	BankAccount bankAccount;

	void SaveOperations() const;
	void SaveCategories() const;

	void LoadOperations();
	void LoadCategories();
	void LoadData();

	void AddCategory(const std::string& category);
	void RenameCategory(int index, const std::string& newName);
	void DeleteCategory(int index);

private:
	void LoadDefaultCategory();
};

QDataStream& operator<<(QDataStream& stream, const Operation& operation);
QDataStream& operator>>(QDataStream& stream, Operation& operation);

inline DataManager s_DataManager;