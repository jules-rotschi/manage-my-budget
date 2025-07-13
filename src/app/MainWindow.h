#pragma once

#include <vector>

#include <qmainwindow.h>
#include <qwidget.h>
#include <qpushbutton.h>

#include "OperationsList.h"
#include "AddOperationForm.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(const std::string version, QWidget* parent = nullptr);

private:
	std::string m_version;

	QWidget* m_centralWidget;
	QVBoxLayout* m_mainLayout;

	QMenu* m_profileMenu;
	QMenu* m_reviewMenu;
	QMenu* m_dataMenu;
	QMenu* m_aboutMenu;

	QAction* m_manageProfilesAction;
	QAction* m_manageCategoriesAction;
	QAction* m_manageAccountsAction;
	QAction* m_monthlyReviewAction;
	QAction* m_yearlyReviewAction;
	QAction* m_backUpAction;
	QAction* m_backUpLoadAction;
	QAction* m_resetDataAction;
	QAction* m_showVersionAction;

	QWidget* m_currentProfileForm;
	QFormLayout* m_currentProfileFormLayout;
	QLabel* m_currentProfileLabel;
	QComboBox* m_currentProfileComboBox;

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
	void HandleBackUpLoad();
	void HandleBackUp() const;
	void HandleDataReset();
	void HandleVersionShow() const;
};