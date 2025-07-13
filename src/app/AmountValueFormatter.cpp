#include "AmountValueFormatter.h"

#include <qlocale.h>

std::string FormatToLineEdit(int value, bool abs)
{
	if (abs && value < 0) {
		value = -value;
	}

	if (value == 0) {
		return "0";
	}
	
	if (value >= 100 || value <= -100) {
		std::string valueString = std::to_string(value);
		valueString.insert(valueString.size() - 2, QLocale::system().decimalPoint().toStdString());
		return valueString;
	}

	if (value >= 10) {
		std::string valueString = std::to_string(value);
		valueString.insert(0, "0" + QLocale::system().decimalPoint().toStdString());
		return valueString;
	}

	if (value <= -10) {
		std::string valueString = std::to_string(value);
		valueString.insert(1, "0" + QLocale::system().decimalPoint().toStdString());
		return valueString;
	}

	if (value < 0) {
		std::string valueString = std::to_string(value);
		valueString.insert(1, "0" + QLocale::system().decimalPoint().toStdString() + "0");
		return valueString;
	}

	std::string valueString = std::to_string(value);
	valueString.insert(0, "0" + QLocale::system().decimalPoint().toStdString() + "0");
	return valueString;
}