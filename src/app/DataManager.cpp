#include "DataManager.h"

#include <iostream>

#include <qfile.h>
#include <qdir.h>
#include <qdebug.h>

BankAccount& DataManager::r_CurrentBankAccount()
{
	return bankAccounts[currentBankAccountIndex];
}

void DataManager::SaveAccounts() const
{
	QDir directory;
	directory.rmdir("data/accounts");
	directory.mkdir("data");
	directory.mkdir("data/accounts");

	QFile file("data/accounts/accounts.dat");

	if (!file.open(QIODeviceBase::WriteOnly)) {
		return;
	}

	QDataStream stream(&file);

	for (const BankAccount& account : bankAccounts) {
		stream << account;
	}

	SaveOperations();
}

void DataManager::SaveOperations() const
{
	QDir directory;
	directory.mkdir("data");
	directory.mkdir("data/accounts");

	for (const BankAccount& account : bankAccounts) {
		directory.mkdir(QString::fromStdString("data/accounts/" + ToFileName(account.name)));

		QFile file(QString::fromStdString("data/accounts/" + ToFileName(account.name) + "/operations.dat"));

		if (!file.open(QIODeviceBase::WriteOnly)) {
			return;
		}

		QDataStream stream(&file);

		for (const Operation& operation : account.operations) {
			stream << operation;
		}
	}
}

void DataManager::SaveCategories() const
{
	QDir directory;
	directory.mkdir("data");

	QFile file("data/categories.dat");

	if (!file.open(QIODeviceBase::WriteOnly)) {
		return;
	}

	QDataStream stream(&file);

	for (const std::string& category : categories) {
		stream << QString::fromStdString(category);
	}
}

void DataManager::LoadAccounts()
{
	QFile file("data/accounts/accounts.dat");

	if (!file.open(QIODeviceBase::ReadOnly)) {
		LoadDefaultAccount();
		return;
	}

	QDataStream stream(&file);

	while (!file.atEnd()) {
		BankAccount account;
		stream >> account;
		bankAccounts.push_back(account);
	}

	if (bankAccounts.empty()) {
		LoadDefaultAccount();
	}
}

void DataManager::LoadOperations()
{
	for (BankAccount& account : bankAccounts) {
		QFile file(QString::fromStdString("data/accounts/" + ToFileName(account.name) + "/operations.dat"));

		if (!file.open(QIODeviceBase::ReadOnly)) {
			return;
		}

		QDataStream stream(&file);

		while (!file.atEnd()) {
			Operation operation;
			stream >> operation;
			account.operations.push_back(operation);
		}
	}
}

void DataManager::LoadCategories()
{
	QFile file("data/categories.dat");

	if (!file.open(QIODeviceBase::ReadOnly)) {
		LoadInternalCategory();
		return;
	}

	QDataStream dataStream(&file);

	while (!file.atEnd()) {
		QString category;
		dataStream >> category;
		categories.push_back(category.toStdString());
	}

	if (categories.empty()) {
		LoadInternalCategory();
	}
}

void DataManager::LoadData()
{
	LoadCategories();
	LoadAccounts();
	LoadOperations();
}

void DataManager::AddCategory(const std::string& newCategory)
{
	for (const std::string& existingCategory : categories) {
		if (newCategory == existingCategory) {
			return;
		}
	}

	categories.push_back(newCategory);
}

void DataManager::RenameCategory(int index, const std::string& newName)
{
	categories[index] = newName;
}

void DataManager::DeleteCategory(int index)
{
	if (categories.size() == 1) {
		return;
	}

	if (categories[index] == "Opération interne") {
		return;
	}

	for (Operation& operation : r_CurrentBankAccount().operations) {
		if (operation.categoryIndex == index) {
			return;
		}

		if (operation.categoryIndex > index) {
			operation.categoryIndex--;
		}
	}

	categories.erase(categories.begin() + index);
}

void DataManager::LoadInternalCategory()
{
	categories.emplace_back("Opération interne");
}

void DataManager::LoadDefaultAccount()
{
	BankAccount defaultAccount;
	defaultAccount.name = "Mon premier compte";
	bankAccounts.push_back(defaultAccount);
}

std::string DataManager::ToFileName(std::string str) const
{
	std::transform(str.begin(), str.end(), str.begin(), tolower);

	std::replace(str.begin(), str.end(), ' ', '-');

	for (int i = 0; i < str.size(); i++) {
		char& c = str[i];

		if ((c < 97 || c > 122) && c != '-') {
			str.erase(i, 1);
			i--;
		}
	}

	return str;
}

QDataStream& operator<<(QDataStream& stream, const BankAccount& account)
{
	stream
		<< QString::fromStdString(account.name)
		<< account.type
		<< account.initialAmount.value
		<< account.nextId;

	return stream;
}

QDataStream& operator>>(QDataStream& stream, BankAccount& account)
{
	QString name;

	stream
		>> name
		>> account.type
		>> account.initialAmount.value
		>> account.nextId;

	account.name = name.toStdString();

	return stream;
}

QDataStream& operator<<(QDataStream& stream, const Operation& operation)
{
	stream
		<< operation.id
		<< operation.year
		<< operation.month
		<< operation.amount.value
		<< operation.categoryIndex
		<< QString::fromStdString(operation.description);

	return stream;
}

QDataStream& operator>>(QDataStream& stream, Operation& operation)
{
	QString description;

	stream
		>> operation.id
		>> operation.year
		>> operation.month
		>> operation.amount.value
		>> operation.categoryIndex
		>> description;

	operation.description = description.toStdString();

	return stream;
}