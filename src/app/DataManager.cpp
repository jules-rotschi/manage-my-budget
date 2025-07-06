#include "DataManager.h"

#include <iostream>

#include <qfile.h>
#include <qdir.h>
#include <qdebug.h>

BankAccount& DataManager::r_CurrentBankAccount()
{
	return bankAccounts[currentBankAccountIndex];
}

int DataManager::GetCurrentAccountIndex() const
{
	return currentBankAccountIndex;
}

void DataManager::SetCurrentAccountIndex(int index)
{
	currentBankAccountIndex = index;
}

void DataManager::SaveAccounts() const
{
	QDir directory;
	if (!RemoveDirectory("data/accounts")) return;
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

bool DataManager::AddCategory(const std::string& newCategory)
{
	for (const std::string& existingCategory : categories) {
		if (newCategory == existingCategory) {
			return false;
		}
	}

	categories.push_back(newCategory);
	return true;
}

bool DataManager::RenameCategory(int index, const std::string& newName)
{
	for (const std::string& existingCategoryName : categories) {
		if (newName == existingCategoryName) {
			return false;
		}
	}

	categories[index] = newName;
	return true;
}

bool DataManager::DeleteCategory(int index)
{
	if (categories.size() == 1) {
		return false;
	}

	if (categories[index] == "Opération interne") {
		return false;
	}

	for (Operation& operation : r_CurrentBankAccount().operations) {
		if (operation.categoryIndex == index) {
			return false;
		}

		if (operation.categoryIndex > index) {
			operation.categoryIndex--;
		}
	}

	categories.erase(categories.begin() + index);
	return true;
}

bool DataManager::AddAccount(const BankAccount& account)
{
	if (ToFileName(account.name).empty()) {
		return false;
	}

	for (const BankAccount& existingAccount : bankAccounts) {
		if (ToFileName(existingAccount.name) == ToFileName(account.name)) {
			return false;
		}
	}

	bankAccounts.push_back(account);
	return true;
}

bool DataManager::EditAccount(int index, const BankAccount& account, const std::string& oldAccountName)
{
	if (ToFileName(account.name).empty()) {
		return false;
	}

	for (int i = 0; i < bankAccounts.size(); i++) {
		const BankAccount& existingAccount = bankAccounts[i];

		bool isNameEdited = account.name != oldAccountName;

		if (isNameEdited && ToFileName(existingAccount.name) == ToFileName(account.name)) {
			return false;
		}
	}

	bankAccounts[index].Edit(account);
	return true;
}

bool DataManager::DeleteAccount(int index) {
	if (bankAccounts.size() == 1) {
		return false;
	}

	if (currentBankAccountIndex > index) {
		currentBankAccountIndex--;
	}

	if (currentBankAccountIndex == index) {
		SetCurrentAccountIndex(0);
	}

	bankAccounts.erase(bankAccounts.begin() + index);
	return true;
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

bool DataManager::RemoveDirectory(const std::string& name) const
{
	bool isSuccessful = false;
	QDir directory;

	if (!directory.exists(QString::fromStdString(name))) return false;

	QDir directoryToRemove(QString::fromStdString(name));

	for (const QFileInfo& fileInfo : directoryToRemove.entryInfoList(
		QDir::NoDotAndDotDot
		| QDir::System
		| QDir::Hidden
		| QDir::AllDirs
		| QDir::Files,
		QDir::DirsFirst)
	) {
		if (fileInfo.isDir()) isSuccessful = RemoveDirectory(name + "/" + fileInfo.fileName().toStdString());
		else isSuccessful = QFile::remove(QString::fromStdString(name) + "/" + fileInfo.fileName());

		if (!isSuccessful) return isSuccessful;
	}

	directory.rmdir(QString::fromStdString(name));

	return isSuccessful;
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