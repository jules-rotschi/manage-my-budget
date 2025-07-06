#include "RenameCategoryDialog.h"

#include <app/DataManager.h>

RenameCategoryDialog::RenameCategoryDialog(int categoryIndex, QWidget* parent)
	: m_categoryIndex(categoryIndex), QDialog(parent)
{
	setWindowTitle(QString::fromStdString("Renommer \"" + s_DataManager.categories[categoryIndex] + "\""));

	m_label = new QLabel("Nom");
	m_lineEdit = new QLineEdit(QString::fromStdString(s_DataManager.categories[categoryIndex]));

	m_confirmButton = new QPushButton("Renommer");
	m_cancelButton = new QPushButton("Annuler");

	m_confirmButton->setDefault(true);

	connect(m_confirmButton, &QPushButton::released, this, &RenameCategoryDialog::HandleConfirm);
	connect(m_cancelButton, &QPushButton::released, this, &RenameCategoryDialog::reject);

	m_layout = new QFormLayout(this);
	m_layout->addRow(m_label, m_lineEdit);
	m_layout->addRow(m_confirmButton, m_cancelButton);
}

void RenameCategoryDialog::HandleConfirm()
{
	s_DataManager.RenameCategory(m_categoryIndex, m_lineEdit->text().toStdString());
	accept();
}