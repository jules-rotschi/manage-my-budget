#include <vector>
#include <qapplication.h>

#include "app/DataManager.h"
#include "app/MainWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	MainWindow window(s_Version);
	window.show();
	return app.exec();
}