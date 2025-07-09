#pragma once

#include <string>

#include "app/CustomException.h"

class InvalidInputException : public CustomException
{
public:
	InvalidInputException(const std::string& description);
};