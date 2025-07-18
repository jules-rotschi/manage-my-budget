﻿#pragma once

#include <vector>

#include <qdialog.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qformlayout.h>
#include <qlistwidget.h>

class MonthlyReviewDialog : public QDialog
{
	Q_OBJECT

public:
	MonthlyReviewDialog(QWidget* parent = nullptr);

private:
	int m_month;
	int m_year;

	QVBoxLayout* m_mainLayout;

	QWidget* m_monthSelectorWidget;
	QFormLayout* m_monthSelectorLayout;

	QLabel* m_monthLabel;
	QComboBox* m_monthComboBox;

	QLabel* m_yearLabel;
	QComboBox* m_yearComboBox;

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
	void HandleMonthSelectorChange();
};