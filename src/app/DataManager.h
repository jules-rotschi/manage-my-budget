#pragma once

#include <vector>
#include <string>

#include <qdatastream.h>

#include <core/Operation.h>

class DataManager
{
public:
	std::vector<std::string> categories{ "Interne" };
	std::vector<Operation> operations;

	int NextId();

	void SaveData() const;
	void LoadData();

	Amount GetTotalAmount() const;

	void EditOperation(int id, const Operation& operation);
	void DeleteOperation(int id);

private:
	int m_nextId = 1;
};

QDataStream& operator<<(QDataStream& stream, const Operation& operation);
QDataStream& operator>>(QDataStream& stream, Operation& operation);

inline DataManager s_DataManager;