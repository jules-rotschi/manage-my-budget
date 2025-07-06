#include "BankAccount.h"

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

void BankAccount::EditOperation(int id, const Operation& operation)
{
	for (Operation& oldOperation : operations) {
		if (oldOperation.id == id) {
			oldOperation.Edit(operation);
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