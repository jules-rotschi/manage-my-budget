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
	~ManageCategoriesDialog();

private:

	QLayout* m_mainLayout;

	QPushButton* m_defaultButton;

	QListWidget* m_categoriesList;
	std::vector<QListWidgetItem*> m_categoryItems;
	std::vector<QWidget*> m_categoryWidgets;

	QWidget* m_newCategoryFormWidget;
	QFormLayout* m_newCategoryFormLayout;
	QLabel* m_nameFieldLabel;
	QLineEdit* m_nameLineEdit;
	QPushButton* m_newCategoryAddButton;

	void ResetUI();
	void UpdateUI();

private slots:
	void HandleCategoryAdd();
	void HandleCategoryRename(int index);
	void HandleCategoryDelete(int index);
};