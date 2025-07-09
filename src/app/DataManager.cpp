#include "DataManager.h"

#include <iostream>

#include <qfile.h>
#include <qdir.h>
#include <qdebug.h>

#include "app/InvalidInputException.h"
#include "app/ForbiddenActionException.h"

Profile& DataManager::r_CurrentProfile()
{
	return profiles[m_currentProfileIndex];
}

int DataManager::GetCurrentProfileIndex() const
{
	return m_currentProfileIndex;
}

void DataManager::SetCurrentProfileIndex(int index)
{
	m_currentProfileIndex = index;
}

void DataManager::AddProfile(const std::string& name)
{
	if (ToFileName(name).empty()) {
		throw InvalidInputException("Le nom du profil doit contenir au moins un caractère (hors caractères spéciaux).");
	}

	for (const Profile& existingProfile : profiles) {
		if (ToFileName(existingProfile.name) == ToFileName(name)) {
			if (existingProfile.name == name) {
				throw InvalidInputException("Un profil du même nom existe déjà.");
			}
			throw InvalidInputException("Un profil du même nom existe déjà (les différences de caractères spéciaux et de casse ne cont pas prises en compte).");
		}
	}

	Profile profile(true);
	profile.name = name;

	profiles.push_back(profile);

	SaveProfiles();
}

void DataManager::RenameProfile(int index, const std::string& newName, const std::string& oldName)
{
	if (ToFileName(newName).empty()) {
		throw InvalidInputException("Le nom du profil doit contenir au moins un caractère (hors caractères spéciaux).");
	}

	for (int i = 0; i < profiles.size(); i++) {
		const Profile& existingProfile = profiles[i];

		bool isNameEdited = newName != oldName;

		if (isNameEdited && ToFileName(existingProfile.name) == ToFileName(newName)) {
			if (existingProfile.name == newName) {
				throw InvalidInputException("Un profil du même nom existe déjà.");
			}
			throw InvalidInputException("Un profil du même nom existe déjà (les différences de caractères spéciaux et de casse ne cont pas prises en compte).");
		}
	}

	profiles[index].Rename(newName);

	SaveProfiles();
}

void DataManager::DeleteProfile(int index) {
	if (profiles.size() == 1) {
		throw ForbiddenActionException("Vous ne pouvez pas supprimer le seul profil existant.");
	}

	if (m_currentProfileIndex > index) {
		m_currentProfileIndex--;
	}

	if (m_currentProfileIndex == index) {
		SetCurrentProfileIndex(0);
	}

	profiles.erase(profiles.begin() + index);

	SaveProfiles();
}

void DataManager::AddCategory(const std::string& category)
{
	if (category.empty()) {
		throw InvalidInputException("Le nom de la catégorie ne peut être vide.");
	}

	for (const std::string& existingCategory : r_CurrentProfile().categories) {
		if (category == existingCategory) {
			throw InvalidInputException("Une catégorie du même nom existe déjà.");
		}
	}

	r_CurrentProfile().categories.push_back(category);

	SaveCategories(r_CurrentProfile());
}

void DataManager::RenameCategory(int index, const std::string& newName)
{
	if (r_CurrentProfile().categories[index] == "Opération interne") {
		throw ForbiddenActionException("Vous ne pouvez pas renommer cette catégorie.");
	}

	if (newName.empty()) {
		throw InvalidInputException("Le nom de la catégorie ne peut être vide.");
	}

	for (const std::string& existingCategoryName : r_CurrentProfile().categories) {
		if (newName == existingCategoryName) {
			throw InvalidInputException("Une catégorie du même nom existe déjà.");
		}
	}

	r_CurrentProfile().categories[index] = newName;

	SaveCategories(r_CurrentProfile());
}

void DataManager::DeleteCategory(int index)
{
	if (r_CurrentProfile().categories[index] == "Opération interne") {
		throw ForbiddenActionException(
			"Vous ne pouvez pas supprimer la catégorie \"Opération interne\". Cette catégorie permet de classer les opérations entre vos différents comptes."
		);
	}

	if (r_CurrentProfile().categories.size() == 1) {
		throw ForbiddenActionException("Vous ne pouvez pas supprimer la seule catégorie existante.");
	}

	for (const BankAccount& account : r_CurrentProfile().bankAccounts) {
		for (const Operation& operation : account.r_operations()) {
			if (operation.categoryIndex == index) {
				throw ForbiddenActionException("Cette catégorie est utilisée par au moins une opération.");
			}
		}
	}
	
	r_CurrentProfile().r_CurrentBankAccount().HandleCategoryDelete(index);
	r_CurrentProfile().categories.erase(r_CurrentProfile().categories.begin() + index);

	SaveCategories(r_CurrentProfile());
}

