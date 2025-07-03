#include "MainWindow.h"

#include <iostream>

#include <qstring.h>
#include <qdatetime.h>

#include "core/Operation.h"

MainWindow::MainWindow(const std::vector<std::string>& categories, QWidget* parent)
	: categories(categories), QMainWindow(parent)
{
	setWindowTitle("Manage my budget");

	m_mainWidget = new QWidget();

	m_operationView = new QWidget();

	m_operationLabel = new QLabel("Veuillez entrer une opération.", m_operationView);

	m_newOperationForm = new QWidget();

	QDate currentDate = QDate::currentDate();

	m_yearLabel = new QLabel(QString::fromUtf8("Année"));

	m_yearCombobox = new QComboBox();
	for (int i = 0; i <= 2; i++) {
		m_yearCombobox->addItem(QString::fromStdString(std::to_string(currentDate.year() - i)));
	}

	m_monthLabel = new QLabel("Mois");

	m_monthCombobox = new QComboBox();
	for (int i = 0; i < 12; i++) {
		m_monthCombobox->addItem(QString::fromStdString(std::to_string(i + 1)));
	}
	m_monthCombobox->setCurrentIndex(m_monthCombobox->findText(QString::fromStdString(std::to_string(currentDate.month()))));

	m_amountLabel = new QLabel("Montant");
	m_amountLineEdit = new QLineEdit();
	
	m_amountValidator = new QDoubleValidator();
	m_amountValidator->setDecimals(2);
	m_amountValidator->setNotation(QDoubleValidator::StandardNotation);
	m_amountValidator->setLocale(QLocale::system());
	
	m_amountLineEdit->setValidator(m_amountValidator);

	m_categoryLabel = new QLabel(QString::fromUtf8("Catégorie"));

	m_categoryCombobox = new QComboBox();
	for (std::string category : categories) {
		m_categoryCombobox->addItem(QString::fromStdString(category));
	}

	m_descriptionLabel = new QLabel("Description");

	m_descriptionLineEdit = new QLineEdit();

	m_addButton = new QPushButton(QString::fromUtf8("Ajouter l'opération"));
	m_cancelButton = new QPushButton("Annuler");

	connect(m_addButton, &QPushButton::released, this, &MainWindow::OnOperationAdd);

	m_formLayout = new QFormLayout(m_newOperationForm);
	m_formLayout->addRow(m_yearLabel, m_yearCombobox);
	m_formLayout->addRow(m_monthLabel, m_monthCombobox);
	m_formLayout->addRow(m_amountLabel, m_amountLineEdit);
	m_formLayout->addRow(m_categoryLabel, m_categoryCombobox);
	m_formLayout->addRow(m_descriptionLabel, m_descriptionLineEdit);
	m_formLayout->addWidget(m_addButton);
	m_formLayout->addWidget(m_cancelButton);

	m_mainLayout = new QVBoxLayout(m_mainWidget);
	m_mainLayout->addWidget(m_operationView);
	m_mainLayout->addWidget(m_newOperationForm);

	setCentralWidget(m_mainWidget);
}

MainWindow::~MainWindow()
{
	delete m_newOperationForm;
}

void MainWindow::OnOperationAdd()
{
	bool isYearOk = false;
	bool isMonthOk = false;
	bool isAmountOk = false;

	int year = m_yearCombobox->currentText().toInt(&isYearOk);
	int month = m_monthCombobox->currentText().toInt(&isMonthOk);
	int amount = m_amountLineEdit->text().toDouble(&isAmountOk) * 100;
	int categoryIndex = m_categoryCombobox->currentIndex();
	std::string description = m_descriptionLineEdit->text().toStdString();
	
	if (!isYearOk || !isMonthOk || !isAmountOk) {
		std::cout << isYearOk << isMonthOk << isAmountOk << std::endl;
	}

	Operation operation(
		year,
		month,
		amount,
		categoryIndex,
		description
	);

	m_operationLabel->setText(
		QString::fromStdString(std::to_string(operation.month))
		+ '/'
		+ QString::fromStdString(std::to_string(operation.year))
		+ " : "
		+ QString::fromStdString(operation.amount.getString())
		+ " | "
		+ QString::fromStdString(categories[operation.categoryIndex])
		+ " ("
		+ QString::fromStdString(operation.description)
		+ ")"
	);
}