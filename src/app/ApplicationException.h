#pragma once

#include <exception>
#include <string>

class ApplicationException : public std::exception
{
public:
	ApplicationException(const std::string& typeIndicator, const std::string& description);

	const char* what() const noexcept override;

private:
	std::string m_message;
};