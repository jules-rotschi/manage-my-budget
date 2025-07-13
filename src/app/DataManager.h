#pragma once

#include <vector>

#include <qdatastream.h>
#include <qdir.h>

#include "Profile.h"

class DataManager
{
public:
	const std::vector<Profile>& r_Profiles() const;

	const Profile& r_CurrentProfile();
	int GetCurrentProfileIndex() const;
	void SetCurrentProfileIndex(int index);

	void SetCurrentProfileCurrentAccountIndex(int index);

	void InitializeData();
	void InitializeData(const QDir& dataDirectory);

	void ResetData();

	void LoadBackUp(const std::string& backUpPath);
	void BackUp(const std::string& backUpPath);

	void AddProfile(const std::string& name);
	void RenameProfile(int index, const std::string& newName);
	void DeleteProfile(int index);

	void AddCategory(const std::string& category);
	void RenameCategory(int index, const std::string& newName);
	void DeleteCategory(int index);

	void AddAccount(const std::string& name, const std::string& type, int initialAmountValue);
	void EditAccount(int index, const std::string& name, const std::string& type, int initialAmountValue);
	void DeleteAccount(int index);

	void AddOperation(int year, int month, long amountValue, int categoryIndex, const std::string& description);
	void EditOperation(int id, int year, int month, long amountValue, int categoryIndex, const std::string& description);
	void DeleteOperation(int id);

private:
	std::vector<Profile> m_profiles;

	int m_currentProfileIndex = 0;

	QDir m_dataDirectory;
	std::string m_dataSubDirectoryName = "manage-my-budget-data";

	void LoadData();

	void LoadProfiles();
	void LoadCategories(Profile& profile) const;
	void LoadAccounts(Profile& profile) const;
	void LoadOperations(const Profile& profile, BankAccount& account) const;

	void SaveData() const;

	void SaveProfiles() const;
	void SaveCategories(const Profile& profile) const;
	void SaveAccounts(const Profile& profile) const;
	void SaveOperations(const Profile& profile, const BankAccount& account) const;

	bool LoadProfilesBackUp(const QDir& backUpDirectory);
	bool LoadCategoriesBackUp(const QDir& backUpDirectory, Profile& profile) const;
	bool LoadAccountsBackUp(const QDir& backUpDirectory, Profile& profile) const;
	bool LoadOperationsBackUp(const QDir& backUpDirectory, const Profile& profile, BankAccount& account) const;

	void BackUpProfiles(const QDir& backUpDirectory) const;
	void BackUpCategories(const QDir& backUpDirectory, const Profile& profile) const;
	void BackUpAccounts(const QDir& backUpDirectory, const Profile& profile) const;
	void BackUpOperations(const QDir& backUpDirectory, const Profile& profile, const BankAccount& account) const;

	void LoadDefaultProfile();

	std::string ToFileName(std::string strCopy) const;
	
	bool RemoveDirectory(const std::string& name) const;
};

QDataStream& operator<<(QDataStream& stream, const Profile& profile);
QDataStream& operator>>(QDataStream& stream, Profile& profile);

QDataStream& operator<<(QDataStream& stream, const BankAccount& account);
QDataStream& operator>>(QDataStream& stream, BankAccount& account);

QDataStream& operator<<(QDataStream& stream, const Operation& operation);
QDataStream& operator>>(QDataStream& stream, Operation& operation);

QTextStream& operator<<(QTextStream& stream, const Profile& profile);

QTextStream& operator<<(QTextStream& stream, const BankAccount& account);

QTextStream& operator<<(QTextStream& stream, const Operation& operation);

inline DataManager s_DataManager;