#include "ManageCategoriesDialog.h"

#include "StateManager.h"
#include "EditCategoryDialog.h"
#include "ExceptionHandler.h"
#include "StringFormatter.h"
#include "Converter.h"

ManageCategoriesDialog::ManageCategoriesDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle("Catégories");

	setMinimumWidth(550);

	m_categoriesList = new QListWidget();

	m_newCategoryFormWidget = new QWidget();
	m_newCategoryFieldsWidget = new QWidget();

	m_nameFieldLabel = new QLabel("Nouvelle catégorie");
	m_nameLineEdit = new QLineEdit();

	m_budgetTypeLabel = new QLabel("Type");
	m_budgetTypeComboBox = new QComboBox();
	m_budgetTypeComboBox->addItem("Crédit");
	m_budgetTypeComboBox->addItem("Débit");

	m_budgetLabel = new QLabel("Montant mensuel");
	m_budgetLineEdit = new QLineEdit();

	m_budgetAmountValidator = new QDoubleValidator();
	m_budgetAmountValidator->setDecimals(2);
	m_budgetAmountValidator->setNotation(QDoubleValidator::StandardNotation);
	m_budgetAmountValidator->setLocale(QLocale::system());
	m_budgetAmountValidator->setRange(0, 1000000);

	m_budgetLineEdit->setValidator(m_budgetAmountValidator);

	m_newCategoryFieldsLayout = new QFormLayout(m_newCategoryFieldsWidget);
	m_newCategoryFieldsLayout->addRow(m_nameFieldLabel, m_nameLineEdit);
	m_newCategoryFieldsLayout->addRow(m_budgetTypeLabel, m_budgetTypeComboBox);
	m_newCategoryFieldsLayout->addRow(m_budgetLabel, m_budgetLineEdit);

	m_newCategoryAddButton = new QPushButton("Ajouter");
	connect(m_newCategoryAddButton, &QPushButton::released, this, &ManageCategoriesDialog::HandleCategoryAdd);
	m_newCategoryAddButton->setMinimumWidth(100);

	m_defaultButton = new QPushButton("Terminé");
	m_defaultButton->setDefault(true);
	connect(m_defaultButton, &QPushButton::released, this, &ManageCategoriesDialog::accept);

	m_newCategoryFormLayout = new QVBoxLayout(m_newCategoryFormWidget);
	m_newCategoryFormLayout->addWidget(m_newCategoryFieldsWidget);
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

	for (size_t i = 0; i < StateManager::Instance().r_CurrentProfile().categories.size(); i++)
	{
		Category category = StateManager::Instance().r_CurrentProfile().categories[i];

		QWidget* categoryWidget = new QWidget();

		QLabel* categoryLabel = new QLabel(QString::fromStdString(LimitLength(category.name, 20) + " : " + category.monthlyBudget.GetString()));

		QPushButton* categoryRenameButton = new QPushButton("Modifier");
		connect(
			categoryRenameButton,
			&QPushButton::released,
			[this, i]() {HandleCategoryRename(SizeToInt(i)); }
		);
		categoryRenameButton->setFixedWidth(100);

		QPushButton* categoryDeleteButton = new QPushButton("Supprimer");
		connect(
			categoryDeleteButton,
			&QPushButton::released,
			[this, i]() {HandleCategoryDelete(SizeToInt(i)); }
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
	bool isBudgetOk = false;

	bool isDebit = m_budgetTypeComboBox->currentIndex();

	std::string name = m_nameLineEdit->text().toStdString();
	long absoluteBudgetAmountValue = QLocale::system().toDouble(m_budgetLineEdit->text(), &isBudgetOk) * 100;

	if (!isBudgetOk)
	{
		return;
	}

	long budgetAmountValue = isDebit ? -(long)absoluteBudgetAmountValue : absoluteBudgetAmountValue;

	try
	{
		StateManager::Instance().AddCategory(name, budgetAmountValue);
	}
	catch (const ApplicationException& e)
	{
		HandleException(e);
		return;
	}

	m_nameLineEdit->setText("");
	m_budgetLineEdit->setText("");
	UpdateUI();
}

void ManageCategoriesDialog::HandleCategoryRename(int index)
{
	EditCategoryDialog dialog(index);

	if (dialog.exec())
	{
		UpdateUI();
	}
}

void ManageCategoriesDialog::HandleCategoryDelete(int index)
{
	try
	{
		StateManager::Instance().DeleteCategory(index);
	}
	catch (const ApplicationException& e)
	{
		HandleException(e);
		return;
	}

	UpdateUI();
}