#include "ConfirmationRequiser.h"

#include <qmessagebox.h>
#include <qpushbutton.h>

#include <iostream>

bool ConfirmAction(const std::string& message, const std::string& confirmButtonText)
{
	QMessageBox box;
	box.setText(QString::fromStdString(message));

	QPushButton* confirmButton = box.addButton(QString::fromStdString(confirmButtonText), QMessageBox::AcceptRole);
	confirmButton->setFixedWidth(150);
	QPushButton* cancelButton = box.addButton("Annuler", QMessageBox::RejectRole);
	cancelButton->setFixedWidth(150);
	box.setDefaultButton(cancelButton);

	box.exec();

	return (box.clickedButton() == confirmButton);
}