#include "ManageCategoriesDialog.h"

#include "DataManager.h"
#include "RenameCategoryDialog.h"
#include "ExceptionHandler.h"

ManageCategoriesDialog::ManageCategoriesDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle("Catégories");

	m_categoriesList = new QListWidget();

	m_newCategoryFormWidget = new QWidget();

	m_nameFieldLabel = new QLabel("Nouvelle catégorie");
	m_nameLineEdit = new QLineEdit();

	m_newCategoryAddButton = new QPushButton("Ajouter");
	connect(m_newCategoryAddButton, &QPushButton::released, this, &ManageCategoriesDialog::HandleCategoryAdd);

	m_defaultButton = new QPushButton("Terminé");
	m_defaultButton->setDefault(true);
	connect(m_defaultButton, &QPushButton::released, this, &ManageCategoriesDialog::accept);

	m_newCategoryFormLayout = new QFormLayout(m_newCategoryFormWidget);
	m_newCategoryFormLayout->addRow(m_nameFieldLabel, m_nameLineEdit);
	m_newCategoryFormLayout->addRow(m_newCategoryAddButton);

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

		QLabel* categoryLabel = new QLabel(QString::fromStdString(category));
		
		QPushButton* categoryRenameButton = new QPushButton("Renommer");
		connect(
			categoryRenameButton,
			&QPushButton::released,
			[this, i]() {HandleCategoryRename(i); }
		);
		
		QPushButton* categoryDeleteButton = new QPushButton("Supprimer");
		connect(
			categoryDeleteButton,
			&QPushButton::released,
			[this, i]() {HandleCategoryDelete(i); }
		);

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
	catch (const CustomException& e) {
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
	catch (const CustomException& e) {
		HandleException(e);
		return;
	}

	UpdateUI();
}