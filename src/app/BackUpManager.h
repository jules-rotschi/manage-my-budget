#pragma once

#include <qtextstream.h>
#include <qdir.h>

#include "core/Profile.h"

void LoadBackUp(const std::string& backUpPath);
void BackUp(const std::string& backUpPath);

bool LoadProfilesBackUp(const QDir& backUpDirectory);
bool LoadCategoriesBackUp(const QDir& backUpDirectory, int profileIndex);
bool LoadAccountsBackUp(const QDir& backUpDirectory, int profileIndex);
bool LoadOperationsBackUp(const QDir& backUpDirectory, int profileIndex, int accountIndex);

void BackUpProfiles(const QDir& backUpDirectory);
void BackUpCategories(const QDir& backUpDirectory, const Profile& profile);
void BackUpAccounts(const QDir& backUpDirectory, const Profile& profile);
void BackUpOperations(const QDir& backUpDirectory, const Profile& profile, const BankAccount& account);

QTextStream& operator<<(QTextStream& stream, const Profile& profile);

QTextStream& operator<<(QTextStream& stream, const Category& category);

QTextStream& operator<<(QTextStream& stream, const BankAccount& account);

QTextStream& operator<<(QTextStream& stream, const Operation& operation);