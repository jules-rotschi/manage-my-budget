#pragma once

#include <qdialog.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qboxlayout.h>

class EditOperationDialog : public QDialog
{
	Q_OBJECT

public:
	EditOperationDialog(int id, QWidget* parent = nullptr);

private:
	int m_id;

	QVBoxLayout* m_layout;

	QWidget* m_formWidget;
	QFormLayout* m_formLayout;

	QLabel* m_typeLabel;
	QComboBox* m_typeCombobox;
	
	QLabel* m_yearLabel;
	QComboBox* m_yearCombobox;
	
	QLabel* m_monthLabel;
	QComboBox* m_monthCombobox;
	
	QLabel* m_categoryLabel;
	QComboBox* m_categoryCombobox;
	
	QLabel* m_amountLabel;
	QLineEdit* m_amountLineEdit;
	QDoubleValidator* m_amountValidator;
	
	QLabel* m_descriptionLabel;
	QLineEdit* m_descriptionLineEdit;

	QWidget* m_buttonsWidget;
	QHBoxLayout* m_buttonsLayout;
	
	QPushButton* m_editButton;
	QPushButton* m_cancelButton;

private slots:
	void HandleConfirm();
};

