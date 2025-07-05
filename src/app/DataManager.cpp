#include "DataManager.h"

#include <iostream>

#include <qfile.h>
#include <qdir.h>
#include <qdebug.h>

void DataManager::SaveOperations() const
{
	QDir dir;
	dir.mkdir("data");

	QFile operationsFile("data/operations.dat");

	if (!operationsFile.open(QIODeviceBase::WriteOnly)) {
		return;
	}

	QDataStream operationStream(&operationsFile);

	for (const Operation& operation : bankAccount.operations) {
		operationStream << operation;
	}
}

void DataManager::SaveCategories() const
{
	QDir dir;
	dir.mkdir("data");

	QFile categoriesFile("data/categories.dat");

	if (!categoriesFile.open(QIODeviceBase::WriteOnly)) {
		return;
	}

	QDataStream categoriesStream(&categoriesFile);

	for (const std::string& category : categories) {
		categoriesStream << QString::fromStdString(category);
	}
}

void DataManager::LoadOperations()
{
	QFile operationFile("data/operations.dat");

	if (!operationFile.open(QIODeviceBase::ReadOnly)) {
		return;
	}

	QDataStream dataStream(&operationFile);

	while (!operationFile.atEnd()) {
		Operation readOperation;
		dataStream >> readOperation;
		bankAccount.operations.push_back(readOperation);
		if (readOperation.id >= bankAccount.nextId) {
			bankAccount.nextId = readOperation.id + 1;
		}
	}
}

void DataManager::LoadCategories()
{
	QFile categoriesFile("data/categories.dat");

	if (!categoriesFile.open(QIODeviceBase::ReadOnly)) {
		LoadDefaultCategory();
		return;
	}

	QDataStream dataStream(&categoriesFile);

	while (!categoriesFile.atEnd()) {
		QString readCategory;
		dataStream >> readCategory;
		categories.push_back(readCategory.toStdString());
	}

	if (categories.empty()) {
		LoadDefaultCategory();
	}
}

void DataManager::LoadData()
{
	LoadCategories();
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

void DataManager::DeleteCategory(int index)
{
	if (categories.size() == 1) {
		return;
	}

	for (Operation& operation : bankAccount.operations) {
		if (operation.categoryIndex == index) {
			return;
		}

		if (operation.categoryIndex > index) {
			operation.categoryIndex--;
		}
	}

	categories.erase(categories.begin() + index);
}

void DataManager::LoadDefaultCategory()
{
	categories.emplace_back("Ma première catégorie");
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