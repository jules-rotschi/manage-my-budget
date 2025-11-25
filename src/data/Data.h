#pragma once

#include <string>

#include <qdatastream.h>
#include <qdir.h>

class Profile;
class BankAccount;
struct Operation;
struct Category;

void InitializeData(const QDir& saveInDirectory);
void InitializeData();

void ResetData();

void SaveData();

void SaveProfiles();
void SaveCategories(const Profile& profile);
void SaveAccounts(const Profile& profile);
void SaveOperations(const Profile& profile, const BankAccount& account);

void LoadData();

void LoadProfiles(bool isDataFromVersion1_0_0 = false);
void LoadCategories(int profileIndex, bool isDataFromVersion1_0_0 = false);
void LoadAccounts(int profileIndex);
void LoadOperations(int profileIndex, int accountIndex);

QDataStream& operator<<(QDataStream& stream, const Profile& profile);
QDataStream& operator>>(QDataStream& stream, Profile& profile);

QDataStream& operator<<(QDataStream& stream, const Category& category);
QDataStream& operator>>(QDataStream& stream, Category& category);

QDataStream& operator<<(QDataStream& stream, const BankAccount& account);
QDataStream& operator>>(QDataStream& stream, BankAccount& account);

QDataStream& operator<<(QDataStream& stream, const Operation& operation);
QDataStream& operator>>(QDataStream& stream, Operation& operation);