#include "BackUpManager.h"

#include "InvalidInputException.h"
#include "StateManager.h"
#include "DataManager.h"
#include "Files.h"
#include "FileException.h"
#include "Converter.h"

void LoadBackUp(const std::string& backUpPath)
{
	const std::vector<Profile> profilesBefore = StateManager::Instance().r_Profiles();

	StateManager::Instance().ResetData();

	QDir directory(QString::fromStdString(backUpPath));

	if (!directory.exists())
	{
		throw InvalidInputException("Le dossier sélectionné n'existe pas.");
	}

	if (!LoadProfilesBackUp(directory))
	{
		StateManager::Instance().SetProfiles(profilesBefore);
		throw InvalidInputException("Le dossier sélectionné ne contient pas les informations nécessaires. Vous devez sélectionner un dossier qui commence par \"manage-my-budget-backup\"");
	}

	SaveData();
}

bool LoadProfilesBackUp(const QDir& backUpDirectory)
{
	QFile file(backUpDirectory.filePath("profiles/profiles.csv"));

	if (file.open(QIODeviceBase::ReadOnly))
	{
		QTextStream stream(&file);

		stream.readLine();

		while (!stream.atEnd())
		{
			Profile profile;

			QString line = stream.readLine();
			QStringList properties = line.split(',');

			assert(properties.size() == 1);

			profile.name = properties[0].toStdString();

			StateManager::Instance().LoadProfile(profile);
		}
	}
	else {
		return false;
	}

	for (size_t i = 0; i < StateManager::Instance().r_Profiles().size(); i++)
	{
		if (!LoadCategoriesBackUp(backUpDirectory, SizeToInt(i)))
		{
			return false;
		}
		if (!LoadAccountsBackUp(backUpDirectory, SizeToInt(i)))
		{
			return false;
		}
	}

	return true;
}