void DataManager::AddAccount(const std::string& name, const std::string& type, int initialAmountValue)
{
	if (s_DataManager.ToFileName(name).empty()) {
		throw InvalidInputException("Le nom du compte doit contenir au moins un caractère (hors caractères spéciaux).");
	}

	for (const BankAccount& existingAccount : r_CurrentProfile().bankAccounts) {
		if (s_DataManager.ToFileName(existingAccount.name) == s_DataManager.ToFileName(name)) {
			if (existingAccount.name == name) {
				throw InvalidInputException("Un compte du même nom existe déjà.");
			}
			throw InvalidInputException("Un compte du même nom existe déjà (les différences de caractères spéciaux et de casse ne cont pas prises en compte).");
		}
	}

	BankAccount account;
	account.name = name;

	if (type == "Compte courant") {
		account.type = AccountType::CURRENT;
	}

	if (type == "Épargne") {
		account.type = AccountType::SAVING;
	}

	account.initialAmount = initialAmountValue;

	r_CurrentProfile().bankAccounts.push_back(account);

	SaveAccounts(r_CurrentProfile());
}

void DataManager::EditAccount(int index, const std::string& name, const std::string& type, int initialAmountValue, const std::string& oldAccountName)
{
	if (s_DataManager.ToFileName(name).empty()) {
		throw InvalidInputException("Le nom du compte doit contenir au moins un caractère (hors caractères spéciaux).");
	}

	for (int i = 0; i < r_CurrentProfile().bankAccounts.size(); i++) {
		const BankAccount& existingAccount = r_CurrentProfile().bankAccounts[i];

		bool isNameEdited = name != oldAccountName;

		if (isNameEdited && s_DataManager.ToFileName(existingAccount.name) == s_DataManager.ToFileName(name)) {
			if (existingAccount.name == name) {
				throw InvalidInputException("Un compte du même nom existe déjà.");
			}
			throw InvalidInputException("Un compte du même nom existe déjà (les différences de caractères spéciaux et de casse ne cont pas prises en compte).");
		}
	}

	BankAccount account;
	account.name = name;

	if (type == "Compte courant") {
		account.type = AccountType::CURRENT;
	}

	if (type == "Épargne") {
		account.type = AccountType::SAVING;
	}

	account.initialAmount = initialAmountValue;

	r_CurrentProfile().bankAccounts[index].Edit(account);

	SaveAccounts(r_CurrentProfile());
}

void DataManager::DeleteAccount(int index)
{
	Profile& currentProfile = r_CurrentProfile();

	if (currentProfile.bankAccounts.size() == 1) {
		throw ForbiddenActionException("Vous ne pouvez pas supprimer le seul compte du profil.");
	}

	if (currentProfile.GetCurrentAccountIndex() > index) {
		currentProfile.SetCurrentAccountIndex(currentProfile.GetCurrentAccountIndex() - 1);
	}

	if (currentProfile.GetCurrentAccountIndex() == index) {
		currentProfile.SetCurrentAccountIndex(0);
	}

	currentProfile.bankAccounts.erase(currentProfile.bankAccounts.begin() + index);

	SaveAccounts(r_CurrentProfile());
}

void DataManager::AddOperation(int year, int month, int amountValue, int categoryIndex, std::string description)
{
	Operation operation;
	operation.year = year;
	operation.month = month;
	operation.amount = amountValue;
	operation.categoryIndex = categoryIndex;
	operation.description = description;

	r_CurrentProfile().r_CurrentBankAccount().AddOperation(operation);

	SaveOperations(r_CurrentProfile(), r_CurrentProfile().r_CurrentBankAccount());
}

void DataManager::EditOperation(int id, int year, int month, int amountValue, int categoryIndex, std::string description)
{
	Operation operation;
	operation.year = year;
	operation.month = month;
	operation.categoryIndex = categoryIndex;
	operation.amount = amountValue;
	operation.description = description;

	r_CurrentProfile().r_CurrentBankAccount().EditOperation(id, operation);

	SaveOperations(r_CurrentProfile(), r_CurrentProfile().r_CurrentBankAccount());
}

void DataManager::DeleteOperation(int id)
{
	r_CurrentProfile().r_CurrentBankAccount().DeleteOperation(id);

	SaveOperations(r_CurrentProfile(), r_CurrentProfile().r_CurrentBankAccount());
}

std::string DataManager::ToFileName(std::string str) const
{
	std::transform(str.begin(), str.end(), str.begin(), tolower);

	std::replace(str.begin(), str.end(), ' ', '-');

	for (int i = 0; i < str.size(); i++) {
		char c = str[i];

		if ((c < 97 || c > 122) && c != '-') {
			str.erase(i, 1);
			i--;
		}
	}

	return str;
}

