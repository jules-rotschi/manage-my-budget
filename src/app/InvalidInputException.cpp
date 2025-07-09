#include "InvalidInputException.h"

InvalidInputException::InvalidInputException(const std::string& description)
	: CustomException("Entrée invalide", description) { }