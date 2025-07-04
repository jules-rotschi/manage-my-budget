#pragma once

#include <string>

class Amount
{
public:
	int value = 0;

	Amount() = default;
	Amount(int value);

	std::string GetString() const;

	Amount& operator+=(const Amount& amount);
};

