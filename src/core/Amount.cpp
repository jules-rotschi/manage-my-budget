#include "Amount.h"

Amount::Amount(long value) : m_value(value) {}

long Amount::GetValue() const
{
	return m_value;
}

std::string Amount::GetString() const
{
	long tempValue = m_value;
	bool isNegative = tempValue < 0;

	if (isNegative) {
		tempValue = -tempValue;
	}

	std::string valueString = std::to_string(tempValue);
	if (valueString.length() == 2) {
		valueString = "0" + valueString;
	}
	if (valueString.length() == 1) {
		valueString = "00" + valueString;
	}
	if (valueString.length() == 0) {
		valueString = "000";
	}
	valueString.insert(valueString.end() - 2, '.');

	if (isNegative) {
		valueString = "-" + valueString;
	}

	return valueString + "€";
}

Amount& Amount::operator+=(const Amount& amount)
{
	m_value += amount.m_value;
	return *this;
}

Amount operator*(const Amount& a1, const Amount& a2)
{
	return Amount(a1.GetValue() * a2.GetValue());
}

Amount operator-(const Amount& a1, const Amount& a2)
{
	return Amount(a1.GetValue() - a2.GetValue());
}

Amount operator-(const Amount& amount)
{
	return Amount(-amount.GetValue());
}