#pragma once

#include <string>

#include "Amount.h"

class Operation
{
public:
	int id = 0;
	int year = 0;
	int month = 1;
	Amount amount = 0;
	int categoryIndex = 0;
	std::string description;

	Operation() = default;

	void Edit(const Operation& operation);
};