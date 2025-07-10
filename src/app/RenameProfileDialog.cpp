#include "RenameProfileDialog.h"

#include "DataManager.h"
#include "ExceptionHandler.h"

RenameProfileDialog::RenameProfileDialog(int index, QWidget* parent)
	: m_index(index), QDialog(parent)
{
	const Profile& profile = s_DataManager.r_Profiles()[index];

	setWindowTitle(QString::fromStdString("Renommer le profil \"" + profile.name + "\""));

	m_nameLabel = new QLabel("Nom du profil");
	m_nameLineEdit = new QLineEdit(QString::fromStdString(profile.name));

	m_editButton = new QPushButton("Renommer");
	m_editButton->setDefault(true);
	connect(m_editButton, &QPushButton::released, this, &RenameProfileDialog::HandleConfirm);
	
	m_cancelButton = new QPushButton("Annuler");
	connect(m_cancelButton, &QPushButton::released, this, &RenameProfileDialog::reject);

	m_formLayout = new QFormLayout(this);
	m_formLayout->addRow(m_nameLabel, m_nameLineEdit);
	m_formLayout->addRow(m_editButton, m_cancelButton);
}

void RenameProfileDialog::HandleConfirm()
{
	std::string newName = m_nameLineEdit->text().toStdString();

	try {
		s_DataManager.RenameProfile(m_index, newName);
	}
	catch (const ApplicationException& e) {
		HandleException(e);
		return;
	}

	accept();
}