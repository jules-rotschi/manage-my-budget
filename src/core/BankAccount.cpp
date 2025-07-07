#include "BankAccount.h"

#include <algorithm>

int BankAccount::GetNextIdAndIncrement()
{
	int id = nextId;
	nextId++;
	return id;
}

Operation BankAccount::GetNewOperation()
{
	Operation operation;
	operation.id = GetNextIdAndIncrement();
	return operation;
}

Operation BankAccount::GetNewOperation(
	int year,
	int month,
	Amount amount,
	int categoryIndex,
	const std::string& description
)
{
	Operation operation = GetNewOperation();
	operation.year = year;
	operation.month = month;
	operation.amount = amount;
	operation.categoryIndex = categoryIndex;
	operation.description = description;

	return operation;
}

void BankAccount::SortOperations()
{
	std::sort(operations.begin(), operations.end(), [](const Operation& op1, const Operation& op2) {
		if (op1.year != op2.year) {
			return op1.year < op2.year;
		}

		if (op1.month != op2.month) {
			return op1.month < op2.month;
		}

		return op1.id < op2.id;
	});
}

std::string BankAccount::GetTypeString() const
{
	if (type == AccountType::SAVING) {
		return "Épargne";
	}
	else {
		return "Compte courant";
	}
}

Amount BankAccount::GetTotalAmount() const
{
	Amount total = initialAmount;

	for (const Operation& operation : operations) {
		total += operation.amount;
	}

	return total;
}

Amount BankAccount::GetMonthlyAmount(int year, int month) const
{
	Amount amount = 0;

	for (const Operation& operation : operations) {

		bool isInternalOperation = operation.categoryIndex == 0;

		if (!isInternalOperation && operation.year == year && operation.month == month) {
			amount += operation.amount;
		}
	}

	return amount;
}

Amount BankAccount::GetMonthlyAmount(int year, int month, int categoryIndex) const
{
	Amount amount = 0;

	for (const Operation& operation : operations) {
		bool isInternalOperation = operation.categoryIndex == 0;

		if (!isInternalOperation && operation.year == year && operation.month == month && operation.categoryIndex == categoryIndex) {
			amount += operation.amount;
		}
	}

	return amount;
}

Amount BankAccount::GetYearlyAmount(int year) const
{
	Amount amount = 0;

	for (const Operation& operation : operations) {
		bool isInternalOperation = operation.categoryIndex == 0;

		if (!isInternalOperation && operation.year == year) {
			amount += operation.amount;
		}
	}

	return amount;
}

Amount BankAccount::GetYearlyAmount(int year, int categoryIndex) const
{
	Amount amount = 0;

	for (const Operation& operation : operations) {
		bool isInternalOperation = operation.categoryIndex == 0;

		if (!isInternalOperation && operation.year == year && operation.categoryIndex == categoryIndex) {
			amount += operation.amount;
		}
	}

	return amount;
}

void BankAccount::AddOperation(const Operation& operation)
{
	operations.push_back(operation);
	SortOperations();
}

void BankAccount::EditOperation(int id, const Operation& operation)
{
	for (Operation& oldOperation : operations) {
		if (oldOperation.id == id) {
			oldOperation.Edit(operation);
			SortOperations();
			return;
		}
	}
}

void BankAccount::DeleteOperation(int id)
{
	for (int i = 0; i < operations.size(); i++) {
		const Operation& operation = operations[i];

		if (operation.id == id) {
			operations.erase(operations.begin() + i);
		}
	}
}

void BankAccount::Edit(const BankAccount& account)
{
	name = account.name;
	type = account.type;
	initialAmount = account.initialAmount;
}