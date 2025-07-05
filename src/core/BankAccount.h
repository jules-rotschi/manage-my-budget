#pragma once

#include <vector>
#include <string>

#include "core/Operation.h"

class BankAccount
{
public:
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

	Amount GetTotalAmount() const;

	void EditOperation(int id, const Operation& operation);
	void DeleteOperation(int id);
};

