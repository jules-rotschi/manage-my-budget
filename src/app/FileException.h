#pragma once

#include <string>

#include "ApplicationException.h"

class FileException : public ApplicationException
{
public:
	FileException(const std::string& description);
};