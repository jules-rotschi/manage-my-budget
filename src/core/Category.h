#pragma once

#include <string>

#include "Amount.h"

struct Category
{
	std::string name;
	Amount monthlyBudget;

	static Category Internal();
};

bool operator==(const Category& c1, const Category& c2);
bool operator!=(const Category& c1, const Category& c2);