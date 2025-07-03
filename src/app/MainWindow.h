#pragma once

#include <vector>

#include <qmainwindow.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qformlayout.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qvalidator.h>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(const std::vector<std::string>& categories, QWidget* parent = nullptr);
	~MainWindow();

private:
	const std::vector<std::string>& categories;

	QWidget* m_mainWidget;
	QVBoxLayout* m_mainLayout;

	QWidget* m_operationView;
	QLabel* m_operationLabel;

	QWidget* m_newOperationForm;
	QFormLayout* m_formLayout;
	QLabel* m_yearLabel;
	QComboBox* m_yearCombobox;
	QLabel* m_monthLabel;
	QComboBox* m_monthCombobox;
	QLabel* m_amountLabel;
	QLineEdit* m_amountLineEdit;
	QDoubleValidator* m_amountValidator;
	QLabel* m_categoryLabel;
	QComboBox* m_categoryCombobox;
	QLabel* m_descriptionLabel;
	QLineEdit* m_descriptionLineEdit;
	QPushButton* m_addButton;
	QPushButton* m_cancelButton;

	void OnOperationAdd();
};

