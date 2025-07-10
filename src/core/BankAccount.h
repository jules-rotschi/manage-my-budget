#pragma once

#include <vector>
#include <string>

#include "Operation.h"

enum class AccountType
{
	CURRENT,
	SAVING
};

class BankAccount
{
public:
	std::string name;
	AccountType type = AccountType::CURRENT;
	Amount initialAmount;

	const std::vector<Operation>& r_Operations() const;

	std::string GetTypeString() const;

	Amount GetTotalAmount() const;

	Amount GetMonthlyAmount(int year, int month) const;
	Amount GetMonthlyAmount(int year, int month, int categoryIndex) const;
	Amount GetYearlyAmount(int year) const;
	Amount GetYearlyAmount(int year, int categoryIndex) const;

	void AddOperation(const Operation& operation);
	void EditOperation(int id, const Operation& operation);
	void DeleteOperation(int id);

	void HandleCategoryDelete(int index);

	void Edit(const BankAccount& account);

	static BankAccount Default();

private:
	std::vector<Operation> m_operations;
	int m_nextId = 1;

	Operation GetNewOperation();
	Operation GetNewOperation(
		int year,
		int month,
		Amount amount,
		int categoryIndex,
		const std::string& description
	);

	int GetNextIdAndIncrement();
	
	void SortOperations();
};

bool operator==(const BankAccount& a1, const BankAccount& a2);
bool operator!=(const BankAccount& a1, const BankAccount& a2);