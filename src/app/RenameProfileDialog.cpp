#include "RenameProfileDialog.h"

#include "DataManager.h"

RenameProfileDialog::RenameProfileDialog(int index, QWidget* parent)
	: index(index), QDialog(parent)
{
	Profile profile = s_DataManager.profiles[index];

	setWindowTitle(QString::fromStdString("Renommer le profil \"" + profile.name + "\""));

	m_nameLabel = new QLabel("Nom du profil");
	m_nameLineEdit = new QLineEdit(QString::fromStdString(profile.name));

	m_editButton = new QPushButton("Renommer");
	m_cancelButton = new QPushButton("Annuler");

	m_editButton->setDefault(true);

	connect(m_editButton, &QPushButton::released, this, &RenameProfileDialog::HandleConfirm);
	connect(m_cancelButton, &QPushButton::released, this, &RenameProfileDialog::reject);

	m_formLayout = new QFormLayout();
	m_formLayout->addRow(m_nameLabel, m_nameLineEdit);
	m_formLayout->addRow(m_editButton, m_cancelButton);

	setLayout(m_formLayout);
}

void RenameProfileDialog::HandleConfirm()
{
	Profile profile;
	profile.name = m_nameLineEdit->text().toStdString();

	std::string oldName = s_DataManager.profiles[index].name;

	if (!s_DataManager.EditProfile(index, profile, oldName)) {
		return;
	}

	accept();
}