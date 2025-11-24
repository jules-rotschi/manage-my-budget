#include "ExceptionHandler.h"

#include <qmessagebox.h>

#include "exceptions/ApplicationException.h"

int HandleException(const ApplicationException& exception)
{
	QMessageBox box;
	box.setText(exception.what());
	return box.exec();
}