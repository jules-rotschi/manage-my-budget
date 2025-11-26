#include "Data.h"

#include <iostream>
#include <assert.h>

#include <qfile.h>
#include <qdir.h>

#include "domain/Profile.h"
#include "domain/Category.h"
#include "domain/BankAccount.h"
#include "domain/Operation.h"

#include "StateManager.h"

#include "exceptions/InvalidInputException.h"
#include "exceptions/ForbiddenActionException.h"
#include "exceptions/FileException.h"

#include "files/Files.h"
#include "types/Converter.h"

const static std::string dataDirectoryName = "manage-my-budget-data";
static QDir saveInDirectory;
static QDir dataDirectory;

void InitializeData(const QDir& saveIn)
{
	saveInDirectory = saveIn;
	saveInDirectory.mkdir(QString::fromStdString(dataDirectoryName));
	dataDirectory = saveInDirectory.filePath(QString::fromStdString(dataDirectoryName));
	LoadData();
	SaveData();
}

void InitializeData()
{
	saveInDirectory.mkdir(QString::fromStdString(dataDirectoryName));
	LoadData();
	SaveData();
}

void ResetData()
{
	if (!RemoveDirectory(dataDirectory.path().toStdString()))
	{
		throw FileException("Impossible de réinitialiser les données enregistrées sur l'ordinateur.");
	}
	StateManager::Instance().ResetData();
}

void LoadData()
{
	bool isDataFromVersion1_0_0 = false;

	QFile file(dataDirectory.filePath("version.dat"));

	if (!file.open(QIODeviceBase::ReadOnly))
	{
		isDataFromVersion1_0_0 = true;
	}

	LoadProfiles(isDataFromVersion1_0_0);
}

void LoadProfiles(bool isDataFromVersion1_0_0)
{
	QFile file(dataDirectory.filePath("profiles/profiles.dat"));

	if (file.open(QIODeviceBase::ReadOnly))
	{
		QDataStream stream(&file);

		while (!file.atEnd())
		{
			Profile profile;
			stream >> profile;
			StateManager::Instance().LoadProfile(profile);
		}
	}
	else
	{
		StateManager::Instance().LoadDefaultProfile();
	}

	for (size_t i = 0; i < StateManager::Instance().r_Profiles().size(); i++)
	{
		LoadCategories(SizeToInt(i), isDataFromVersion1_0_0);
		LoadAccounts(SizeToInt(i));
	}
}

void LoadCategories(int profileIndex, bool isDataFromVersion1_0_0)
{
	const Profile& profile = StateManager::Instance().r_Profiles()[profileIndex];

	QFile file(dataDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.GetName()) + "/categories.dat")));

	if (file.open(QIODeviceBase::ReadOnly))
	{
		QDataStream stream(&file);

		while (!file.atEnd())
		{
			Category category;

			if (isDataFromVersion1_0_0)
			{
				QString categoryName;
				stream >> categoryName;
				category.name = categoryName.toStdString();
				category.monthlyBudget = 0;
			}
			else
			{
				stream >> category;
			}

			StateManager::Instance().LoadCategory(category, profileIndex);
		}
	}
	else
	{
		StateManager::Instance().LoadInternalCategory(profileIndex);
	}
}

void LoadAccounts(int profileIndex)
{
	const Profile& profile = StateManager::Instance().r_Profiles()[profileIndex];

	QFile file(dataDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.GetName()) + "/accounts/accounts.dat")));

	if (file.open(QIODeviceBase::ReadOnly))
	{
		QDataStream stream(&file);

		while (!file.atEnd())
		{
			BankAccount account;
			stream >> account;
			StateManager::Instance().LoadAccount(account, profileIndex);
		}
	}
	else
	{
		StateManager::Instance().LoadDefaultAccount(profileIndex);
	}

	for (size_t i = 0; i < StateManager::Instance().r_Profiles()[profileIndex].r_BankAccounts().size(); i++)
	{
		LoadOperations(profileIndex, SizeToInt(i));
	}
}

void LoadOperations(int profileIndex, int accountIndex)
{
	const Profile& profile = StateManager::Instance().r_Profiles()[profileIndex];
	const BankAccount& account = StateManager::Instance().r_Profiles()[profileIndex].r_BankAccounts()[accountIndex];

	QFile file(dataDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.GetName()) + "/accounts/" + ToFileName(account.GetName()) + "/operations.dat")));

	file.open(QIODeviceBase::ReadOnly);

	QDataStream stream(&file);

	while (!file.atEnd())
	{
		Operation operation;
		stream >> operation;
		StateManager::Instance().LoadOperation(operation, profileIndex, accountIndex);
	}
}

