#pragma once

#include <qwidget.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qvalidator.h>
#include <qpushbutton.h>

#include "core/Operation.h"

class AddOperationForm : public QWidget
{
	Q_OBJECT

public:
	AddOperationForm(QWidget* parent = nullptr);
	~AddOperationForm();

private:
	QFormLayout* m_mainLayout;
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
	QPushButton* m_addButton;
	QPushButton* m_cancelButton;

	void Initialize();

	void HandleAddButton();
	void HandleCancelButton();

signals:
	void OperationAdd(const Operation& operation);
};