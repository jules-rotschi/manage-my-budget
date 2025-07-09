#pragma once

#include <string>

class Amount
{
public:
	Amount() = default;
	Amount(int value);
	
	int GetValue() const;

	std::string GetString() const;

	Amount& operator+=(const Amount& amount);

private:
	int m_value = 0;
};