bool LoadCategoriesBackUp(const QDir& backUpDirectory, int profileIndex)
{
	const Profile& profile = StateManager::Instance().r_Profiles()[profileIndex];

	QFile file(backUpDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/categories.csv")));

	if (file.open(QIODeviceBase::ReadOnly))
	{
		QTextStream stream(&file);

		stream.readLine();

		while (!stream.atEnd())
		{
			Category category;

			QString line = stream.readLine();
			QStringList properties = line.split(',');

			assert(properties.size() == 1 || properties.size() == 2);

			category.name = properties[0].toStdString();

			if (properties.size() == 2) {
				bool isBudgetOk = false;
				category.monthlyBudget = properties[1].toInt(&isBudgetOk);
			}

			StateManager::Instance().LoadCategory(category, profileIndex);
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool LoadAccountsBackUp(const QDir& backUpDirectory, int profileIndex)
{
	const Profile& profile = StateManager::Instance().r_Profiles()[profileIndex];

	QFile file(backUpDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/accounts/accounts.csv")));

	if (file.open(QIODeviceBase::ReadOnly))
	{
		QTextStream stream(&file);

		stream.readLine();

		while (!stream.atEnd())
		{
			BankAccount account;

			QString line = stream.readLine();
			QStringList properties = line.split(',');

			assert(properties.size() == 3);

			account.name = properties[0].toStdString();

			if (properties[1] == "Compte courant")
			{
				account.type = AccountType::CURRENT;
			}
			if (properties[1] == "Épargne")
			{
				account.type = AccountType::SAVING;
			}

			bool isInitialAmountOk = false;
			account.initialAmount = properties[2].toInt(&isInitialAmountOk);

			assert(isInitialAmountOk);

			StateManager::Instance().LoadAccount(account, profileIndex);
		}
	}
	else
	{
		return false;
	}

	for (size_t i = 0; i < StateManager::Instance().r_Profiles()[profileIndex].bankAccounts.size(); i++)
	{
		if (!LoadOperationsBackUp(backUpDirectory, profileIndex, SizeToInt(i)))
		{
			return false;
		}
	}

	return true;
}

bool LoadOperationsBackUp(const QDir& backUpDirectory, int profileIndex, int accountIndex)
{
	const Profile& profile = StateManager::Instance().r_Profiles()[profileIndex];
	const BankAccount& account = StateManager::Instance().r_Profiles()[profileIndex].bankAccounts[accountIndex];

	QFile file(backUpDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/accounts/" + ToFileName(account.name) + "/operations.csv")));

	if (file.open(QIODeviceBase::ReadOnly))
	{
		QTextStream stream(&file);

		stream.readLine();

		while (!stream.atEnd())
		{
			Operation operation;

			QString line = stream.readLine();
			QStringList properties = line.split(',');

			assert(properties.size() == 5);

			bool isYearOk = false;
			bool isMonthOk = false;
			bool isAmountValueOk = false;
			bool isCategoryIndexOk = false;

			operation.year = properties[0].toInt(&isYearOk);
			operation.month = properties[1].toInt(&isMonthOk);
			operation.amount = properties[2].toLong(&isAmountValueOk);
			operation.categoryIndex = properties[3].toInt(&isCategoryIndexOk);
			operation.description = properties[4].toStdString();

			StateManager::Instance().LoadOperation(operation, accountIndex, profileIndex);
		}
	}
	else
	{
		return false;
	}

	return true;
}

void BackUp(const std::string& backUpPath)
{
	QDir directory(QString::fromStdString(backUpPath));

	if (!directory.exists())
	{
		throw InvalidInputException("Le dossier sélectionné n'existe pas.");
	}

	QDate currentDate = QDate::currentDate();

	QString directoryName = "manage-my-budget-backup-" + currentDate.toString(Qt::ISODate);
	directory.mkdir(directoryName);

	QDir backUpDirectory(QString::fromStdString(backUpPath + '/') + directoryName);

	QFile file(backUpDirectory.filePath("version.txt"));

	if (!file.open(QIODeviceBase::WriteOnly))
	{
		throw FileException("Impossible de sauvegarder les données.");
	}

	QTextStream stream(&file);

	stream << QString::fromStdString(StateManager::Instance().appVersion);

	BackUpProfiles(backUpDirectory);
}

void BackUpProfiles(const QDir& backUpDirectory)
{
	backUpDirectory.mkdir("profiles");

	QFile file(backUpDirectory.filePath("profiles/profiles.csv"));

	file.open(QIODeviceBase::WriteOnly);

	QTextStream stream(&file);

	stream << "Name\n";

	for (const Profile& profile : StateManager::Instance().r_Profiles())
	{
		stream << profile << '\n';
		backUpDirectory.mkdir(QString::fromStdString("profiles/" + ToFileName(profile.name)));
		BackUpCategories(backUpDirectory, profile);
		BackUpAccounts(backUpDirectory, profile);
	}
}

void BackUpCategories(const QDir& backUpDirectory, const Profile& profile)
{
	QFile file(backUpDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/categories.csv")));

	file.open(QIODeviceBase::WriteOnly);

	QTextStream stream(&file);

	stream << "Name,Monthly budget\n";

	for (const Category& category : profile.categories)
	{
		stream << category << '\n';
	}
}

void BackUpAccounts(const QDir& backUpDirectory, const Profile& profile)
{
	backUpDirectory.mkdir(backUpDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/accounts")));

	QFile file(backUpDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/accounts/accounts.csv")));

	file.open(QIODeviceBase::WriteOnly);

	QTextStream stream(&file);

	stream << "Name,Type,Initial amount\n";

	for (const BankAccount& account : profile.bankAccounts)
	{
		stream << account << '\n';
		backUpDirectory.mkdir(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/accounts/" + ToFileName(account.name)));
		BackUpOperations(backUpDirectory, profile, account);
	}
}

void BackUpOperations(const QDir& backUpDirectory, const Profile& profile, const BankAccount& account)
{
	QFile file(backUpDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/accounts/" + ToFileName(account.name) + "/operations.csv")));

	file.open(QIODeviceBase::WriteOnly);

	QTextStream stream(&file);

	stream << "Year,Month,Amount,Category index,Description\n";

	for (const Operation& operation : account.r_Operations())
	{
		stream << operation << '\n';
	}
}