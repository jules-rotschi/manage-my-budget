#include "DataManager.h"

#include <iostream>
#include <assert.h>

#include <qfile.h>
#include <qdir.h>

#include "InvalidInputException.h"
#include "ForbiddenActionException.h"
#include "FileException.h"

const std::vector<Profile>& DataManager::r_Profiles() const
{
	return m_profiles;
}

const Profile& DataManager::r_CurrentProfile()
{
	return m_profiles[m_currentProfileIndex];
}

int DataManager::GetCurrentProfileIndex() const
{
	return m_currentProfileIndex;
}

void DataManager::SetCurrentProfileIndex(int index)
{
	assert((index < m_profiles.size()) && "Profile index must be less or equal to last profile index.");

	m_currentProfileIndex = index;
}

void DataManager::SetCurrentProfileCurrentAccountIndex(int index)
{
	assert((index < r_CurrentProfile().bankAccounts.size()) && "Account index must be less or equal to last account index.");

	m_profiles[m_currentProfileIndex].SetCurrentAccountIndex(index);
}

void DataManager::AddProfile(const std::string& name)
{
	if (ToFileName(name).empty()) {
		throw InvalidInputException("Le nom du profil doit contenir au moins un caractère (hors caractères spéciaux).");
	}

	for (const Profile& existingProfile : m_profiles) {
		if (ToFileName(existingProfile.name) == ToFileName(name)) {
			if (existingProfile.name == name) {
				throw InvalidInputException("Un profil du même nom existe déjà.");
			}
			throw InvalidInputException("Un profil du même nom existe déjà (les différences de caractères spéciaux et de casse ne cont pas prises en compte).");
		}
	}

	Profile profile(true);
	profile.name = name;

	m_profiles.push_back(profile);

	SaveProfiles();
}

void DataManager::RenameProfile(int index, const std::string& newName)
{
	assert((index < m_profiles.size()) && "Profile index must be less or equal to last profile index.");

	if (ToFileName(newName).empty()) {
		throw InvalidInputException("Le nom du profil doit contenir au moins un caractère (hors caractères spéciaux).");
	}

	std::string oldName = m_profiles[index].name;

	for (int i = 0; i < m_profiles.size(); i++) {
		const Profile& existingProfile = m_profiles[i];

		bool isNameEdited = newName != oldName;

		if (isNameEdited && ToFileName(existingProfile.name) == ToFileName(newName)) {
			if (existingProfile.name == newName) {
				throw InvalidInputException("Un profil du même nom existe déjà.");
			}
			throw InvalidInputException("Un profil du même nom existe déjà (les différences de caractères spéciaux et de casse ne cont pas prises en compte).");
		}
	}

	m_profiles[index].Rename(newName);

	SaveProfiles();
}

void DataManager::DeleteProfile(int index)
{
	assert((index < m_profiles.size()) && "Profile index must be less or equal to last profile index.");

	if (m_profiles.size() == 1) {
		throw ForbiddenActionException("Vous ne pouvez pas supprimer le seul profil existant.");
	}

	if (m_currentProfileIndex == index) {
		SetCurrentProfileIndex(0);
	}

	if (m_currentProfileIndex > index) {
		m_currentProfileIndex--;
	}

	m_profiles.erase(m_profiles.begin() + index);

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

	m_profiles[m_currentProfileIndex].categories.push_back(category);

	SaveCategories(r_CurrentProfile());
}

void DataManager::RenameCategory(int index, const std::string& newName)
{
	assert((index < r_CurrentProfile().categories.size()) && "Category index must be less or equal to last category index.");

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

	m_profiles[m_currentProfileIndex].categories[index] = newName;

	SaveCategories(r_CurrentProfile());
}

