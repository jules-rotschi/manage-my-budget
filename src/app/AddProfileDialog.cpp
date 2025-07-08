#include "AddProfileDialog.h"

#include <qlineedit.h>
#include <qpushbutton.h>

#include "app/DataManager.h"

AddProfileDialog::AddProfileDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle("Nouveau profil");

	m_nameLabel = new QLabel("Nom du profil");
	m_nameLineEdit = new QLineEdit();

	m_addButton = new QPushButton("Ajouter le profil");
	m_cancelButton = new QPushButton("Annuler");

	m_addButton->setDefault(true);

	connect(m_addButton, &QPushButton::released, this, &AddProfileDialog::HandleConfirm);
	connect(m_cancelButton, &QPushButton::released, this, &AddProfileDialog::reject);

	m_formLayout = new QFormLayout();
	m_formLayout->addRow(m_nameLabel, m_nameLineEdit);
	m_formLayout->addRow(m_addButton, m_cancelButton);

	setLayout(m_formLayout);
}

void AddProfileDialog::HandleConfirm()
{
	Profile newProfile(true, true);
	newProfile.name = m_nameLineEdit->text().toStdString();

	if (!s_DataManager.AddProfile(newProfile)) {
		return;
	}

	accept();
}