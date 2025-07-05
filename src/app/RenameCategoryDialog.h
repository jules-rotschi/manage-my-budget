#pragma once

#include <qdialog.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qlineedit>
#include <qpushbutton.h>

class RenameCategoryDialog : public QDialog
{
	Q_OBJECT

public:
	RenameCategoryDialog(int categoryIndex, QWidget* parent = nullptr);

private:
	int m_categoryIndex;

	QFormLayout* m_layout;
	QLabel* m_label;
	QLineEdit* m_lineEdit;
	QPushButton* m_confirmButton;
	QPushButton* m_cancelButton;

	void HandleConfirm();
};

