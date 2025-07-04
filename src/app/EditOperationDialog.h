#pragma once

#include <qdialog.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qpushbutton.h>

#include "core/Operation.h"

class EditOperationDialog : public QDialog
{
	Q_OBJECT

public:
	Operation operation;

	EditOperationDialog(const Operation& operation, QWidget* parent = nullptr);
	~EditOperationDialog();

private:
	QFormLayout* m_formLayout;
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
	QPushButton* m_editButton;
	QPushButton* m_cancelButton;

	void OnAccept();
};

