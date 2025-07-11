#include "ManageCategoriesDialog.h"

#include "DataManager.h"
#include "RenameCategoryDialog.h"
#include "ExceptionHandler.h"
#include "StringFormatter.h"

ManageCategoriesDialog::ManageCategoriesDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle("Catégories");

	setMinimumWidth(550);

	m_categoriesList = new QListWidget();

	m_newCategoryFormWidget = new QWidget();

	m_nameFieldLabel = new QLabel("Nouvelle catégorie");
	m_nameLineEdit = new QLineEdit();

	m_newCategoryAddButton = new QPushButton("Ajouter");
	connect(m_newCategoryAddButton, &QPushButton::released, this, &ManageCategoriesDialog::HandleCategoryAdd);
	m_newCategoryAddButton->setFixedWidth(100);

	m_defaultButton = new QPushButton("Terminé");
	m_defaultButton->setDefault(true);
	connect(m_defaultButton, &QPushButton::released, this, &ManageCategoriesDialog::accept);

	m_newCategoryFormLayout = new QHBoxLayout(m_newCategoryFormWidget);
	m_newCategoryFormLayout->addWidget(m_nameFieldLabel);
	m_newCategoryFormLayout->addWidget(m_nameLineEdit);
	m_newCategoryFormLayout->addWidget(m_newCategoryAddButton);

	m_mainLayout = new QVBoxLayout(this);
	m_mainLayout->addWidget(m_categoriesList);
	m_mainLayout->addWidget(m_newCategoryFormWidget);
	m_mainLayout->addWidget(m_defaultButton);

	UpdateUI();
}

void ManageCategoriesDialog::UpdateUI()
{
	m_categoriesList->clear();

	for (int i = 0; i < s_DataManager.r_CurrentProfile().categories.size(); i++) {
		std::string category = s_DataManager.r_CurrentProfile().categories[i];

		QWidget* categoryWidget = new QWidget();

		QLabel* categoryLabel = new QLabel(QString::fromStdString(LimitLength(category, 20)));
		
		QPushButton* categoryRenameButton = new QPushButton("Renommer");
		connect(
			categoryRenameButton,
			&QPushButton::released,
			[this, i]() {HandleCategoryRename(i); }
		);
		categoryRenameButton->setFixedWidth(100);
		
		QPushButton* categoryDeleteButton = new QPushButton("Supprimer");
		connect(
			categoryDeleteButton,
			&QPushButton::released,
			[this, i]() {HandleCategoryDelete(i); }
		);
		categoryDeleteButton->setFixedWidth(100);

		QHBoxLayout* categoryLayout = new QHBoxLayout(categoryWidget);
		categoryLayout->addWidget(categoryLabel);
		categoryLayout->addWidget(categoryRenameButton);
		categoryLayout->addWidget(categoryDeleteButton);

		QListWidgetItem* categoryItem = new QListWidgetItem();
		categoryItem->setSizeHint(categoryWidget->sizeHint());
		m_categoriesList->addItem(categoryItem);
		m_categoriesList->setItemWidget(categoryItem, categoryWidget);
	}
}

void ManageCategoriesDialog::HandleCategoryAdd()
{
	try {
		s_DataManager.AddCategory(m_nameLineEdit->text().toStdString());
	}
	catch (const ApplicationException& e) {
		HandleException(e);
		return;
	}

	m_nameLineEdit->setText("");
	UpdateUI();
}

void ManageCategoriesDialog::HandleCategoryRename(int index)
{
	RenameCategoryDialog dialog(index);

	if (dialog.exec()) {
		UpdateUI();
	}
}

void ManageCategoriesDialog::HandleCategoryDelete(int index)
{
	try {
		s_DataManager.DeleteCategory(index);
	}
	catch (const ApplicationException& e) {
		HandleException(e);
		return;
	}

	UpdateUI();
}