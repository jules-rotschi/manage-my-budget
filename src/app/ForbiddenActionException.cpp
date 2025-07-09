#include "ForbiddenActionException.h"

ForbiddenActionException::ForbiddenActionException(const std::string& description)
	: CustomException("Action impossible", description) {
}