#include "Operation.h"

void Operation::Edit(const Operation& operation)
{
	year = operation.year;
	month = operation.month;
	categoryIndex = operation.categoryIndex;
	amount = operation.amount;
	description = operation.description;
}