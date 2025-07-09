#pragma once

#include <string>

#include "app/CustomException.h"

class ForbiddenActionException : public CustomException
{
public:
	ForbiddenActionException(const std::string& description);
};

