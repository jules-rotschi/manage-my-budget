#pragma once

#include <vector>

#include <qdialog.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qformlayout.h>
#include <qlistwidget.h>

class YearlyReviewDialog : public QDialog
{
	Q_OBJECT

public:
	YearlyReviewDialog(QWidget* parent = nullptr);

private:
	int m_year;

	QVBoxLayout* m_mainLayout;

	QWidget* m_yearSelectorWidget;
	QFormLayout* m_yearSelectorLayout;

	QLabel* m_yearLabel;
	QComboBox* m_yearComboBox;

	QListWidget* m_categoriesList;

	QLabel* m_totalLabel;

	QLabel* m_savingsLabel;

	QPushButton* m_defaultButton;

	void UpdateUI();

private slots:
	void HandleYearSelectorChange();
};