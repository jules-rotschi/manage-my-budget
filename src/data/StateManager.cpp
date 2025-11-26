#include "StateManager.h"

#include <assert.h>

#include "domain/Profile.h"
#include "domain/Category.h"
#include "domain/BankAccount.h"

#include "Data.h"

#include "exceptions/InvalidInputException.h"
#include "exceptions/ForbiddenActionException.h"

#include "files/Files.h"

static StateManager* s_Instance = nullptr;

StateManager::StateManager(const char* appVersion)
	: appVersion(appVersion) {}

void StateManager::Init(const char* appVersion)
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
	assert((index < r_CurrentProfile().r_BankAccounts().size()) && "Account index must be less or equal to last account index.");

	m_profiles[m_currentProfileIndex].SetCurrentAccountIndex(index);
}

void StateManager::AddProfile(const std::string& name)
{
	if (ToFileName(name).empty())
	{
		throw InvalidInputException("Le nom du profil doit contenir au moins un caractère (hors caractères spéciaux).");
	}

	for (const Profile& existingProfile : m_profiles)
	{
		if (ToFileName(existingProfile.GetName()) == ToFileName(name))
		{
			if (existingProfile.GetName() == name)
			{
				throw InvalidInputException("Un profil du même nom existe déjà.");
			}
			throw InvalidInputException("Un profil du même nom existe déjà (les différences de caractères spéciaux et de casse ne cont pas prises en compte).");
		}
	}

	Profile profile(true);
	profile.Rename(name);

	m_profiles.push_back(profile);

	SaveProfiles();
}

