#include "ManageProfilesDialog.h"

#include "app/DataManager.h"
#include "app/RenameCategoryDialog.h"
#include "app/AddProfileDialog.h"
#include "app/RenameProfileDialog.h"

ManageProfilesDialog::ManageProfilesDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle("Profils");

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

void ManageProfilesDialog::ResetUI()
{
	for (QListWidgetItem* item : m_profileItems) {
		delete item;
	}

	for (QWidget* widget : m_profileWidgets) {
		delete widget;
	}

	m_profileItems.clear();
	m_profileWidgets.clear();
}

void ManageProfilesDialog::UpdateUI()
{
	ResetUI();

	for (int i = 0; i < s_DataManager.profiles.size(); i++) {
		const Profile& profile = s_DataManager.profiles[i];

		QListWidgetItem* profileItem = new QListWidgetItem();
		QWidget* profileWidget = new QWidget();
		QHBoxLayout* profileLayout = new QHBoxLayout(profileWidget);
		QLabel* profileLabel = new QLabel(QString::fromStdString(profile.name));
		QPushButton* profileEditButton = new QPushButton("Renommer");
		QPushButton* profileDeleteButton = new QPushButton("Supprimer");

		connect(
			profileEditButton,
			&QPushButton::released,
			[this, i]() {HandleProfileEdit(i); }
		);

		connect(
			profileDeleteButton,
			&QPushButton::released,
			[this, i]() {HandleProfileDelete(i); }
		);

		m_profileItems.push_back(profileItem);
		m_profileWidgets.push_back(profileWidget);

		profileLayout->addWidget(profileLabel);
		profileLayout->addWidget(profileEditButton);
		profileLayout->addWidget(profileDeleteButton);

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
		s_DataManager.SaveProfiles();
	}
}

void ManageProfilesDialog::HandleProfileEdit(int index)
{
	RenameProfileDialog dialog(index);

	if (dialog.exec()) {
		UpdateUI();
		s_DataManager.SaveProfiles();
	}
}

void ManageProfilesDialog::HandleProfileDelete(int index)
{
	if (s_DataManager.DeleteProfile(index)) {
		UpdateUI();
		s_DataManager.SaveProfiles();
	}
}