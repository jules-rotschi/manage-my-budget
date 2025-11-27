#include <string>

#include "application/Application.h"

constexpr const char* version = "1.0.2 (wip)";

int main(int argc, char* argv[])
{
	Application app;

	app.Init(version);
	int result = app.Run(argc, argv);
	app.ShutDown();

	return result;
}