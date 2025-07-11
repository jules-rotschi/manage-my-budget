#pragma once

#include <qdialog.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qpushbutton.h>

class EditAccountDialog : public QDialog
{
	Q_OBJECT

public:
	EditAccountDialog(int index, QWidget* parent = nullptr);

private:
	int m_index;

	QVBoxLayout* m_layout;

	QWidget* m_formWidget;
	QFormLayout* m_formLayout;

	QLabel* m_nameLabel;
	QLineEdit* m_nameLineEdit;

	QLabel* m_initialAmountLabel;
	QLineEdit* m_initialAmountLineEdit;
	QDoubleValidator* m_initialAmountValidator;

	QLabel* m_typeLabel;
	QComboBox* m_typeCombobox;

	QWidget* m_buttonsWidget;
	QHBoxLayout* m_buttonsLayout;

	QPushButton* m_editButton;
	QPushButton* m_cancelButton;

private slots:
	void HandleConfirm();
};