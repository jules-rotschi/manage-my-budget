#include "MonthString.h"

#include <array>

std::array<std::string, 12> months = {
	"Janvier",
	"Février",
	"Mars",
	"Avril",
	"Mai",
	"Juin",
	"Juillet",
	"Août",
	"Septembre",
	"Octobre",
	"Novembre",
	"Décembre",
};

std::string MonthToString(int month)
{
	if (month < 0 || month > 11) {
		return "";
	}

	return months[static_cast<std::array<std::string, 12>::size_type>(month) - 1];
}

int StringToMonth(const std::string& str)
{
	for (int i = 0; i < months.size(); i++) {
		const std::string& monthString = months[i];
		if (monthString == str) {
			return i + 1;
		}
	}

	return -1;
}