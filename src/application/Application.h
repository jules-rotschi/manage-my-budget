#pragma once

#include <string>

class Application
{
public:
	void Init(const char* version);
	int Run(int argc, char* argv[]);
	void ShutDown();
};

