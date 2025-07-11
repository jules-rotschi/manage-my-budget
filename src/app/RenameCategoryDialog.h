#pragma once

#include <qdialog.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qlineedit>
#include <qpushbutton.h>
#include <qboxlayout.h>

class RenameCategoryDialog : public QDialog
{
	Q_OBJECT

public:
	RenameCategoryDialog(int categoryIndex, QWidget* parent = nullptr);

private:
	int m_categoryIndex;

	QVBoxLayout* m_layout;

	QWidget* m_formWidget;
	QFormLayout* m_formLayout;
	QLabel* m_label;
	QLineEdit* m_lineEdit;

	QWidget* m_buttonsWidget;
	QHBoxLayout* m_buttonsLayout;
	QPushButton* m_confirmButton;
	QPushButton* m_cancelButton;

	void HandleConfirm();
};