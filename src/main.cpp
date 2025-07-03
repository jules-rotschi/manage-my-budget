#include <vector>
#include <qapplication.h>
#include "app/MainWindow.h"

int main(int argc, char* argv[]) {

	std::vector<std::string> categories{ "Cat 1", "Cat 2", "Cat 3" };

	QApplication app(argc, argv);
	MainWindow window(categories);
	window.show();
	return app.exec();
}