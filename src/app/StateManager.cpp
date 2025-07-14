#include "StateManager.h"

#include <assert.h>

#include "InvalidInputException.h"
#include "ForbiddenActionException.h"
#include "Files.h"
#include "DataManager.h"

static StateManager* s_Instance = nullptr;

StateManager::StateManager(const std::string& appVersion)
	: appVersion(appVersion) {
}

void StateManager::Init(const std::string& appVersion)
{
	assert(!s_Instance);
	s_Instance = new StateManager(appVersion);
}

void StateManager::ShutDown()
{
	delete s_Instance;
	s_Instance = nullptr;
}

StateManager& StateManager::Instance()
{
	assert(s_Instance);
	return *s_Instance;
}

void StateManager::ResetData()
{
	m_profiles.clear();
}

void StateManager::SetProfiles(const std::vector<Profile>& profiles)
{
	m_profiles = profiles;
}

const std::vector<Profile>& StateManager::r_Profiles() const
{
	return m_profiles;
}

const Profile& StateManager::r_CurrentProfile()
{
	return m_profiles[m_currentProfileIndex];
}

int StateManager::GetCurrentProfileIndex() const
{
	return m_currentProfileIndex;
}

void StateManager::SetCurrentProfileIndex(int index)
{
	assert((index < m_profiles.size()) && "Profile index must be less or equal to last profile index.");

	m_currentProfileIndex = index;
}

void StateManager::SetCurrentProfileCurrentAccountIndex(int index)
{
	assert((index < r_CurrentProfile().bankAccounts.size()) && "Account index must be less or equal to last account index.");

	m_profiles[m_currentProfileIndex].SetCurrentAccountIndex(index);
}

void StateManager::AddProfile(const std::string& name)
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

void StateManager::RenameProfile(int index, const std::string& newName)
{
	assert((index < m_profiles.size()) && "Profile index must be less or equal to last profile index.");

	if (ToFileName(newName).empty()) {
		throw InvalidInputException("Le nom du profil doit contenir au moins un caractère (hors caractères spéciaux).");
	}

	std::string oldName = m_profiles[index].name;
	bool isNameEdited = newName != oldName;

	for (size_t i = 0; i < m_profiles.size(); i++) {
		const Profile& existingProfile = m_profiles[i];

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

void StateManager::DeleteProfile(int index)
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

void StateManager::AddCategory(const std::string& name, Amount monthlyBudget)
{
	if (name.empty()) {
		throw InvalidInputException("Le nom de la catégorie ne peut être vide.");
	}

	Category category(name, monthlyBudget);

	for (const Category& existingCategory : r_CurrentProfile().categories) {
		if (category == existingCategory) {
			throw InvalidInputException("Une catégorie du même nom existe déjà.");
		}
	}

	m_profiles[m_currentProfileIndex].categories.push_back(category);

	SaveCategories(r_CurrentProfile());
}

void StateManager::EditCategory(int index, const std::string& name, Amount monthlyBudget)
{
	assert((index < r_CurrentProfile().categories.size()) && "Category index must be less or equal to last category index.");

	if (r_CurrentProfile().categories[index] == Category::Internal()) {
		throw ForbiddenActionException("Vous ne pouvez pas renommer cette catégorie.");
	}

	if (name.empty()) {
		throw InvalidInputException("Le nom de la catégorie ne peut être vide.");
	}

	Category category(name, monthlyBudget);

	std::string oldName = r_CurrentProfile().categories[index].name;
	bool isNameEdited = category.name != oldName;

	for (const Category& existingCategory : r_CurrentProfile().categories) {

		if (isNameEdited && category == existingCategory) {
			throw InvalidInputException("Une catégorie du même nom existe déjà.");
		}
	}

	m_profiles[m_currentProfileIndex].categories[index] = category;

	SaveCategories(r_CurrentProfile());
}

void StateManager::DeleteCategory(int index)
{
	assert((index < r_CurrentProfile().categories.size()) && "Category index must be less or equal to last category index.");

	if (r_CurrentProfile().categories[index] == Category::Internal()) {
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

void StateManager::AddAccount(const std::string& name, const std::string& type, int initialAmountValue)
{
	if (ToFileName(name).empty()) {
		throw InvalidInputException("Le nom du compte doit contenir au moins un caractère (hors caractères spéciaux).");
	}

	for (const BankAccount& existingAccount : r_CurrentProfile().bankAccounts) {
		if (ToFileName(existingAccount.name) == ToFileName(name)) {
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

void StateManager::EditAccount(int index, const std::string& name, const std::string& type, int initialAmountValue)
{
	assert((index < r_CurrentProfile().bankAccounts.size()) && "Account index must be less or equal to last account index.");

	if (ToFileName(name).empty()) {
		throw InvalidInputException("Le nom du compte doit contenir au moins un caractère (hors caractères spéciaux).");
	}

	std::string oldAccountName = r_CurrentProfile().bankAccounts[index].name;

	for (size_t i = 0; i < r_CurrentProfile().bankAccounts.size(); i++) {
		const BankAccount& existingAccount = r_CurrentProfile().bankAccounts[i];

		bool isNameEdited = name != oldAccountName;

		if (isNameEdited && ToFileName(existingAccount.name) == ToFileName(name)) {
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

void StateManager::DeleteAccount(int index)
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

void StateManager::AddOperation(int year, int month, long amountValue, int categoryIndex, const std::string& description)
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

void StateManager::EditOperation(int id, int year, int month, long amountValue, int categoryIndex, const std::string& description)
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

void StateManager::DeleteOperation(int id)
{
	m_profiles[m_currentProfileIndex].r_CurrentBankAccount().DeleteOperation(id);

	SaveOperations(r_CurrentProfile(), m_profiles[m_currentProfileIndex].r_CurrentBankAccount());
}

void StateManager::LoadProfile(const Profile& profile)
{
	m_profiles.push_back(profile);
}

void StateManager::LoadDefaultProfile()
{
	Profile defaultProfile;
	defaultProfile.name = "Profil principal";
	m_profiles.push_back(defaultProfile);
}

void StateManager::LoadCategory(const Category& category, int profileIndex)
{
	m_profiles[profileIndex].categories.push_back(category);
}

void StateManager::LoadInternalCategory(int profileIndex)
{
	m_profiles[profileIndex].categories.push_back(Category::Internal());
}

void StateManager::LoadAccount(const BankAccount& account, int profileIndex)
{
	m_profiles[profileIndex].bankAccounts.push_back(account);
}

void StateManager::LoadDefaultAccount(int profileIndex)
{
	m_profiles[profileIndex].bankAccounts.push_back(BankAccount::Default());
}

void StateManager::LoadOperation(const Operation& operation, int profileIndex, int accountIndex)
{
	m_profiles[profileIndex].bankAccounts[accountIndex].AddOperation(operation);
}