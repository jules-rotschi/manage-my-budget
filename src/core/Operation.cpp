#include "Operation.h"

void Operation::Edit(const Operation& operation)
{
	year = operation.year;
	month = operation.month;
	amount = operation.amount;
	categoryIndex = operation.categoryIndex;
	description = operation.description;
}