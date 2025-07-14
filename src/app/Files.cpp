#include "Files.h"

#include <algorithm>

#include <qdir.h>

std::string ToFileName(std::string strCopy)
{
	std::transform(strCopy.begin(), strCopy.end(), strCopy.begin(), tolower);

	std::replace(strCopy.begin(), strCopy.end(), ' ', '-');

	for (size_t i = 0; i < strCopy.size(); i++) {
		char c = strCopy[i];

		if ((c < 97 || c > 122) && c != '-') {
			strCopy.erase(i, 1);
			i--;
		}
	}

	return strCopy;
}

bool RemoveDirectory(const std::string& name)
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