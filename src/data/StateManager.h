#pragma once

#include <vector>
#include <string>

class Profile;
class BankAccount;
struct Operation;
class Amount;
struct Category;

class StateManager
{
public:
	const std::string& appVersion;

	StateManager(const std::string& appVersion);

	static void Init(const std::string& appVersion);
	static void ShutDown();

	static StateManager& Instance();

	void ResetData();

	void SetProfiles(const std::vector<Profile>& profiles);

	const std::vector<Profile>& r_Profiles() const;

	const Profile& r_CurrentProfile();
	int GetCurrentProfileIndex() const;
	void SetCurrentProfileIndex(int index);

	void SetCurrentProfileCurrentAccountIndex(int index);

	void AddProfile(const std::string& name);
	void RenameProfile(int index, const std::string& newName);
	void DeleteProfile(int index);

	void AddCategory(const std::string& name, Amount monthlyBudget);
	void EditCategory(int index, const std::string& name, Amount monthlyBudget);
	void DeleteCategory(int index);

	void AddAccount(const std::string& name, const std::string& type, int initialAmountValue);
	void EditAccount(int index, const std::string& name, const std::string& type, int initialAmountValue);
	void DeleteAccount(int index);

	void AddOperation(int year, int month, long amountValue, int categoryIndex, const std::string& description);
	void EditOperation(int id, int year, int month, long amountValue, int categoryIndex, const std::string& description);
	void DeleteOperation(int id);

	void LoadProfile(const Profile& profile);
	void LoadDefaultProfile();

	void LoadCategory(const Category& category, int profileIndex);
	void LoadInternalCategory(int profileIndex);

	void LoadAccount(const BankAccount& account, int profileIndex);
	void LoadDefaultAccount(int profileIndex);

	void LoadOperation(const Operation& operation, int profileIndex, int accountIndex);

private:
	std::vector<Profile> m_profiles;

	int m_currentProfileIndex = 0;
};

