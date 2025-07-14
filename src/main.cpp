﻿#include <vector>
#include <qapplication.h>

#include "app/StateManager.h"
#include "app/DataManager.h"
#include "app/BackUpManager.h"
#include "app/MainWindow.h"

const std::string version("1.0.1");

int main(int argc, char* argv[])
{
	StateManager::Init(version);

	QApplication app(argc, argv);

	MainWindow window;
	window.show();

	int result = app.exec();

	StateManager::ShutDown();

	return result;
}