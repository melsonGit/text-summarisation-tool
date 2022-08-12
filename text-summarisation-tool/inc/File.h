#pragma once
#ifndef FILE_H
#define FILE_H

#include <string>

class File
{
private:

	const std::string mName{};
	const std::string mDirectory{};
	const std::string mFileType{};
	const std::string mFilePath{};

public:

	File() = delete;

	File(const std::string& name, const std::string& directory, const std::string& fileType) :
		mName{ name }, mDirectory{ directory }, mFileType{ fileType }, mFilePath{ directory + name + fileType } {}

	const std::string& getFileName() const;
	const std::string& getDirectoryName() const;
	const std::string& getFileType() const;
	const std::string& getFilePath() const;
};
#endif