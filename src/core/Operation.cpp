#include "Operation.h"

#include "app/DataManager.h"

Operation::Operation()
{
	id = dataManager.nextId;
	dataManager.nextId++;
}

Operation::Operation(
	int year,
	int month,
	Amount amount,
	int categoryIndex,
	const std::string& description
)
	: year(year),
	month(month),
	amount(amount),
	categoryIndex(categoryIndex),
	description(description)
{
	id = dataManager.nextId;
	dataManager.nextId++;
}

void Operation::Edit(const Operation& operation)
{
	year = operation.year;
	month = operation.month;
	categoryIndex = operation.categoryIndex;
	amount = operation.amount;
	description = operation.description;
}