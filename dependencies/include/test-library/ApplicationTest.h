#pragma once

#include <vector>

#include "Test.h"

class ApplicationTest
{
public:
	void Run() const;

	void AddTest(const Test& test);

private:
	std::vector<Test> m_Tests;
};

