#pragma once

#include <vector>

#include <qwidget.h>
#include <qpushbutton.h>

#include "OperationsList.h"
#include "AddOperationForm.h"

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);

private:
	QVBoxLayout* m_mainLayout;

	QPushButton* m_manageProfilesButton;

	QWidget* m_currentProfileForm;
	QFormLayout* m_currentProfileFormLayout;
	QLabel* m_currentProfileLabel;
	QComboBox* m_currentProfileComboBox;

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

	void UpdateUI(bool scrollDown = false);
	void LoadProfilesToComboBox();
	void LoadAccountsToComboBox();

private slots:
	void HandleManageProfiles();
	void HandleManageCategories();
	void HandleManageAccounts();
	void HandleCurrentProfileChange();
	void HandleCurrentAccountChange();
	void HandleOperationAdd();
	void HandleMonthlyReview() const;
	void HandleYearlyReview() const;
};