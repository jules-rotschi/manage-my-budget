#pragma once

#include <exception>
#include <string>

class CustomException : public std::exception
{
public:
	CustomException(const std::string& typeIndicator, const std::string& description);

	const char* what() const noexcept override;

private:
	std::string m_message;
};