void DataManager::DeleteCategory(int index)
{
	assert((index < r_CurrentProfile().categories.size()) && "Category index must be less or equal to last category index.");

	if (r_CurrentProfile().categories[index] == "Opération interne") {
		throw ForbiddenActionException(
			"Vous ne pouvez pas supprimer la catégorie \"Opération interne\". Cette catégorie permet de classer les opérations entre vos différents comptes."
		);
	}

	if (r_CurrentProfile().categories.size() == 1) {
		throw ForbiddenActionException("Vous ne pouvez pas supprimer la seule catégorie existante.");
	}

	for (const BankAccount& account : r_CurrentProfile().bankAccounts) {
		for (const Operation& operation : account.r_Operations()) {
			if (operation.categoryIndex == index) {
				throw ForbiddenActionException("Cette catégorie est utilisée par au moins une opération.");
			}
		}
	}
	
	m_profiles[m_currentProfileIndex].r_CurrentBankAccount().HandleCategoryDelete(index);
	m_profiles[m_currentProfileIndex].categories.erase(r_CurrentProfile().categories.begin() + index);

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

	m_profiles[m_currentProfileIndex].bankAccounts.push_back(account);

	SaveAccounts(r_CurrentProfile());
}

void DataManager::EditAccount(int index, const std::string& name, const std::string& type, int initialAmountValue)
{
	assert((index < r_CurrentProfile().bankAccounts.size()) && "Account index must be less or equal to last account index.");

	if (s_DataManager.ToFileName(name).empty()) {
		throw InvalidInputException("Le nom du compte doit contenir au moins un caractère (hors caractères spéciaux).");
	}

	std::string oldAccountName = r_CurrentProfile().bankAccounts[index].name;

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
	else if (type == "Épargne") {
		account.type = AccountType::SAVING;
	}
	else {
		throw InvalidInputException("Type du compte invalide.");
	}

	account.initialAmount = initialAmountValue;

	m_profiles[m_currentProfileIndex].bankAccounts[index].Edit(account);

	SaveAccounts(r_CurrentProfile());
}

void DataManager::DeleteAccount(int index)
{
	assert((index < r_CurrentProfile().bankAccounts.size()) && "Account index must be less or equal to last account index.");
	
	if (r_CurrentProfile().bankAccounts.size() == 1) {
		throw ForbiddenActionException("Vous ne pouvez pas supprimer le seul compte du profil.");
	}

	if (r_CurrentProfile().GetCurrentAccountIndex() == index) {
		m_profiles[m_currentProfileIndex].SetCurrentAccountIndex(0);
	}

	if (r_CurrentProfile().GetCurrentAccountIndex() > index) {
		m_profiles[m_currentProfileIndex].SetCurrentAccountIndex(r_CurrentProfile().GetCurrentAccountIndex() - 1);
	}

	m_profiles[m_currentProfileIndex].bankAccounts.erase(r_CurrentProfile().bankAccounts.begin() + index);

	SaveAccounts(r_CurrentProfile());
}

void DataManager::AddOperation(int year, int month, int amountValue, int categoryIndex, const std::string& description)
{
	if (month < 1 || month > 12) {
		throw InvalidInputException("Le mois doit être compris entre 1 et 12.");
	}

	if (categoryIndex >= r_CurrentProfile().categories.size()) {
		throw InvalidInputException("La catégorie n'existe pas.");
	}

	Operation operation;
	operation.year = year;
	operation.month = month;
	operation.amount = amountValue;
	operation.categoryIndex = categoryIndex;
	operation.description = description;

	m_profiles[m_currentProfileIndex].r_CurrentBankAccount().AddOperation(operation);

	SaveOperations(r_CurrentProfile(), m_profiles[m_currentProfileIndex].r_CurrentBankAccount());
}

void DataManager::EditOperation(int id, int year, int month, int amountValue, int categoryIndex, const std::string& description)
{
	if (month < 1 || month > 12) {
		throw InvalidInputException("Le mois doit être compris entre 1 et 12.");
	}

	if (categoryIndex >= r_CurrentProfile().categories.size()) {
		throw InvalidInputException("La catégorie n'existe pas.");
	}

	Operation operation;
	operation.year = year;
	operation.month = month;
	operation.categoryIndex = categoryIndex;
	operation.amount = amountValue;
	operation.description = description;

	m_profiles[m_currentProfileIndex].r_CurrentBankAccount().EditOperation(id, operation);

	SaveOperations(r_CurrentProfile(), m_profiles[m_currentProfileIndex].r_CurrentBankAccount());
}

void DataManager::DeleteOperation(int id)
{
	m_profiles[m_currentProfileIndex].r_CurrentBankAccount().DeleteOperation(id);

	SaveOperations(r_CurrentProfile(), m_profiles[m_currentProfileIndex].r_CurrentBankAccount());
}

