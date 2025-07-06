#pragma once

#include <vector>

#include <qdialog.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qformlayout.h>

class YearlyReviewDialog : public QDialog
{
	Q_OBJECT

public:
	YearlyReviewDialog(QWidget* parent = nullptr);

private:
	QVBoxLayout* m_mainLayout;

	QWidget* m_yearSelectorWidget;
	QFormLayout* m_yearSelectorLayout;
	QLabel* m_yearLabel;
	QComboBox* m_yearComboBox;

	QWidget* m_categoriesWidget;
	QVBoxLayout* m_categoriesLayout;
	std::vector<QLabel*> m_categoryReviewLabels;
	QLabel* m_totalLabel;
	QLabel* m_savingsLabel;

	QPushButton* m_defaultButton;

	int m_year;

	void UpdateUI();
	void ResetUI();

private slots:
	void HandleYearSelectorChange();
};