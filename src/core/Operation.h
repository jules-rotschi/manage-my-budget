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
	int categoryIndex;
	std::string description;

	Operation() = delete;
	Operation(const Operation& operation);
	Operation(
		int year,
		int month,
		Amount amount,
		int categoryIndex,
		const std::string& description
	);
};

