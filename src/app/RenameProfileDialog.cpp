#include "RenameProfileDialog.h"

#include "StateManager.h"
#include "ExceptionHandler.h"
#include "StringFormatter.h"

RenameProfileDialog::RenameProfileDialog(int index, QWidget* parent)
	: m_index(index), QDialog(parent)
{
	const Profile& profile = StateManager::Instance().r_Profiles()[index];

	setWindowTitle(QString::fromStdString("Renommer le profil \"" + LimitLength(profile.name, 20) + "\""));

	m_formWidget = new QWidget();

	m_nameLabel = new QLabel("Nom du profil");
	m_nameLineEdit = new QLineEdit(QString::fromStdString(profile.name));

	m_formLayout = new QFormLayout(m_formWidget);
	m_formLayout->addRow(m_nameLabel, m_nameLineEdit);

	m_buttonsWidget = new QWidget();

	m_editButton = new QPushButton("Renommer");
	m_editButton->setDefault(true);
	connect(m_editButton, &QPushButton::released, this, &RenameProfileDialog::HandleConfirm);
	m_editButton->setMinimumWidth(100);

	m_cancelButton = new QPushButton("Annuler");
	connect(m_cancelButton, &QPushButton::released, this, &RenameProfileDialog::reject);
	m_cancelButton->setMinimumWidth(100);

	m_buttonsLayout = new QHBoxLayout(m_buttonsWidget);
	m_buttonsLayout->addWidget(m_editButton);
	m_buttonsLayout->addWidget(m_cancelButton);

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_formWidget);
	m_layout->addWidget(m_buttonsWidget);
}

void RenameProfileDialog::HandleConfirm()
{
	std::string newName = m_nameLineEdit->text().toStdString();

	try
	{
		StateManager::Instance().RenameProfile(m_index, newName);
	}
	catch (const ApplicationException& e)
	{
		HandleException(e);
		return;
	}

	accept();
}