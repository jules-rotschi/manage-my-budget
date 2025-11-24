#include "Converter.h"

#include <limits>
#include <stdexcept>

int SizeToInt(size_t value)
{
	if (value > std::numeric_limits<int>::max())
	{
		throw std::overflow_error("Value is larger than int type max value.");
	}

	return static_cast<int>(value);
}