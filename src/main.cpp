#include <string>

#include "application/Application.h"

const std::string version("1.0.1");

int main(int argc, char* argv[])
{
	Application app;

	app.Init(version);
	int result = app.Run(argc, argv);
	app.ShutDown();

	return result;
}