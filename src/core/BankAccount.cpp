#include "BankAccount.h"

#include <algorithm>

const std::vector<Operation>& BankAccount::r_operations() const
{
	return m_operations;
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

	for (const Operation& operation : m_operations) {
		total += operation.amount;
	}

	return total;
}

Amount BankAccount::GetMonthlyAmount(int year, int month) const
{
	Amount amount = 0;

	for (const Operation& operation : m_operations) {

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

	for (const Operation& operation : m_operations) {
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

	for (const Operation& operation : m_operations) {
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

	for (const Operation& operation : m_operations) {
		bool isInternalOperation = operation.categoryIndex == 0;

		if (!isInternalOperation && operation.year == year && operation.categoryIndex == categoryIndex) {
			amount += operation.amount;
		}
	}

	return amount;
}

void BankAccount::AddOperation(const Operation& operation)
{
	Operation newOperation = GetNewOperation(
		operation.year,
		operation.month,
		operation.amount,
		operation.categoryIndex,
		operation.description
	);
	m_operations.push_back(newOperation);
	SortOperations();
}

void BankAccount::EditOperation(int id, const Operation& operation)
{
	for (Operation& oldOperation : m_operations) {
		if (oldOperation.id == id) {
			oldOperation.Edit(operation);
			SortOperations();
			return;
		}
	}
}

void BankAccount::DeleteOperation(int id)
{
	for (int i = 0; i < m_operations.size(); i++) {
		const Operation& operation = m_operations[i];

		if (operation.id == id) {
			m_operations.erase(m_operations.begin() + i);
			return;
		}
	}
}

void BankAccount::HandleCategoryDelete(int index)
{
	for (Operation& operation : m_operations) {
		if (operation.categoryIndex > index) {
			operation.categoryIndex--;
		}
	}
}

void BankAccount::Edit(const BankAccount& account)
{
	name = account.name;
	type = account.type;
	initialAmount = account.initialAmount;
}

BankAccount BankAccount::Default()
{
	BankAccount defaultBankAccount;
	defaultBankAccount.name = "Mon compte bancaire";
	return defaultBankAccount;
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

int BankAccount::GetNextIdAndIncrement()
{
	int id = m_nextId;
	m_nextId++;
	return id;
}

void BankAccount::SortOperations()
{
	std::sort(m_operations.begin(), m_operations.end(), [](const Operation& op1, const Operation& op2) {
		if (op1.year != op2.year) {
			return op1.year < op2.year;
		}

		if (op1.month != op2.month) {
			return op1.month < op2.month;
		}

		return op1.id < op2.id;
		});
}