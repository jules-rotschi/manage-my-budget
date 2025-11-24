#pragma once

#include <vector>

#include <qdialog.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qformlayout.h>
#include <qlineedit.h>
#include <qlistwidget.h>

class ManageProfilesDialog : public QDialog
{
	Q_OBJECT

public:
	ManageProfilesDialog(QWidget* parent = nullptr);

private:
	QLayout* m_mainLayout;
	QListWidget* m_profilesList;
	QPushButton* m_addButton;
	QPushButton* m_defaultButton;

	void UpdateUI();

private slots:
	void HandleProfileAdd();
	void HandleProfileEdit(int index);
	void HandleProfileDelete(int index);
};