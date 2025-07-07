#include "ManageCategoriesDialog.h"

#include "app/DataManager.h"
#include "app/RenameCategoryDialog.h"

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

void ManageCategoriesDialog::ResetUI()
{
	for (QListWidgetItem* item : m_categoryItems) {
		delete item;
	}

	for (QWidget* widget : m_categoryWidgets) {
		delete widget;
	}

	m_categoryItems.clear();
	m_categoryWidgets.clear();
}

void ManageCategoriesDialog::UpdateUI()
{
	ResetUI();

	for (int i = 0; i < s_DataManager.categories.size(); i++) {
		std::string category = s_DataManager.categories[i];

		QListWidgetItem* categoryItem = new QListWidgetItem();
		QWidget* categoryWidget = new QWidget();
		QHBoxLayout* categoryLayout = new QHBoxLayout(categoryWidget);
		QLabel* categoryLabel = new QLabel(QString::fromStdString(category));
		QPushButton* categoryRenameButton = new QPushButton("Renommer");
		QPushButton* categoryDeleteButton = new QPushButton("Supprimer");

		connect(
			categoryRenameButton,
			&QPushButton::released,
			[this, i]() {HandleCategoryRename(i); }
		);

		connect(
			categoryDeleteButton,
			&QPushButton::released,
			[this, i]() {HandleCategoryDelete(i); }
		);

		m_categoryItems.push_back(categoryItem);
		m_categoryWidgets.push_back(categoryWidget);

		categoryLayout->addWidget(categoryLabel);
		categoryLayout->addWidget(categoryRenameButton);
		categoryLayout->addWidget(categoryDeleteButton);

		categoryItem->setSizeHint(categoryWidget->sizeHint());
		m_categoriesList->addItem(categoryItem);
		m_categoriesList->setItemWidget(categoryItem, categoryWidget);
	}
}

ManageCategoriesDialog::~ManageCategoriesDialog() {}

void ManageCategoriesDialog::HandleCategoryAdd()
{
	s_DataManager.AddCategory(m_nameLineEdit->text().toStdString());
	UpdateUI();
	m_nameLineEdit->setText("");

	s_DataManager.SaveCategories();
}

void ManageCategoriesDialog::HandleCategoryRename(int index)
{
	RenameCategoryDialog dialog(index);
	if (dialog.exec()) {
		UpdateUI();
		s_DataManager.SaveCategories();
	}
}

void ManageCategoriesDialog::HandleCategoryDelete(int index)
{
	s_DataManager.DeleteCategory(index);
	UpdateUI();

	s_DataManager.SaveCategories();
	s_DataManager.SaveAccounts();
}