#include "DataManager.h"

#include <iostream>

#include <qfile.h>
#include <qdir.h>
#include <qdebug.h>

Profile& DataManager::r_CurrentProfile()
{
	return profiles[currentProfileIndex];
}

int DataManager::GetCurrentProfileIndex() const
{
	return currentProfileIndex;
}

void DataManager::SetCurrentProfileIndex(int index)
{
	currentProfileIndex = index;
}

void DataManager::SaveProfiles() const
{
	QDir directory;
	RemoveDirectory("data/profiles");
	directory.mkdir("data");
	directory.mkdir("data/profiles");

	QFile file("data/profiles/profiles.dat");

	if (!file.open(QIODeviceBase::WriteOnly)) {
		return;
	}

	QDataStream stream(&file);

	for (const Profile& profile : profiles) {
		stream << profile;
		directory.mkdir(QString::fromStdString("data/profiles/" + ToFileName(profile.name)));
	}

	SaveCategories();
	SaveAccounts();
}

void DataManager::SaveAccounts() const
{
	QDir directory;
	directory.mkdir("data");
	directory.mkdir("data/profiles");

	for (const Profile& profile : profiles) {
		RemoveDirectory("data/profiles/" + ToFileName(profile.name) + "/accounts");
		directory.mkdir(QString::fromStdString("data/profiles/" + ToFileName(profile.name) + "/accounts"));
		QFile file(QString::fromStdString("data/profiles/" + ToFileName(profile.name) + "/accounts/accounts.dat"));

		if (!file.open(QIODeviceBase::WriteOnly)) {
			return;
		}

		QDataStream stream(&file);

		for (const BankAccount& account : profile.bankAccounts) {
			stream << account;
			directory.mkdir(QString::fromStdString("data/profiles/" + ToFileName(profile.name) + "/accounts/" + ToFileName(account.name)));
		}
	}

	SaveOperations();
}

void DataManager::SaveOperations() const
{
	QDir directory;
	directory.mkdir("data");
	directory.mkdir("data/profiles");

	for (const Profile& profile : profiles) {
		directory.mkdir(QString::fromStdString("data/profiles/" + ToFileName(profile.name) + "/accounts"));

		for (const BankAccount& account : profile.bankAccounts) {
			QFile file(QString::fromStdString("data/profiles/" + ToFileName(profile.name) + "/accounts/" + ToFileName(account.name) + "/operations.dat"));

			if (!file.open(QIODeviceBase::WriteOnly)) {
				return;
			}

			QDataStream stream(&file);

			for (const Operation& operation : account.operations) {
				stream << operation;
			}
		}
	}
}

void DataManager::SaveCategories() const
{
	QDir directory;
	directory.mkdir("data");
	directory.mkdir("data/profiles");

	for (const Profile& profile : profiles) {
		QFile file(QString::fromStdString("data/profiles/" + ToFileName(profile.name) + "/categories.dat"));

		if (!file.open(QIODeviceBase::WriteOnly)) {
			return;
		}

		QDataStream stream(&file);

		for (const std::string& category : profile.categories) {
			stream << QString::fromStdString(category);
		}
	}
}

void DataManager::LoadProfiles()
{
	QFile file("data/profiles/profiles.dat");

	if (!file.open(QIODeviceBase::ReadOnly)) {
		LoadDefaultProfile();
		return;
	}

	QDataStream stream(&file);

	while (!file.atEnd()) {
		Profile profile;
		stream >> profile;
		profiles.push_back(profile);
	}

	if (profiles.empty()) {
		LoadDefaultProfile();
	}
}

void DataManager::LoadAccounts()
{
	for (Profile& profile : profiles) {
		QFile file(QString::fromStdString("data/profiles/" + ToFileName(profile.name) + "/accounts/accounts.dat"));

		if (!file.open(QIODeviceBase::ReadOnly)) {
			profile.bankAccounts.push_back(BankAccount::Default());
		}

		QDataStream stream(&file);

		while (!file.atEnd()) {
			BankAccount account;
			stream >> account;
			profile.bankAccounts.push_back(account);
		}
	}
}

void DataManager::LoadOperations()
{
	for (Profile& profile : profiles) {
		for (BankAccount& account : profile.bankAccounts) {
			QFile file(QString::fromStdString("data/profiles/" + ToFileName(profile.name) + "/accounts/" + ToFileName(account.name) + "/operations.dat"));

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
}

void DataManager::LoadCategories()
{
	for (Profile& profile : profiles) {
		QFile file(QString::fromStdString("data/profiles/" + ToFileName(profile.name) + "/categories.dat"));

		if (!file.open(QIODeviceBase::ReadOnly)) {
			profile.categories.push_back("Opération interne");
			return;
		}

		QDataStream dataStream(&file);

		while (!file.atEnd()) {
			QString category;
			dataStream >> category;
			profile.categories.push_back(category.toStdString());
		}
	}
}

void DataManager::LoadData()
{
	LoadProfiles();
	LoadCategories();
	LoadAccounts();
	LoadOperations();
}

bool DataManager::AddProfile(const Profile& profile)
{
	if (ToFileName(profile.name).empty()) {
		return false;
	}

	for (const Profile& existingProfile : profiles) {
		if (ToFileName(existingProfile.name) == ToFileName(profile.name)) {
			return false;
		}
	}

	profiles.push_back(profile);
	return true;
}

bool DataManager::EditProfile(int index, const Profile& profile, const std::string& oldName)
{
	if (ToFileName(profile.name).empty()) {
		return false;
	}

	for (int i = 0; i < profiles.size(); i++) {
		const Profile& existingProfile = profiles[i];

		bool isNameEdited = profile.name != oldName;

		if (isNameEdited && ToFileName(existingProfile.name) == ToFileName(profile.name)) {
			return false;
		}
	}

	profiles[index].Rename(profile.name);
	return true;
}

bool DataManager::DeleteProfile(int index) {
	if (profiles.size() == 1) {
		return false;
	}

	if (currentProfileIndex > index) {
		currentProfileIndex--;
	}

	if (currentProfileIndex == index) {
		SetCurrentProfileIndex(0);
	}

	profiles.erase(profiles.begin() + index);
	return true;
}

void DataManager::LoadDefaultProfile()
{
	Profile defaultProfile;
	defaultProfile.name = "Profil principal";
	profiles.push_back(defaultProfile);
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

QDataStream& operator<<(QDataStream& stream, const Profile& profile)
{
	stream
		<< QString::fromStdString(profile.name);

	return stream;
}

QDataStream& operator>>(QDataStream& stream, Profile& profile)
{
	QString name;

	stream
		>> name;

	profile.name = name.toStdString();

	return stream;
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