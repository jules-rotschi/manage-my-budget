#pragma once

#include <vector>

#include <qdialog.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qformlayout.h>
#include <qlineedit.h>

class ManageCategoriesDialog : public QDialog
{
	Q_OBJECT

public:
	ManageCategoriesDialog(QWidget* parent = nullptr);
	~ManageCategoriesDialog();

private:

	QLayout* m_mainLayout;

	QWidget* m_categoriesWidget;
	QVBoxLayout* m_categoriesLayout;
	std::vector<QWidget*> m_categoryWidgets;
	std::vector<QHBoxLayout*> m_categoryLayouts;
	std::vector<QLabel*> m_categoryLabels;
	std::vector<QPushButton*> m_categoryEditButtons;
	std::vector<QPushButton*> m_categoryDeleteButtons;

	QWidget* m_newOperationFormWidget;
	QFormLayout* m_newOperationFormLayout;
	QLabel* m_nameFieldLabel;
	QLineEdit* m_nameLineEdit;
	QPushButton* m_newOperationAddButton;

	void ResetUI();
	void UpdateUI();

private slots:
	void HandleCategoryAdd();
	void HandleCategoryDelete(int index);
};