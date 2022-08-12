#pragma once
#ifndef FILE_HANDLER
#define FILE_HANDLER

#include "File.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

class FileHandler
{
private:

	const File mInputFile;
	const File mStopWordsFile;
	const File mStopCharFile;
	const File mOutputFile;

	bool checkFileExists(const std::string& fileName) const;
	std::string userFindFile(const std::string& directory, const std::string& type) const;
	std::string userCreateFile(const std::string& directory, const std::string& type) const;
	std::string selectInputFile(const std::string& directory, const std::string& type) const;
	std::string selectStopWordFile(const std::string& directory, const std::string& type) const;
	std::string selectStopCharFile(const std::string& directory, const std::string& type) const;
	std::string createOutputFile(const std::string& directory, const std::string& type) const;

public:

	FileHandler() = delete;

	FileHandler(const std::string& inputFileDir, const std::string& stopWordsDir, const std::string& stopCharDir, const std::string& outputFileDir, const std::string& type)
		:
		mInputFile{ this->selectInputFile(inputFileDir, type), inputFileDir, type },
		mStopWordsFile{ this->selectStopWordFile(stopWordsDir, type), stopWordsDir, type },
		mStopCharFile{ this->selectStopCharFile(stopCharDir, type), stopCharDir, type },
		mOutputFile{ this->createOutputFile(outputFileDir, type), outputFileDir, type }{}

	const std::string& getInputFilePath() const;
	const std::string& getStopWordsFilePath() const;
	const std::string& getStopCharFilePath() const;
	const std::string& getOutputFilePath() const;
};

#endif