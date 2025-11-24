#pragma once

#include <qwidget.h>
#include <qlabel.h>
#include <qboxlayout.h>
#include <qpushbutton.h>
#include <qlistwidget.h>
#include <qformlayout.h>
#include <qcombobox.h>

class OperationsList : public QWidget
{
	Q_OBJECT

public:
	OperationsList(QWidget* parent = nullptr);

	void UpdateUI(bool scrollDown = false);

private:
	int m_displayOperationsSinceMonths = 2;

	QVBoxLayout* m_mainLayout;

	QWidget* m_displaySinceWidget;
	QFormLayout* m_displaySinceLayout;
	QLabel* m_displaySinceLabel;
	QComboBox* m_displaySinceComboBox;

	QLabel* m_totalLabel;

	QListWidget* m_operationsList;

	bool IsOperationInDisplayableMonth(int operationYear, int operationMonth, const QDate& currentDate) const;

private slots:
	void HandleOperationEdit(int id);
	void HandleOperationDelete(int id);

	void HandleDisplayFromChange();
};

