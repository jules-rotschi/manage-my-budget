#pragma once

#include <vector>

#include <qdialog.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qformlayout.h>

class MonthlyReviewDialog : public QDialog
{
	Q_OBJECT

public:
	MonthlyReviewDialog(QWidget* parent = nullptr);

private:
	QVBoxLayout* m_mainLayout;

	QWidget* m_monthSelectorWidget;
	QFormLayout* m_monthSelectorLayout;
	QLabel* m_monthLabel;
	QComboBox* m_monthComboBox;
	QLabel* m_yearLabel;
	QComboBox* m_yearComboBox;

	QWidget* m_categoriesWidget;
	QVBoxLayout* m_categoriesLayout;
	std::vector<QLabel*> m_categoryReviewLabels;
	QLabel* m_totalLabel;
	QLabel* m_savingsLabel;

	QPushButton* m_defaultButton;

	int m_month;
	int m_year;

	void UpdateUI();
	void ResetUI();

private slots:
	void HandleMonthSelectorChange();
};