#include "EditCategoryDialog.h"

#include "domain/Category.h"
#include "domain/Profile.h"

#include "data/StateManager.h"

#include "exceptions/ApplicationException.h"

#include "ui/hooks/ExceptionHandler.h"
#include "ui/format/StringFormatter.h"
#include "ui/format/AmountValueFormatter.h"

EditCategoryDialog::EditCategoryDialog(int categoryIndex, QWidget* parent)
	: m_categoryIndex(categoryIndex), QDialog(parent)
{
	Category categoryToEdit = StateManager::Instance().r_CurrentProfile().r_Categories()[categoryIndex];

	setWindowTitle(QString::fromStdString("Modifier la catégorie \"" + LimitLength(categoryToEdit.name, 20) + "\""));

	m_formWidget = new QWidget();

	m_nameLabel = new QLabel("Nom");
	m_nameLineEdit = new QLineEdit(QString::fromStdString(categoryToEdit.name));

	m_typeLabel = new QLabel("Type");
	m_typeCombobox = new QComboBox();
	m_typeCombobox->addItem("Crédit");
	m_typeCombobox->addItem("Débit");
	if (categoryToEdit.monthlyBudget.GetValue() < 0)
	{
		m_typeCombobox->setCurrentIndex(1);
	}
	else
	{
		m_typeCombobox->setCurrentIndex(0);
	}

	m_budgetLabel = new QLabel("Montant mensuel");
	m_budgetLineEdit = new QLineEdit(QString::fromStdString(FormatToLineEdit(categoryToEdit.monthlyBudget.GetValue(), true)));

	m_budgetAmountValidator = new QDoubleValidator();
	m_budgetAmountValidator->setDecimals(2);
	m_budgetAmountValidator->setNotation(QDoubleValidator::StandardNotation);
	m_budgetAmountValidator->setLocale(QLocale::system());
	m_budgetAmountValidator->setRange(0, 1000000);

	m_budgetLineEdit->setValidator(m_budgetAmountValidator);

	m_formLayout = new QFormLayout(m_formWidget);
	m_formLayout->addRow(m_nameLabel, m_nameLineEdit);
	m_formLayout->addRow(m_typeLabel, m_typeCombobox);
	m_formLayout->addRow(m_budgetLabel, m_budgetLineEdit);

	m_buttonsWidget = new QWidget();

	m_confirmButton = new QPushButton("Modifier");
	m_confirmButton->setDefault(true);
	connect(m_confirmButton, &QPushButton::released, this, &EditCategoryDialog::HandleConfirm);
	m_confirmButton->setMinimumWidth(100);

	m_cancelButton = new QPushButton("Annuler");
	connect(m_cancelButton, &QPushButton::released, this, &EditCategoryDialog::reject);
	m_cancelButton->setMinimumWidth(100);

	m_buttonsLayout = new QHBoxLayout(m_buttonsWidget);
	m_buttonsLayout->addWidget(m_confirmButton);
	m_buttonsLayout->addWidget(m_cancelButton);

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_formWidget);
	m_layout->addWidget(m_buttonsWidget);
}

void EditCategoryDialog::HandleConfirm()
{
	bool isBudgetOk = false;

	bool isDebit = m_typeCombobox->currentIndex();

	std::string name = m_nameLineEdit->text().toStdString();
	long absoluteBudgetAmountValue = QLocale::system().toDouble(m_budgetLineEdit->text(), &isBudgetOk) * 100;

	if (!isBudgetOk)
	{
		return;
	}

	long budgetAmountValue = isDebit ? -(long)absoluteBudgetAmountValue : absoluteBudgetAmountValue;

	try
	{
		StateManager::Instance().EditCategory(m_categoryIndex, name, budgetAmountValue);
	}
	catch (const ApplicationException& e)
	{
		HandleException(e);
		return;
	}

	accept();
}