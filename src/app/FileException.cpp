#include "FileException.h"

FileException::FileException(const std::string& description)
	: ApplicationException("Problème de fichier", description) {}