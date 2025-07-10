#include "ExceptionHandler.h"

#include <qmessagebox.h>

int HandleException(const ApplicationException& exception)
{
	QMessageBox box;
	box.setText(exception.what());
	return box.exec();
}