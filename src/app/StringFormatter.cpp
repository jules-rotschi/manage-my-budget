#include "StringFormatter.h"

#include <assert.h>

std::string LimitLength(std::string str, int maxLength, bool ellipses)
{
	assert(maxLength >= 0 && "Max length must be positive.");

	if (str.size() <= maxLength) {
		return str;
	}

	int realMaxLength = maxLength;

	if (ellipses) {
		realMaxLength = maxLength - 3;
	}

	str.erase(realMaxLength, str.size() - realMaxLength);

	if (str.size() > 0) {
		char lastChar = str[str.size() - 1];

		while (lastChar <= 32 || lastChar >= 127) {
			str.pop_back();
			lastChar = str[str.size() - 1];
		}
	}

	if (ellipses) {
		str += "...";
	}

	return str;
}