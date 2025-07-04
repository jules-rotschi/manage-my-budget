#pragma once

#include <vector>

#include <qmainwindow.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qformlayout.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qvalidator.h>
#include <qdialog.h>

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

	OperationsList* m_operationsList;
	AddOperationForm* m_addOperationForm;

	void Update();
	void InitializeData();

private slots:
	void OnOperationAdd(const Operation& operation);
};