#include "DataManager.h"

#include <iostream>

#include <qfile.h>
#include <qdir.h>
#include <qdebug.h>

void DataManager::SaveData() const
{ 
	QDir dir;
	dir.mkdir("data");

	QFile operationFile("data/operations.dat");

	if (!operationFile.open(QIODeviceBase::WriteOnly)) {
		qDebug() << "Cannot open file for writing: " << operationFile.errorString();
		return;
	}

	std::cout << "Saving" << std::endl;
	QDataStream dataStream(&operationFile);

	for (const Operation& operation : operations) {
		std::cout << "writing operation" << std::endl;
		dataStream << operation;
	}
}

void DataManager::LoadData()
{
	QFile operationFile("data/operations.dat");

	if (!operationFile.open(QIODeviceBase::ReadOnly)) {
		return;
	}
	
	std::cout << "Loading" << std::endl;
	QDataStream dataStream(&operationFile);

	while (!operationFile.atEnd()) {
		Operation readOperation;
		dataStream >> readOperation;
		operations.push_back(readOperation);
	}
}

Amount DataManager::GetTotalAmount() const
{
	Amount total;

	for (const Operation& operation : operations) {
		total += operation.amount;
	}

	return total;
}

void DataManager::EditOperation(int id, const Operation& operation)
{
	for (Operation& oldOperation : operations) {
		if (oldOperation.id == id) {
			oldOperation.Edit(operation);
			return;
		}
	}
}

void DataManager::DeleteOperation(int id)
{
	for (int i = 0; i < operations.size(); i++) {
		const Operation& operation = operations[i];

		if (operation.id == id) {
			operations.erase(operations.begin() + i);
		}
	}
}

QDataStream& operator<<(QDataStream& stream, const Operation& operation)
{
	stream
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
		>> operation.year
		>> operation.month
		>> operation.amount.value
		>> operation.categoryIndex
		>> description;

	operation.description = description.toStdString();

	return stream;
}