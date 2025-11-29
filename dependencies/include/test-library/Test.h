#pragma once

#include <string>
#include <functional>

#include "Verifier.h"

class Test
{
public:
	Test() = delete;
	Test(const std::string& name, const std::function<void(Verifier&)>& function);

	void Run(Verifier& verifier) const;

	std::string GetName() const;

private:
	std::string m_Name;
	std::function<void(Verifier&)> m_Function;
};