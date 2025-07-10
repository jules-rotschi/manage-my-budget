#include "InvalidInputException.h"

InvalidInputException::InvalidInputException(const std::string& description)
	: ApplicationException("Entrée invalide", description) { }