#pragma once

#include <vector>

#include <qdatastream.h>

#include <app/Profile.h>

class DataManager
{
public:
	std::vector<Profile> profiles;

	Profile& r_CurrentProfile();
	int GetCurrentProfileIndex() const;
	void SetCurrentProfileIndex(int index);

	void SaveProfiles() const;

	void LoadData();

	bool AddProfile(const Profile& profile);
	bool EditProfile(int index, const Profile& profile, const std::string& oldName);
	bool DeleteProfile(int index);

	std::string ToFileName(std::string str) const;

private:
	int currentProfileIndex = 0;

	void LoadProfiles();
	void LoadCategories();
	void LoadAccounts();
	void LoadOperations();

	void SaveCategories() const;
	void SaveAccounts() const;
	void SaveOperations() const;

	void LoadDefaultProfile();

	bool RemoveDirectory(const std::string& name) const;
};

QDataStream& operator<<(QDataStream& stream, const Profile& profile);
QDataStream& operator>>(QDataStream& stream, Profile& profile);

QDataStream& operator<<(QDataStream& stream, const BankAccount& account);
QDataStream& operator>>(QDataStream& stream, BankAccount& account);

QDataStream& operator<<(QDataStream& stream, const Operation& operation);
QDataStream& operator>>(QDataStream& stream, Operation& operation);

inline DataManager s_DataManager;