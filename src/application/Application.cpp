#include "Application.h"

#include <qapplication.h>

#include "data/StateManager.h"

#include "ui/MainWindow.h"

void Application::Init(const char* version)
{
	StateManager::Init(version);
}

int Application::Run(int argc, char* argv[])
{
	QApplication qtApp(argc, argv);

	MainWindow window;
	window.show();

	return qtApp.exec();
}

void Application::ShutDown()
{
	StateManager::ShutDown();
}