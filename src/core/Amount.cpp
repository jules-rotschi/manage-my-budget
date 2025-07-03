#include "Amount.h"

Amount::Amount(int value) : value(value) {}

std::string Amount::getString() const {
	std::string valueString = std::to_string(value);
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
	return valueString + "€";
}

Amount& Amount::operator+=(const Amount& amount) {
	value += amount.value;
	return *this;
}