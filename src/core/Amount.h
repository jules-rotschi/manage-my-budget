#pragma once

#include <string>

class Amount
{
public:
	Amount() = default;
	Amount(long value);
	
	long GetValue() const;

	std::string GetString() const;

	Amount& operator+=(const Amount& amount);

private:
	long m_value = 0;
};