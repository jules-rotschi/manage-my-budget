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

	int m_firstActiveMonth = 1;
	int m_lastActiveMonth = 12;

	QVBoxLayout* m_mainLayout;

	QWidget* m_yearSelectorWidget;
	QFormLayout* m_yearSelectorLayout;

	QLabel* m_yearLabel;
	QComboBox* m_yearComboBox;

	QLabel* m_monthsIntervalLabel;

	QListWidget* m_categoriesList;

	QWidget* m_totalWidget;
	QHBoxLayout* m_totalLayout;
	QLabel* m_totalTitleLabel;
	QLabel* m_totalValueLabel;

	QWidget* m_savingsWidget;
	QHBoxLayout* m_savingsLayout;
	QLabel* m_savingsTitleLabel;
	QLabel* m_savingsValueLabel;

	QPushButton* m_defaultButton;

	void UpdateUI();

private slots:
	void HandleYearSelectorChange();
};