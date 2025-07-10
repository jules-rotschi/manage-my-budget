#include "ConfirmationRequiser.h"

#include <qmessagebox.h>
#include <qpushbutton.h>

#include <iostream>

bool ConfirmAction(const std::string& message, const std::string& confirmButtonText)
{
	QMessageBox box;
	box.setText(QString::fromStdString(message));
	QPushButton* confirmButton = box.addButton(QString::fromStdString(confirmButtonText), QMessageBox::AcceptRole);
	QPushButton* cancelButton = box.addButton("Annuler", QMessageBox::RejectRole);
	box.setDefaultButton(cancelButton);

	box.exec();

	return (box.clickedButton() == confirmButton);
}