#include "ManageProfilesDialog.h"

#include "DataManager.h"
#include "RenameCategoryDialog.h"
#include "AddProfileDialog.h"
#include "RenameProfileDialog.h"
#include "ExceptionHandler.h"
#include "ConfirmationRequiser.h"
#include "StringFormatter.h"

ManageProfilesDialog::ManageProfilesDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle("Profils");

	setMinimumWidth(550);

	m_profilesList = new QListWidget();

	m_addButton = new QPushButton("Ajouter un profil");
	connect(m_addButton, &QPushButton::released, this, &ManageProfilesDialog::HandleProfileAdd);

	m_defaultButton = new QPushButton("Terminé");
	m_defaultButton->setDefault(true);
	connect(m_defaultButton, &QPushButton::released, this, &ManageProfilesDialog::accept);

	m_mainLayout = new QVBoxLayout(this);
	m_mainLayout->addWidget(m_profilesList);
	m_mainLayout->addWidget(m_addButton);
	m_mainLayout->addWidget(m_defaultButton);

	UpdateUI();
}

void ManageProfilesDialog::UpdateUI()
{
	m_profilesList->clear();

	for (int i = 0; i < s_DataManager.r_Profiles().size(); i++) {
		const Profile& profile = s_DataManager.r_Profiles()[i];

		QWidget* profileWidget = new QWidget();

		QLabel* profileLabel = new QLabel(QString::fromStdString(LimitLength(profile.name, 20)));

		QPushButton* profileEditButton = new QPushButton("Renommer");
		connect(
			profileEditButton,
			&QPushButton::released,
			[this, i]() {HandleProfileEdit(i); }
		);
		profileEditButton->setFixedWidth(100);

		QPushButton* profileDeleteButton = new QPushButton("Supprimer");
		connect(
			profileDeleteButton,
			&QPushButton::released,
			[this, i]() {HandleProfileDelete(i); }
		);
		profileDeleteButton->setFixedWidth(100);

		QHBoxLayout* profileLayout = new QHBoxLayout(profileWidget);
		profileLayout->addWidget(profileLabel);
		profileLayout->addWidget(profileEditButton);
		profileLayout->addWidget(profileDeleteButton);

		QListWidgetItem* profileItem = new QListWidgetItem();
		profileItem->setSizeHint(profileWidget->sizeHint());
		m_profilesList->addItem(profileItem);
		m_profilesList->setItemWidget(profileItem, profileWidget);
	}
}

void ManageProfilesDialog::HandleProfileAdd()
{
	AddProfileDialog dialog;

	if (dialog.exec()) {
		UpdateUI();
	}
}

void ManageProfilesDialog::HandleProfileEdit(int index)
{
	RenameProfileDialog dialog(index);

	if (dialog.exec()) {
		UpdateUI();
	}
}

void ManageProfilesDialog::HandleProfileDelete(int index)
{
	if (!ConfirmAction("Voulez-vous vraiment supprimer le profil \"" + s_DataManager.r_Profiles()[index].name + "\" ?", "Supprimer")) {
		return;
	}

	try {
		s_DataManager.DeleteProfile(index);
	}
	catch (const ApplicationException& e) {
		HandleException(e);
		return;
	}

	UpdateUI();
}