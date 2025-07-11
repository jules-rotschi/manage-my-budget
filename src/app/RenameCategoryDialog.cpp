#include "RenameCategoryDialog.h"

#include "DataManager.h"
#include "ExceptionHandler.h"
#include "StringFormatter.h"

RenameCategoryDialog::RenameCategoryDialog(int categoryIndex, QWidget* parent)
	: m_categoryIndex(categoryIndex), QDialog(parent)
{
	setWindowTitle(QString::fromStdString("Renommer \"" + LimitLength(s_DataManager.r_CurrentProfile().categories[categoryIndex], 20) + "\""));

	m_formWidget = new QWidget();

	m_label = new QLabel("Nom");
	m_lineEdit = new QLineEdit(QString::fromStdString(s_DataManager.r_CurrentProfile().categories[categoryIndex]));

	m_formLayout = new QFormLayout(m_formWidget);
	m_formLayout->addRow(m_label, m_lineEdit);

	m_buttonsWidget = new QWidget();

	m_confirmButton = new QPushButton("Renommer");
	m_confirmButton->setDefault(true);
	connect(m_confirmButton, &QPushButton::released, this, &RenameCategoryDialog::HandleConfirm);
	m_confirmButton->setMinimumWidth(100);

	m_cancelButton = new QPushButton("Annuler");
	connect(m_cancelButton, &QPushButton::released, this, &RenameCategoryDialog::reject);
	m_cancelButton->setMinimumWidth(100);

	m_buttonsLayout = new QHBoxLayout(m_buttonsWidget);
	m_buttonsLayout->addWidget(m_confirmButton);
	m_buttonsLayout->addWidget(m_cancelButton);

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_formWidget);
	m_layout->addWidget(m_buttonsWidget);
}

void RenameCategoryDialog::HandleConfirm()
{
	try {
		s_DataManager.RenameCategory(m_categoryIndex, m_lineEdit->text().toStdString());
	}
	catch (const ApplicationException& e) {
		HandleException(e);
		return;
	}

	accept();
}