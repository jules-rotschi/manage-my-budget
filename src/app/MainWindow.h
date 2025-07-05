#pragma once

#include <vector>

#include <qwidget.h>
#include <qpushbutton.h>

#include "core/Operation.h"
#include "app/OperationsList.h"
#include "app/AddOperationForm.h"
#include "app/ManageCategoriesDialog.h"

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private:
	QVBoxLayout* m_mainLayout;

	QPushButton* m_manageCategoriesButton;

	OperationsList* m_operationsList;
	AddOperationForm* m_addOperationForm;

	void UpdateUI();
	void InitializeData();

private slots:
	void HandleOperationAdd(const Operation& operation);
	void HandleManageCategories();
};