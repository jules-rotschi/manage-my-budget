#pragma once

#include <vector>
#include <string>

#include "Amount.h"
#include "Operation.h"

enum class AccountType
{
	CURRENT,
	SAVING
};

class BankAccount
{
public:
	const std::vector<Operation>& r_Operations() const;

	bool IsSaving() const;
	std::string GetTypeString() const;

	std::string GetName() const;
	AccountType GetType() const;
	Amount GetInitialAmount() const;

	Amount GetTotalAmount() const;

	Amount GetMonthlyAmount(int year, int month) const;
	Amount GetMonthlyAmount(int year, int month, int categoryIndex) const;
	Amount GetYearlyAmount(int year) const;
	Amount GetYearlyAmount(int year, int categoryIndex) const;

	void AddOperation(const Operation& operation);
	void EditOperation(int id, const Operation& operation);
	void DeleteOperation(int id);

	void Rename(std::string_view newName);
	void SetType(AccountType type);
	void SetInitialAmount(Amount amount);
	void Edit(const BankAccount& account);

	void HandleCategoryDelete(int index);

	static BankAccount Default();

	friend bool operator==(const BankAccount& a1, const BankAccount& a2);

private:
	Operation GetNewOperation();
	Operation GetNewOperation(
		int year,
		int month,
		Amount amount,
		int categoryIndex,
		std::string_view description
	);

	int GetNextIdAndIncrement();

	void SortOperations();

private:
	std::string m_name;
	AccountType m_type = AccountType::CURRENT;
	Amount m_initialAmount;
	std::vector<Operation> m_operations;
	int m_nextId = 1;
};