std::string DataManager::ToFileName(std::string strCopy) const
{
	std::transform(strCopy.begin(), strCopy.end(), strCopy.begin(), tolower);

	std::replace(strCopy.begin(), strCopy.end(), ' ', '-');

	for (int i = 0; i < strCopy.size(); i++) {
		char c = strCopy[i];

		if ((c < 97 || c > 122) && c != '-') {
			strCopy.erase(i, 1);
			i--;
		}
	}

	return strCopy;
}

void DataManager::InitializeData()
{
	LoadData();
	SaveData();
}

void DataManager::InitializeData(const QDir& dataDirectory)
{
	m_dataDirectory = dataDirectory;
	LoadData();
	SaveData();
}

void DataManager::ResetData()
{
	if (!RemoveDirectory(m_dataDirectory.filePath(QString::fromStdString(m_dataSubDirectoryName)).toStdString())) {
		throw FileException("Impossible de réinitialiser les données enregistrées sur l'ordinateur.");
	}
	m_profiles.clear();
}

void DataManager::LoadData()
{
	LoadProfiles();
}

void DataManager::LoadProfiles()
{
	QDir dataSubDirectory(m_dataDirectory.filePath(QString::fromStdString(m_dataSubDirectoryName)));

	QFile file(dataSubDirectory.filePath("profiles/profiles.dat"));

	if (file.open(QIODeviceBase::ReadOnly)) {
		QDataStream stream(&file);

		while (!file.atEnd()) {
			Profile profile;
			stream >> profile;
			m_profiles.push_back(profile);
		}
	}
	else {
		LoadDefaultProfile();
	}

	for (Profile& profile : m_profiles) {
		LoadCategories(profile);
		LoadAccounts(profile);
	}
}

