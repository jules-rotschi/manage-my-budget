#include "ManageCategoriesDialog.h"

#include "app/DataManager.h"
#include "app/RenameCategoryDialog.h"

ManageCategoriesDialog::ManageCategoriesDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle("Catégories");

	m_categoriesWidget = new QWidget();
	m_categoriesLayout = new QVBoxLayout(m_categoriesWidget);

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
	m_mainLayout->addWidget(m_categoriesWidget);
	m_mainLayout->addWidget(m_newCategoryFormWidget);
	m_mainLayout->addWidget(m_defaultButton);

	UpdateUI();
}

void ManageCategoriesDialog::ResetUI()
{
	for (QWidget* widget : m_categoryWidgets) {
		delete widget;
	}

	m_categoryWidgets.clear();
	m_categoryLayouts.clear();
	m_categoryLabels.clear();
	m_categoryRenameButtons.clear();
	m_categoryDeleteButtons.clear();
}

void ManageCategoriesDialog::UpdateUI()
{
	ResetUI();

	for (int i = 0; i < s_DataManager.categories.size(); i++) {
		std::string category = s_DataManager.categories[i];

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

		m_categoryWidgets.push_back(categoryWidget);
		m_categoryLayouts.push_back(categoryLayout);
		m_categoryLabels.push_back(categoryLabel);
		m_categoryRenameButtons.push_back(categoryRenameButton);
		m_categoryDeleteButtons.push_back(categoryDeleteButton);

		categoryLayout->addWidget(categoryLabel);
		categoryLayout->addWidget(categoryRenameButton);
		categoryLayout->addWidget(categoryDeleteButton);

		m_categoriesLayout->addWidget(categoryWidget);
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