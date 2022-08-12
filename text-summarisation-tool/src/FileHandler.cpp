#include "../inc/FileHandler.h"

// Private

bool FileHandler::checkFileExists(const std::string& fileName) const { return std::filesystem::exists(fileName); }

std::string FileHandler::userFindFile(const std::string& directory, const std::string& type) const
{
	std::string file{ "" };

	do
	{
		std::getline(std::cin, file);

		if (checkFileExists(directory + file + type)) { std::cout << file + type << " found.\n\n"; return file; }
		else
			std::cout << "File doesn't exist. Enter a valid file name:\n";

	} while (true);
}

std::string FileHandler::userCreateFile(const std::string& directory, const std::string& type) const
{
	std::string file{ "" };

	do
	{
		std::getline(std::cin, file);
		if (!checkFileExists(directory + file + type))
		{
			std::ofstream newOutPutFile{ directory + file + type };
			newOutPutFile.close();
			std::cout << file + type << " created.\n\n";
			return file;
		}
		else
			std::cout << file + type << " already exists. Enter a new file name:\n";

	} while (true);
}

std::string FileHandler::selectInputFile(const std::string& directory, const std::string& type) const
{
	std::cout << "Enter an input file name you wish to read from (file type is not required):\n";
	return this->userFindFile(directory, type);
}

std::string FileHandler::selectStopWordFile(const std::string& directory, const std::string& type) const
{
	std::cout << "Enter stop words file name you wish to read from (file type is not required | stop words must be stored as lower case):\n";
	return this->userFindFile(directory, type);
}

std::string FileHandler::selectStopCharFile(const std::string& directory, const std::string& type) const
{
	std::cout << "Enter stop char file name you wish to read from (file type is not required | stop chars must be stored as lower case):\n";
	return this->userFindFile(directory, type);
}

std::string FileHandler::createOutputFile(const std::string& directory, const std::string& type) const
{
	std::cout << "Enter output file name you wish to create (file type is not required):\n";
	return this->userCreateFile(directory, type);
}


// Public

const std::string& FileHandler::getInputFilePath() const { return this->mInputFile.getFilePath(); }

const std::string& FileHandler::getStopWordsFilePath() const { return this->mStopWordsFile.getFilePath(); }

const std::string& FileHandler::getStopCharFilePath() const { return this->mStopCharFile.getFilePath(); }

const std::string& FileHandler::getOutputFilePath() const { return this->mOutputFile.getFilePath(); }