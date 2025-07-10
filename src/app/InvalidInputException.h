#pragma once

#include <string>

#include "ApplicationException.h"

class InvalidInputException : public ApplicationException
{
public:
	InvalidInputException(const std::string& description);
};