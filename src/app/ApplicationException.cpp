#include "ApplicationException.h"

ApplicationException::ApplicationException(const std::string& typeIndicator, const std::string& description)
	: m_message(typeIndicator + " : " + description) {}

const char* ApplicationException::what() const noexcept
{
	return m_message.c_str();
}