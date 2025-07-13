#pragma once

#include <qdialog.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qlineedit>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qcombobox.h>

class EditCategoryDialog : public QDialog
{
	Q_OBJECT

public:
	EditCategoryDialog(int categoryIndex, QWidget* parent = nullptr);

private:
	int m_categoryIndex;

	QVBoxLayout* m_layout;

	QWidget* m_formWidget;
	QFormLayout* m_formLayout;

	QLabel* m_nameLabel;
	QLineEdit* m_nameLineEdit;

	QLabel* m_typeLabel;
	QComboBox* m_typeCombobox;

	QLabel* m_budgetLabel;
	QLineEdit* m_budgetLineEdit;

	QDoubleValidator* m_budgetAmountValidator;

	QWidget* m_buttonsWidget;
	QHBoxLayout* m_buttonsLayout;
	QPushButton* m_confirmButton;
	QPushButton* m_cancelButton;

	void HandleConfirm();
};