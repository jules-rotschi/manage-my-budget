#include "AddProfileDialog.h"

#include <qlineedit.h>
#include <qpushbutton.h>

#include "data/StateManager.h"
#include "exceptions/ApplicationException.h"
#include "ui/hooks/ExceptionHandler.h"

AddProfileDialog::AddProfileDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle("Nouveau profil");

	m_formWidget = new QWidget();

	m_nameLabel = new QLabel("Nom du profil");
	m_nameLineEdit = new QLineEdit();

	m_formLayout = new QFormLayout(m_formWidget);
	m_formLayout->addRow(m_nameLabel, m_nameLineEdit);

	m_buttonsWidget = new QWidget();

	m_addButton = new QPushButton("Ajouter le profil");
	m_addButton->setDefault(true);
	connect(m_addButton, &QPushButton::released, this, &AddProfileDialog::HandleConfirm);
	m_addButton->setMinimumWidth(150);

	m_cancelButton = new QPushButton("Annuler");
	connect(m_cancelButton, &QPushButton::released, this, &AddProfileDialog::reject);
	m_cancelButton->setMinimumWidth(150);

	m_buttonsLayout = new QHBoxLayout(m_buttonsWidget);
	m_buttonsLayout->addWidget(m_addButton);
	m_buttonsLayout->addWidget(m_cancelButton);

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_formWidget);
	m_layout->addWidget(m_buttonsWidget);
}

void AddProfileDialog::HandleConfirm()
{
	try
	{
		StateManager::Instance().AddProfile(m_nameLineEdit->text().toStdString());
	}
	catch (const ApplicationException& e)
	{
		HandleException(e);
		return;
	}

	accept();
}