#include "app/ExceptionHandler.h"

#include <qmessagebox.h>

int HandleException(const CustomException& exception)
{
	QMessageBox box;
	box.setText(exception.what());
	return box.exec();
}