void StateManager::RenameProfile(int index, const std::string& newName)
{
	assert((index < m_profiles.size()) && "Profile index must be less or equal to last profile index.");

	if (ToFileName(newName).empty())
	{
		throw InvalidInputException("Le nom du profil doit contenir au moins un caractère (hors caractères spéciaux).");
	}

	std::string oldName = m_profiles[index].GetName();
	bool isNameEdited = newName != oldName;

	for (size_t i = 0; i < m_profiles.size(); i++)
	{
		const Profile& existingProfile = m_profiles[i];

		if (isNameEdited && ToFileName(existingProfile.GetName()) == ToFileName(newName))
		{
			if (existingProfile.GetName() == newName)
			{
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

	if (m_profiles.size() == 1)
	{
		throw ForbiddenActionException("Vous ne pouvez pas supprimer le seul profil existant.");
	}

	if (m_currentProfileIndex == index)
	{
		SetCurrentProfileIndex(0);
	}

	if (m_currentProfileIndex > index)
	{
		m_currentProfileIndex--;
	}

	m_profiles.erase(m_profiles.begin() + index);

	SaveProfiles();
}

void StateManager::AddCategory(const std::string& name, Amount monthlyBudget)
{
	if (name.empty())
	{
		throw InvalidInputException("Le nom de la catégorie ne peut être vide.");
	}

	Category category(name, monthlyBudget);

	for (const Category& existingCategory : r_CurrentProfile().r_Categories())
	{
		if (category == existingCategory) {
			throw InvalidInputException("Une catégorie du même nom existe déjà.");
		}
	}

	m_profiles[m_currentProfileIndex].AddCategory(category);

	SaveCategories(r_CurrentProfile());
}

void StateManager::EditCategory(int index, const std::string& name, Amount monthlyBudget)
{
	assert((index < r_CurrentProfile().r_Categories().size()) && "Category index must be less or equal to last category index.");

	if (r_CurrentProfile().r_Categories()[index] == Category::Internal())
	{
		throw ForbiddenActionException("Vous ne pouvez pas renommer cette catégorie.");
	}

	if (name.empty())
	{
		throw InvalidInputException("Le nom de la catégorie ne peut être vide.");
	}

	Category category(name, monthlyBudget);

	std::string oldName = r_CurrentProfile().r_Categories()[index].name;
	bool isNameEdited = category.name != oldName;

	for (const Category& existingCategory : r_CurrentProfile().r_Categories())
	{
		if (isNameEdited && category == existingCategory)
		{
			throw InvalidInputException("Une catégorie du même nom existe déjà.");
		}
	}

	m_profiles[m_currentProfileIndex].SetCategory(index, category);

	SaveCategories(r_CurrentProfile());
}

void StateManager::DeleteCategory(int index)
{
	assert((index < r_CurrentProfile().r_Categories().size()) && "Category index must be less or equal to last category index.");

	if (r_CurrentProfile().r_Categories()[index] == Category::Internal())
	{
		throw ForbiddenActionException(
			"Vous ne pouvez pas supprimer la catégorie \"Opération interne\". Cette catégorie permet de classer les opérations entre vos différents comptes."
		);
	}

	if (r_CurrentProfile().r_Categories().size() == 1)
	{
		throw ForbiddenActionException("Vous ne pouvez pas supprimer la seule catégorie existante.");
	}

	for (const BankAccount& account : r_CurrentProfile().r_BankAccounts())
	{
		for (const Operation& operation : account.r_Operations())
		{
			if (operation.categoryIndex == index)
			{
				throw ForbiddenActionException("Cette catégorie est utilisée par au moins une opération.");
			}
		}
	}

	m_profiles[m_currentProfileIndex].DeleteCategory(index);

	SaveCategories(r_CurrentProfile());
}

void StateManager::AddAccount(const std::string& name, const std::string& type, int initialAmountValue)
{
	if (ToFileName(name).empty())
	{
		throw InvalidInputException("Le nom du compte doit contenir au moins un caractère (hors caractères spéciaux).");
	}

	for (const BankAccount& existingAccount : r_CurrentProfile().r_BankAccounts())
	{
		if (ToFileName(existingAccount.GetName()) == ToFileName(name))
		{
			if (existingAccount.GetName() == name)
			{
				throw InvalidInputException("Un compte du même nom existe déjà.");
			}
			throw InvalidInputException("Un compte du même nom existe déjà (les différences de caractères spéciaux et de casse ne cont pas prises en compte).");
		}
	}

	BankAccount account;
	account.Rename(name);

	if (type == "Compte courant")
	{
		account.SetType(AccountType::CURRENT);
	}

	if (type == "Épargne")
	{
		account.SetType(AccountType::SAVING);
	}

	account.SetInitialAmount(initialAmountValue);

	m_profiles[m_currentProfileIndex].AddAccount(account);

	SaveAccounts(r_CurrentProfile());
}

void StateManager::EditAccount(int index, const std::string& name, const std::string& type, int initialAmountValue)
{
	assert((index < r_CurrentProfile().r_BankAccounts().size()) && "Account index must be less or equal to last account index.");

	if (ToFileName(name).empty())
	{
		throw InvalidInputException("Le nom du compte doit contenir au moins un caractère (hors caractères spéciaux).");
	}

	std::string oldAccountName = r_CurrentProfile().r_BankAccounts()[index].GetName();

	for (size_t i = 0; i < r_CurrentProfile().r_BankAccounts().size(); i++)
	{
		const BankAccount& existingAccount = r_CurrentProfile().r_BankAccounts()[i];

		bool isNameEdited = name != oldAccountName;

		if (isNameEdited && ToFileName(existingAccount.GetName()) == ToFileName(name))
		{
			if (existingAccount.GetName() == name)
			{
				throw InvalidInputException("Un compte du même nom existe déjà.");
			}
			throw InvalidInputException("Un compte du même nom existe déjà (les différences de caractères spéciaux et de casse ne cont pas prises en compte).");
		}
	}

	BankAccount account;
	account.Rename(name);

	if (type == "Compte courant")
	{
		account.SetType(AccountType::CURRENT);
	}
	else if (type == "Épargne")
	{
		account.SetType(AccountType::SAVING);
	}
	else
	{
		throw InvalidInputException("Type du compte invalide.");
	}

	account.SetInitialAmount(initialAmountValue);

	m_profiles[m_currentProfileIndex].EditAccount(index, account);

	SaveAccounts(r_CurrentProfile());
}

void StateManager::DeleteAccount(int index)
{
	assert((index < r_CurrentProfile().r_BankAccounts().size()) && "Account index must be less or equal to last account index.");

	if (r_CurrentProfile().r_BankAccounts().size() == 1)
	{
		throw ForbiddenActionException("Vous ne pouvez pas supprimer le seul compte du profil.");
	}

	if (r_CurrentProfile().GetCurrentAccountIndex() == index)
	{
		m_profiles[m_currentProfileIndex].SetCurrentAccountIndex(0);
	}

	if (r_CurrentProfile().GetCurrentAccountIndex() > index)
	{
		m_profiles[m_currentProfileIndex].SetCurrentAccountIndex(r_CurrentProfile().GetCurrentAccountIndex() - 1);
	}

	m_profiles[m_currentProfileIndex].DeleteAccount(index);

	SaveAccounts(r_CurrentProfile());
}

void StateManager::AddOperation(int year, int month, long amountValue, int categoryIndex, const std::string& description)
{
	if (month < 1 || month > 12)
	{
		throw InvalidInputException("Le mois doit être compris entre 1 et 12.");
	}

	if (categoryIndex >= r_CurrentProfile().r_Categories().size())
	{
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
	if (month < 1 || month > 12)
	{
		throw InvalidInputException("Le mois doit être compris entre 1 et 12.");
	}

	if (categoryIndex >= r_CurrentProfile().r_Categories().size())
	{
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
	defaultProfile.Rename("Profil principal");
	m_profiles.push_back(defaultProfile);
}

void StateManager::LoadCategory(const Category& category, int profileIndex)
{
	m_profiles[profileIndex].AddCategory(category);
}

void StateManager::LoadInternalCategory(int profileIndex)
{
	m_profiles[profileIndex].AddCategory(Category::Internal());
}

void StateManager::LoadAccount(const BankAccount& account, int profileIndex)
{
	m_profiles[profileIndex].AddAccount(account);
}

void StateManager::LoadDefaultAccount(int profileIndex)
{
	m_profiles[profileIndex].AddAccount(BankAccount::Default());
}

void StateManager::LoadOperation(const Operation& operation, int profileIndex, int accountIndex)
{
	m_profiles[profileIndex].AddOperation(accountIndex, operation);
}