void DataManager::InitializeData()
{
	LoadData();
	SaveData();
}

void DataManager::LoadData()
{
	LoadProfiles();
}

void DataManager::LoadProfiles()
{
	QFile file("data/profiles/profiles.dat");

	if (file.open(QIODeviceBase::ReadOnly)) {
		QDataStream stream(&file);

		while (!file.atEnd()) {
			Profile profile;
			stream >> profile;
			profiles.push_back(profile);
		}
	}
	else {
		LoadDefaultProfile();
	}

	for (Profile& profile : profiles) {
		LoadCategories(profile);
		LoadAccounts(profile);
	}
}

void DataManager::LoadCategories(Profile& profile) const
{
	QFile file(QString::fromStdString("data/profiles/" + ToFileName(profile.name) + "/categories.dat"));

	if (file.open(QIODeviceBase::ReadOnly)) {
		QDataStream dataStream(&file);

		while (!file.atEnd()) {
			QString category;
			dataStream >> category;
			profile.categories.push_back(category.toStdString());
		}
	}
	else {
		profile.categories.push_back("Opération interne");
	}
}

void DataManager::LoadAccounts(Profile& profile) const
{
	QFile file(QString::fromStdString("data/profiles/" + ToFileName(profile.name) + "/accounts/accounts.dat"));

	if (file.open(QIODeviceBase::ReadOnly)) {
		QDataStream stream(&file);

		while (!file.atEnd()) {
			BankAccount account;
			stream >> account;
			profile.bankAccounts.push_back(account);
		}
	}
	else {
		profile.bankAccounts.push_back(BankAccount::Default());
	}

	for (BankAccount& account : profile.bankAccounts) {
		LoadOperations(profile, account);
	}
}

void DataManager::LoadOperations(const Profile& profile, BankAccount& account) const
{
	QFile file(QString::fromStdString("data/profiles/" + ToFileName(profile.name) + "/accounts/" + ToFileName(account.name) + "/operations.dat"));

	if (!file.open(QIODeviceBase::ReadOnly)) {
		return;
	}

	QDataStream stream(&file);

	while (!file.atEnd()) {
		Operation operation;
		stream >> operation;
		account.AddOperation(operation);
	}
}

void DataManager::SaveData() const
{
	QDir directory;
	directory.mkdir("data");
	SaveProfiles();
}

void DataManager::SaveProfiles() const
{
	QDir directory;
	RemoveDirectory("data/profiles");
	directory.mkdir("data/profiles");

	QFile file("data/profiles/profiles.dat");

	if (!file.open(QIODeviceBase::WriteOnly)) {
		return;
	}

	QDataStream stream(&file);

	for (const Profile& profile : profiles) {
		stream << profile;
		directory.mkdir(QString::fromStdString("data/profiles/" + ToFileName(profile.name)));
		SaveCategories(profile);
		SaveAccounts(profile);
	}

}

void DataManager::SaveCategories(const Profile& profile) const
{
	QFile file(QString::fromStdString("data/profiles/" + ToFileName(profile.name) + "/categories.dat"));

	if (!file.open(QIODeviceBase::WriteOnly)) {
		return;
	}

	QDataStream stream(&file);

	for (const std::string& category : profile.categories) {
		stream << QString::fromStdString(category);
	}
}

void DataManager::SaveAccounts(const Profile& profile) const
{
	QDir directory;
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
		SaveOperations(profile, account);
	}

}

void DataManager::SaveOperations(const Profile& profile, const BankAccount& account) const
{
	QFile file(QString::fromStdString("data/profiles/" + ToFileName(profile.name) + "/accounts/" + ToFileName(account.name) + "/operations.dat"));

	if (!file.open(QIODeviceBase::WriteOnly)) {
		return;
	}

	QDataStream stream(&file);

	for (const Operation& operation : account.r_operations()) {
		stream << operation;
	}
}

void DataManager::LoadDefaultProfile()
{
	Profile defaultProfile;
	defaultProfile.name = "Profil principal";
	profiles.push_back(defaultProfile);
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
		<< account.initialAmount.GetValue();

	return stream;
}

QDataStream& operator>>(QDataStream& stream, BankAccount& account)
{
	QString name;
	int initialAmountValue;

	stream
		>> name
		>> account.type
		>> initialAmountValue;

	account.name = name.toStdString();
	account.initialAmount = initialAmountValue;

	return stream;
}

QDataStream& operator<<(QDataStream& stream, const Operation& operation)
{
	stream
		<< operation.year
		<< operation.month
		<< operation.amount.GetValue()
		<< operation.categoryIndex
		<< QString::fromStdString(operation.description);

	return stream;
}

QDataStream& operator>>(QDataStream& stream, Operation& operation)
{
	int amountValue;
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