void DataManager::LoadCategories(Profile& profile) const
{
	QDir dataSubDirectory(m_dataDirectory.filePath(QString::fromStdString(m_dataSubDirectoryName)));

	QFile file(dataSubDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/categories.dat")));

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
	QDir dataSubDirectory(m_dataDirectory.filePath(QString::fromStdString(m_dataSubDirectoryName)));

	QFile file(dataSubDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/accounts/accounts.dat")));

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
	QDir dataSubDirectory(m_dataDirectory.filePath(QString::fromStdString(m_dataSubDirectoryName)));

	QFile file(dataSubDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/accounts/" + ToFileName(account.name) + "/operations.dat")));

	file.open(QIODeviceBase::ReadOnly);

	QDataStream stream(&file);

	while (!file.atEnd()) {
		Operation operation;
		stream >> operation;
		account.AddOperation(operation);
	}
}

void DataManager::SaveData() const
{
	m_dataDirectory.mkdir(QString::fromStdString(m_dataSubDirectoryName));
	SaveProfiles();
}

void DataManager::SaveProfiles() const
{
	QDir dataSubDirectory(m_dataDirectory.filePath(QString::fromStdString(m_dataSubDirectoryName)));

	RemoveDirectory(dataSubDirectory.filePath("profiles").toStdString());
	dataSubDirectory.mkdir("profiles");

	QFile file(dataSubDirectory.filePath("profiles/profiles.dat"));

	if (!file.open(QIODeviceBase::WriteOnly)) {
		throw FileException("Impossible d'enregistrer les données.");
	}

	QDataStream stream(&file);

	for (const Profile& profile : m_profiles) {
		stream << profile;
		dataSubDirectory.mkdir(QString::fromStdString("profiles/" + ToFileName(profile.name)));
		SaveCategories(profile);
		SaveAccounts(profile);
	}
}

void DataManager::SaveCategories(const Profile& profile) const
{
	QDir dataSubDirectory(m_dataDirectory.filePath(QString::fromStdString(m_dataSubDirectoryName)));

	QFile file(dataSubDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/categories.dat")));

	if (!file.open(QIODeviceBase::WriteOnly)) {
		throw FileException("Impossible d'enregistrer les données.");
	}

	QDataStream stream(&file);

	for (const std::string& category : profile.categories) {
		stream << QString::fromStdString(category);
	}
}

void DataManager::SaveAccounts(const Profile& profile) const
{
	QDir dataSubDirectory(m_dataDirectory.filePath(QString::fromStdString(m_dataSubDirectoryName)));

	RemoveDirectory(dataSubDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/accounts")).toStdString());
	dataSubDirectory.mkdir(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/accounts"));
	
	QFile file(dataSubDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/accounts/accounts.dat")));

	if (!file.open(QIODeviceBase::WriteOnly)) {
		throw FileException("Impossible d'enregistrer les données.");
	}

	QDataStream stream(&file);

	for (const BankAccount& account : profile.bankAccounts) {
		stream << account;
		dataSubDirectory.mkdir(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/accounts/" + ToFileName(account.name)));
		SaveOperations(profile, account);
	}
}

void DataManager::SaveOperations(const Profile& profile, const BankAccount& account) const
{
	QDir dataSubDirectory(m_dataDirectory.filePath(QString::fromStdString(m_dataSubDirectoryName)));

	QFile file(dataSubDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/accounts/" + ToFileName(account.name) + "/operations.dat")));

	if (!file.open(QIODeviceBase::WriteOnly)) {
		throw FileException("Impossible d'enregistrer les données.");
	}

	QDataStream stream(&file);

	for (const Operation& operation : account.r_Operations()) {
		stream << operation;
	}
}

void DataManager::LoadBackUp(const std::string& backUpPath)
{
	const std::vector<Profile> profilesBefore = m_profiles;

	ResetData();

	QDir directory(QString::fromStdString(backUpPath));

	if (!directory.exists()) {
		throw InvalidInputException("Le dossier sélectionné n'existe pas.");
	}

	if (!LoadProfilesBackUp(directory)) {
		m_profiles = profilesBefore;
		throw InvalidInputException("Le dossier sélectionné ne contient pas les informations nécessaires. Vous devez sélectionner un dossier qui commence par \"manage-my-budget-backup\"");
	}

	SaveData();
}

bool DataManager::LoadProfilesBackUp(const QDir& backUpDirectory)
{
	QFile file(backUpDirectory.filePath("profiles/profiles.csv"));

	if (file.open(QIODeviceBase::ReadOnly)) {
		QTextStream stream(&file);

		stream.readLine();

		while (!stream.atEnd()) {
			Profile profile;

			QString line = stream.readLine();
			QStringList properties = line.split(',');

			assert(properties.size() == 1);

			profile.name = properties[0].toStdString();

			m_profiles.push_back(profile);
		}
	}
	else {
		return false;
	}

	for (Profile& profile : m_profiles) {
		if (!LoadCategoriesBackUp(backUpDirectory, profile)) {
			return false;
		}
		if (!LoadAccountsBackUp(backUpDirectory, profile)) {
			return false;
		}
	}

	return true;
}

bool DataManager::LoadCategoriesBackUp(const QDir& backUpDirectory, Profile& profile) const
{
	QFile file(backUpDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/categories.csv")));

	if (file.open(QIODeviceBase::ReadOnly)) {
		QTextStream stream(&file);

		stream.readLine();

		while (!stream.atEnd()) {
			std::string category;

			QString line = stream.readLine();
			QStringList properties = line.split(',');

			assert(properties.size() == 1);

			category = properties[0].toStdString();

			profile.categories.push_back(category);
		}
	}
	else {
		return false;
	}

	return true;
}

bool DataManager::LoadAccountsBackUp(const QDir& backUpDirectory, Profile& profile) const
{
	QFile file(backUpDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/accounts/accounts.csv")));

	if (file.open(QIODeviceBase::ReadOnly)) {
		QTextStream stream(&file);

		stream.readLine();

		while (!stream.atEnd()) {
			BankAccount account;

			QString line = stream.readLine();
			QStringList properties = line.split(',');

			assert(properties.size() == 3);

			account.name = properties[0].toStdString();

			if (properties[1] == "Compte courant") {
				account.type = AccountType::CURRENT;
			}
			if (properties[1] == "Épargne") {
				account.type = AccountType::SAVING;
			}

			bool isInitialAmountOk = false;
			account.initialAmount = properties[2].toInt(&isInitialAmountOk);

			assert(isInitialAmountOk);

			profile.bankAccounts.push_back(account);
		}
	}
	else {
		return false;
	}

	for (BankAccount& account : profile.bankAccounts) {
		if (!LoadOperationsBackUp(backUpDirectory, profile, account)) {
			return false;
		}
	}

	return true;
}

bool DataManager::LoadOperationsBackUp(const QDir& backUpDirectory, const Profile& profile, BankAccount& account) const
{
	QFile file(backUpDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/accounts/" + ToFileName(account.name) + "/operations.csv")));

	if (file.open(QIODeviceBase::ReadOnly)) {
		QTextStream stream(&file);

		stream.readLine();
		
		while (!stream.atEnd()) {
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
			operation.amount = properties[2].toInt(&isAmountValueOk);
			operation.categoryIndex = properties[3].toInt(&isCategoryIndexOk);
			operation.description = properties[4].toStdString();

			account.AddOperation(operation);
		}
	}
	else {
		return false;
	}

	return true;
}

void DataManager::BackUp(const std::string& backUpPath)
{
	QDir directory(QString::fromStdString(backUpPath));

	if (!directory.exists()) {
		throw InvalidInputException("Le dossier sélectionné n'existe pas.");
	}

	QDate currentDate = QDate::currentDate();
	
	QString directoryName = "manage-my-budget-backup-" + currentDate.toString(Qt::ISODate);
	directory.mkdir(directoryName);

	QDir backUpDirectory(QString::fromStdString(backUpPath + '/') + directoryName);

	BackUpProfiles(backUpDirectory);
}

void DataManager::BackUpProfiles(const QDir& backUpDirectory) const
{
	backUpDirectory.mkdir("profiles");

	QFile file(backUpDirectory.filePath("profiles/profiles.csv"));

	file.open(QIODeviceBase::WriteOnly);

	QTextStream stream(&file);

	stream << "Name\n";

	for (const Profile& profile : m_profiles) {
		stream << profile << '\n';
		backUpDirectory.mkdir(QString::fromStdString("profiles/" + ToFileName(profile.name)));
		BackUpCategories(backUpDirectory, profile);
		BackUpAccounts(backUpDirectory, profile);
	}
}

void DataManager::BackUpCategories(const QDir& backUpDirectory, const Profile& profile) const
{
	QFile file(backUpDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/categories.csv")));

	file.open(QIODeviceBase::WriteOnly);

	QTextStream stream(&file);

	stream << "Name\n";

	for (const std::string& category : profile.categories) {
		stream << QString::fromStdString(category) << '\n';
	}
}

void DataManager::BackUpAccounts(const QDir& backUpDirectory, const Profile& profile) const
{
	backUpDirectory.mkdir(backUpDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/accounts")));

	QFile file(backUpDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/accounts/accounts.csv")));

	file.open(QIODeviceBase::WriteOnly);

	QTextStream stream(&file);

	stream << "Name,Type,Initial amount\n";

	for (const BankAccount& account : profile.bankAccounts) {
		stream << account << '\n';
		backUpDirectory.mkdir(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/accounts/" + ToFileName(account.name)));
		BackUpOperations(backUpDirectory, profile, account);
	}
}

void DataManager::BackUpOperations(const QDir& backUpDirectory, const Profile& profile, const BankAccount& account) const
{
	QFile file(backUpDirectory.filePath(QString::fromStdString("profiles/" + ToFileName(profile.name) + "/accounts/" + ToFileName(account.name) + "/operations.csv")));

	file.open(QIODeviceBase::WriteOnly);

	QTextStream stream(&file);

	stream << "Year,Month,Amount,Category index,Description\n";

	for (const Operation& operation : account.r_Operations()) {
		stream << operation << '\n';
	}
}

void DataManager::LoadDefaultProfile()
{
	Profile defaultProfile;
	defaultProfile.name = "Profil principal";
	m_profiles.push_back(defaultProfile);
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

QTextStream& operator<<(QTextStream& stream, const Profile& profile)
{
	stream
		<< QString::fromStdString(profile.name);

	return stream;
}

QTextStream& operator<<(QTextStream& stream, const BankAccount& account)
{
	QString separator = ",";

	stream
		<< QString::fromStdString(account.name)
		<< separator
		<< QString::fromStdString(account.GetTypeString())
		<< separator
		<< account.initialAmount.GetValue();

	return stream;
}

QTextStream& operator<<(QTextStream& stream, const Operation& operation)
{
	QString separator = ",";

	stream
		<< operation.year
		<< separator
		<< operation.month
		<< separator
		<< operation.amount.GetValue()
		<< separator
		<< operation.categoryIndex
		<< separator
		<< QString::fromStdString(operation.description);

	return stream;
}