#pragma once

#include <string>

class Application
{
public:
	void Init(const std::string& version);
	int Run(int argc, char* argv[]);
	void ShutDown();
};

