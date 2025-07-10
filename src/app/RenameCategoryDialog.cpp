#include "RenameCategoryDialog.h"

#include "DataManager.h"
#include "ExceptionHandler.h"

RenameCategoryDialog::RenameCategoryDialog(int categoryIndex, QWidget* parent)
	: m_categoryIndex(categoryIndex), QDialog(parent)
{
	setWindowTitle(QString::fromStdString("Renommer \"" + s_DataManager.r_CurrentProfile().categories[categoryIndex] + "\""));

	m_label = new QLabel("Nom");
	m_lineEdit = new QLineEdit(QString::fromStdString(s_DataManager.r_CurrentProfile().categories[categoryIndex]));

	m_confirmButton = new QPushButton("Renommer");
	m_confirmButton->setDefault(true);
	connect(m_confirmButton, &QPushButton::released, this, &RenameCategoryDialog::HandleConfirm);

	m_cancelButton = new QPushButton("Annuler");
	connect(m_cancelButton, &QPushButton::released, this, &RenameCategoryDialog::reject);

	m_layout = new QFormLayout(this);
	m_layout->addRow(m_label, m_lineEdit);
	m_layout->addRow(m_confirmButton, m_cancelButton);
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