void SaveData()
{
	QFile file(dataDirectory.filePath("version.dat"));

	if (!file.open(QIODeviceBase::WriteOnly))
	{
		throw FileException("Impossible d'enregistrer les données.");
	}

	QDataStream stream(&file);

	stream << QString::fromStdString(StateManager::Instance().appVersion);

	SaveProfiles();
}

void SaveProfiles()
{
	RemoveDirectory(dataDirectory.filePath("profiles").toStdString());
	dataDirectory.mkdir("profiles");

	QFile file(dataDirectory.filePath("profiles/profiles.dat"));

	if (!file.open(QIODeviceBase::WriteOnly))
	{
		throw FileException("Impossible d'enregistrer les données.");
	}

	QDataStream stream(&file);

	for (const Profile& profile : StateManager::Instance().r_Profiles())
	{
		stream << profile;
		dataDirectory.mkdir(QString::fromStdString("profiles/" + ToFileName(profile.GetName())));
		SaveCategories(profile);
		SaveAccounts(profile);
	}
}

void SaveCategories(const Profile& profile)
{
	QFile file(dataDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.GetName()) + "/categories.dat")));

	if (!file.open(QIODeviceBase::WriteOnly))
	{
		throw FileException("Impossible d'enregistrer les données.");
	}

	QDataStream stream(&file);

	for (const Category& category : profile.r_Categories())
	{
		stream << category;
	}
}

void SaveAccounts(const Profile& profile)
{
	RemoveDirectory(dataDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.GetName()) + "/accounts")).toStdString());
	dataDirectory.mkdir(QString::fromStdString("profiles/" + ToFileName(profile.GetName()) + "/accounts"));

	QFile file(dataDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.GetName()) + "/accounts/accounts.dat")));

	if (!file.open(QIODeviceBase::WriteOnly))
	{
		throw FileException("Impossible d'enregistrer les données.");
	}

	QDataStream stream(&file);

	for (const BankAccount& account : profile.r_BankAccounts())
	{
		stream << account;
		dataDirectory.mkdir(QString::fromStdString("profiles/" + ToFileName(profile.GetName()) + "/accounts/" + ToFileName(account.GetName())));
		SaveOperations(profile, account);
	}
}

void SaveOperations(const Profile& profile, const BankAccount& account)
{
	QFile file(dataDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.GetName()) + "/accounts/" + ToFileName(account.GetName()) + "/operations.dat")));

	if (!file.open(QIODeviceBase::WriteOnly))
	{
		throw FileException("Impossible d'enregistrer les données.");
	}

	QDataStream stream(&file);

	for (const Operation& operation : account.r_Operations())
	{
		stream << operation;
	}
}

QDataStream& operator<<(QDataStream& stream, const Profile& profile)
{
	stream
		<< QString::fromStdString(profile.GetName());

	return stream;
}

QDataStream& operator>>(QDataStream& stream, Profile& profile)
{
	QString name;

	stream
		>> name;

	profile.Rename(name.toStdString());

	return stream;
}

QDataStream& operator<<(QDataStream& stream, const Category& category)
{
	stream
		<< QString::fromStdString(category.name)
		<< (qint32)category.monthlyBudget.GetValue();

	return stream;
}

QDataStream& operator>>(QDataStream& stream, Category& category)
{
	QString name;
	qint32 monthlyBudgetValue;

	stream
		>> name
		>> monthlyBudgetValue;

	category.name = name.toStdString();
	category.monthlyBudget = monthlyBudgetValue;

	return stream;
}

QDataStream& operator<<(QDataStream& stream, const BankAccount& account)
{
	stream
		<< QString::fromStdString(account.GetName())
		<< account.GetType()
		<< (qint32)account.GetInitialAmount().GetValue();

	return stream;
}

QDataStream& operator>>(QDataStream& stream, BankAccount& account)
{
	QString name;
	AccountType type;
	qint32 initialAmountValue;

	stream
		>> name
		>> type
		>> initialAmountValue;

	account.Rename(name.toStdString());
	account.SetType(type);
	account.SetInitialAmount(initialAmountValue);

	return stream;
}

QDataStream& operator<<(QDataStream& stream, const Operation& operation)
{
	stream
		<< operation.year
		<< operation.month
		<< (qint32)operation.amount.GetValue()
		<< operation.categoryIndex
		<< QString::fromStdString(operation.description);

	return stream;
}

QDataStream& operator>>(QDataStream& stream, Operation& operation)
{
	qint32 amountValue;
	QString description;

	stream
		>> operation.year
		>> operation.month
		>> amountValue
		>> operation.categoryIndex
		>> description;

	operation.amount = amountValue;
	operation.description = description.toStdString();

	return stream;
}