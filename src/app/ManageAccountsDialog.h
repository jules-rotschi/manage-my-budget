#pragma once

#include <vector>

#include <qdialog.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qformlayout.h>
#include <qlineedit.h>

class ManageAccountsDialog : public QDialog
{
	Q_OBJECT

public:
	ManageAccountsDialog(QWidget* parent = nullptr);
	~ManageAccountsDialog();

private:

	QLayout* m_mainLayout;

	QWidget* m_accountsWidget;
	QVBoxLayout* m_accountsLayout;
	std::vector<QWidget*> m_accountWidgets;
	std::vector<QHBoxLayout*> m_accountLayouts;
	std::vector<QLabel*> m_accountLabels;
	std::vector<QPushButton*> m_accountEditButtons;
	std::vector<QPushButton*> m_accountDeleteButtons;

	QPushButton* m_addButton;

	QPushButton* m_defaultButton;

	void ResetUI();
	void UpdateUI();

private slots:
	void HandleAccountAdd();
	void HandleAccountEdit(int index);
	void HandleAccountDelete(int index);
};

