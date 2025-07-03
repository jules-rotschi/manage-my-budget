#pragma once

#include <string>

class Amount
{
public:
	int value = 0;

	Amount() = delete;
	Amount(int value);

	std::string getString() const;

	Amount& operator+=(const Amount& amount);
};

