#include "ManageCategoriesDialog.h"

#include "app/DataManager.h"

ManageCategoriesDialog::ManageCategoriesDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle("Catégories");

	m_categoriesWidget = new QWidget();
	m_categoriesLayout = new QVBoxLayout(m_categoriesWidget);

	m_newOperationFormWidget = new QWidget();

	m_nameFieldLabel = new QLabel("Nouvelle catégorie");
	m_nameLineEdit = new QLineEdit();
	m_newOperationAddButton = new QPushButton("Ajouter");

	connect(m_newOperationAddButton, &QPushButton::released, this, &ManageCategoriesDialog::HandleCategoryAdd);

	m_newOperationFormLayout = new QFormLayout(m_newOperationFormWidget);
	m_newOperationFormLayout->addRow(m_nameFieldLabel, m_nameLineEdit);
	m_newOperationFormLayout->addRow(m_newOperationAddButton);

	m_mainLayout = new QVBoxLayout(this);
	m_mainLayout->addWidget(m_categoriesWidget);
	m_mainLayout->addWidget(m_newOperationFormWidget);

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
	m_categoryEditButtons.clear();
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
		QPushButton* categoryEditButton = new QPushButton("Modifier");
		QPushButton* categoryDeleteButton = new QPushButton("Supprimer");

		connect(
			categoryDeleteButton,
			&QPushButton::released,
			[this, i]() {HandleCategoryDelete(i); }
		);

		m_categoryWidgets.push_back(categoryWidget);
		m_categoryLayouts.push_back(categoryLayout);
		m_categoryLabels.push_back(categoryLabel);
		m_categoryEditButtons.push_back(categoryEditButton);
		m_categoryDeleteButtons.push_back(categoryDeleteButton);

		categoryLayout->addWidget(categoryLabel);
		categoryLayout->addWidget(categoryEditButton);
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

void ManageCategoriesDialog::HandleCategoryDelete(int index)
{
	s_DataManager.DeleteCategory(index);
	UpdateUI();

	s_DataManager.SaveCategories();
	s_DataManager.SaveOperations();
}