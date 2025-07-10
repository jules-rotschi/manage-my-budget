#pragma once

#include <string>

#include "ApplicationException.h"

class ForbiddenActionException : public ApplicationException
{
public:
	ForbiddenActionException(const std::string& description);
};

