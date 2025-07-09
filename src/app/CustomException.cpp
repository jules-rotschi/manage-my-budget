#include "CustomException.h"

CustomException::CustomException(const std::string& typeIndicator, const std::string& description)
	: m_message(typeIndicator + " : " + description) {
}

const char* CustomException::what() const noexcept
{
	return m_message.c_str();
}