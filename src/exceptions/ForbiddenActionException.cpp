#include "ForbiddenActionException.h"

ForbiddenActionException::ForbiddenActionException(const std::string& description)
	: ApplicationException("Action impossible", description) {
}