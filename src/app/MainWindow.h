#pragma once

#include <vector>

#include <qwidget.h>
#include <qpushbutton.h>

#include "core/Operation.h"
#include "app/OperationsList.h"
#include "app/AddOperationForm.h"

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private:
	QVBoxLayout* m_mainLayout;

	QPushButton* m_manageCategoriesButton;
	QPushButton* m_manageAccountsButton;

	QPushButton* m_monthlyReviewButton;
	QPushButton* m_yearlyReviewButton;

	QWidget* m_currentAccountForm;
	QFormLayout* m_currentAccountFormLayout;
	QLabel* m_currentAccountLabel;
	QComboBox* m_currentAccountComboBox;

	OperationsList* m_operationsList;
	AddOperationForm* m_addOperationForm;

	void UpdateUI();
	void InitializeData();
	void LoadAccountsToComboBox();

private slots:
	void HandleManageCategories();
	void HandleManageAccounts();
	void HandleMonthlyReview() const;
	void HandleYearlyReview() const;
	void HandleCurrentAccountChange();
	void HandleOperationAdd(const Operation& operation);
};