#pragma once

#include <qwidget.h>
#include <qlabel.h>
#include <qboxlayout.h>
#include <qpushbutton.h>

#include <core/Operation.h>

class OperationsList : public QWidget
{
	Q_OBJECT

public:
	OperationsList(QWidget* parent = nullptr);
	~OperationsList();

	void UpdateUI();

private:
	QVBoxLayout* m_mainLayout;

	QLabel* m_totalLabel;

	QWidget* m_operationsView;
	QVBoxLayout* m_operationsLayout;
	std::vector<QWidget*> m_operationWidgets;
	std::vector<QHBoxLayout*> m_operationLayouts;
	std::vector<QLabel*> m_operationLabels;
	std::vector<QPushButton*> m_editOperationButtons;
	std::vector<QPushButton*> m_deleteOperationButtons;

	void Reset();
	void HandleOperationEdit(const Operation& operation);
	void HandleOperationDelete(int id);
};

