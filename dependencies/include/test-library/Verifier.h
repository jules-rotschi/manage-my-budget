#pragma once

#include <iostream>
#include <string>

class Test;

class Verifier
{
public:
	void IsTrue(bool assertion, std::string_view description);

	void NewTest(const Test& test);
	void CloseTest() const;

	void PrintSummary() const;

private:
	int m_FailsCount = 0;

	const Test* m_CurrentTest = nullptr;
	bool m_HasCurrentTestFailed = false;
};

