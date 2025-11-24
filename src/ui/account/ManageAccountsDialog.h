#pragma once

#include <vector>

#include <qdialog.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qformlayout.h>
#include <qlineedit.h>
#include <qlistwidget.h>

class ManageAccountsDialog : public QDialog
{
	Q_OBJECT

public:
	ManageAccountsDialog(QWidget* parent = nullptr);

private:
	QLayout* m_mainLayout;
	QListWidget* m_accountsList;
	QPushButton* m_addButton;
	QPushButton* m_defaultButton;

	void UpdateUI();

private slots:
	void HandleAccountAdd();
	void HandleAccountEdit(int index);
	void HandleAccountDelete(int index);
};

