#pragma once

#include <qdialog.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qcombobox.h>

class AddProfileDialog : public QDialog
{
	Q_OBJECT

public:
	AddProfileDialog(QWidget* parent = nullptr);

private:
	QFormLayout* m_formLayout;
	QLabel* m_nameLabel;
	QLineEdit* m_nameLineEdit;
	QPushButton* m_addButton;
	QPushButton* m_cancelButton;

private slots:
	void HandleConfirm();
};