#pragma once

#include <string>

#include "core/Amount.h"

class Operation
{
public:
	int id = 0;
	int year = 2025;
	int month = 1;
	Amount amount = 0;
	int categoryIndex = 0;
	std::string description;

	Operation() = default;

	void Edit(const Operation& operation);
};