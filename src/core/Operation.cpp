#include "Operation.h"

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
	description(description) {}

Operation::Operation(const Operation& operation)
	: id(operation.id),
	year(operation.year),
	month(operation.month),
	amount(operation.amount),
	categoryIndex(operation.categoryIndex),
	description(operation.description)
{}