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

	Operation();
	Operation(
		int year,
		int month,
		Amount amount,
		int categoryIndex,
		const std::string& description
	);

	void Edit(const Operation& operation);
};

