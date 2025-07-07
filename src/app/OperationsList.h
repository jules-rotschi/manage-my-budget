#pragma once

#include <qwidget.h>
#include <qlabel.h>
#include <qboxlayout.h>
#include <qpushbutton.h>
#include <qlistwidget.h>
#include <qformlayout.h>
#include <qcombobox.h>

#include <core/Operation.h>

class OperationsList : public QWidget
{
	Q_OBJECT

public:
	OperationsList(QWidget* parent = nullptr);
	~OperationsList();

	void UpdateUI();

private:
	int m_displayOperationsSinceMonths = 2;

	QVBoxLayout* m_mainLayout;

	QWidget* m_displaySinceWidget;
	QFormLayout* m_displaySinceLayout;
	QLabel* m_displaySinceLabel;
	QComboBox* m_displaySinceComboBox;

	QLabel* m_totalLabel;

	QListWidget* m_operationsList;
	std::vector<QListWidgetItem*> m_operationItems;
	std::vector<QWidget*> m_operationWidgets;
	std::vector<QHBoxLayout*> m_operationLayouts;
	std::vector<QLabel*> m_operationLabels;
	std::vector<QPushButton*> m_editOperationButtons;
	std::vector<QPushButton*> m_deleteOperationButtons;

	void ResetUI();

	bool IsOperationInDisplayableMonth(const Operation& operation, const QDate& currentDate) const;

private slots:
	void HandleOperationEdit(const Operation& operation);
	void HandleOperationDelete(int id);

	void HandleDisplayFromChange();
};

