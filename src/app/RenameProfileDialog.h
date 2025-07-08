#pragma once

#include <qdialog.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qpushbutton.h>

class RenameProfileDialog : public QDialog
{
	Q_OBJECT

public:
	int index;

	RenameProfileDialog(int index, QWidget* parent = nullptr);

private:
	QFormLayout* m_formLayout;
	QLabel* m_nameLabel;
	QLineEdit* m_nameLineEdit;
	QPushButton* m_editButton;
	QPushButton* m_cancelButton;

private slots:
	void HandleConfirm();
};