#pragma once

#include <string>

#include "Amount.h"

struct Category
{
	std::string name;
	Amount monthlyBudget;

	static Category Internal();

	friend bool operator==(const Category& c1, const Category& c2);
};