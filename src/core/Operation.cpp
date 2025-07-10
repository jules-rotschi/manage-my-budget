#include "Operation.h"

#include <assert.h>

void Operation::Edit(const Operation& operation)
{
	assert((operation.month >= 1 && operation.month <= 12) && "Operation month must be within [1:12]");

	year = operation.year;
	month = operation.month;
	amount = operation.amount;
	categoryIndex = operation.categoryIndex;
	description = operation.description;
}