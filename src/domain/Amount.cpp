#include "Amount.h"

Amount::Amount(long value) : m_value(value) {}

long Amount::GetValue() const
{
	return m_value;
}

std::string Amount::GetString() const
{
	bool isNegative = m_value < 0;

	long absValue = 0;

	if (!isNegative)
	{
		absValue = m_value;
	}
	else
	{
		absValue = -m_value;
	}

	std::string valueString = std::to_string(absValue);

	if (valueString.length() == 2)
	{
		valueString = "0" + valueString;
	}
	if (valueString.length() == 1)
	{
		valueString = "00" + valueString;
	}
	if (valueString.length() == 0)
	{
		valueString = "000";
	}

	valueString.insert(valueString.end() - 2, '.');

	if (isNegative)
	{
		valueString = "-" + valueString;
	}

	return valueString + "€";
}

Amount& Amount::operator+=(const Amount& amount)
{
	m_value += amount.m_value;
	return *this;
}

Amount& Amount::operator-=(const Amount& amount)
{
	m_value -= amount.m_value;
	return *this;
}

Amount& Amount::operator*=(int coefficient)
{
	m_value *= coefficient;
	return *this;
}

Amount operator+(Amount a1, const Amount& a2)
{
	a1 += a2;
	return a1;
}

Amount operator-(Amount a1, const Amount& a2)
{
	a1 -= a2;
	return a1;
}

Amount operator*(Amount a, int coefficient)
{
	a *= coefficient;
	return a;
}

Amount operator-(const Amount& amount)
{
	return Amount(-amount.GetValue());
}