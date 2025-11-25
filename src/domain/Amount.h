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
	Amount& operator-=(const Amount& amount);
	Amount& operator*=(int coefficient);

	friend Amount operator+(Amount a1, const Amount& a2);
	friend Amount operator-(Amount a1, const Amount& a2);
	friend Amount operator*(Amount a, int coefficient);
	friend Amount operator-(const Amount& amount);

private:
	long m_value = 0;
};