#pragma once

#include <vector>
#include <string>

#include "core/Operation.h"

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
	std::vector<Operation> operations;

	int nextId = 1;

	int GetNextIdAndIncrement();

	Operation GetNewOperation();
	Operation GetNewOperation(
		int year,
		int month,
		Amount amount,
		int categoryIndex,
		const std::string& description
	);

	std::string GetTypeString() const;

	Amount GetTotalAmount() const;

	void EditOperation(int id, const Operation& operation);
	void DeleteOperation(int id);

	void Edit(const BankAccount& account);
};

