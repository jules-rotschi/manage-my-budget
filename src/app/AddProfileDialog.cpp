#include "AddProfileDialog.h"

#include <qlineedit.h>
#include <qpushbutton.h>

#include "DataManager.h"
#include "ExceptionHandler.h"

AddProfileDialog::AddProfileDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle("Nouveau profil");

	m_nameLabel = new QLabel("Nom du profil");
	m_nameLineEdit = new QLineEdit();

	m_addButton = new QPushButton("Ajouter le profil");
	m_addButton->setDefault(true);
	connect(m_addButton, &QPushButton::released, this, &AddProfileDialog::HandleConfirm);

	m_cancelButton = new QPushButton("Annuler");
	connect(m_cancelButton, &QPushButton::released, this, &AddProfileDialog::reject);

	m_formLayout = new QFormLayout(this);
	m_formLayout->addRow(m_nameLabel, m_nameLineEdit);
	m_formLayout->addRow(m_addButton, m_cancelButton);
}

void AddProfileDialog::HandleConfirm()
{
	try {
		s_DataManager.AddProfile(m_nameLineEdit->text().toStdString());
	}
	catch (const CustomException& e) {
		HandleException(e);
		return;
	}

	accept();
}