#pragma once

#include <vector>

#include <qdialog.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qformlayout.h>
#include <qlineedit.h>
#include <qlistwidget.h>

class ManageCategoriesDialog : public QDialog
{
	Q_OBJECT

public:
	ManageCategoriesDialog(QWidget* parent = nullptr);

private:
	QLayout* m_mainLayout;

	QListWidget* m_categoriesList;

	QWidget* m_newCategoryFormWidget;
	QHBoxLayout* m_newCategoryFormLayout;
	
	QLabel* m_nameFieldLabel;
	QLineEdit* m_nameLineEdit;
	
	QPushButton* m_newCategoryAddButton;

	QPushButton* m_defaultButton;
	
	void UpdateUI();

private slots:
	void HandleCategoryAdd();
	void HandleCategoryRename(int index);
	void HandleCategoryDelete(int index);
};