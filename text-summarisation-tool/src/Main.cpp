#include <iostream>
#include <filesystem>
#include <string>

#include "../inc/DirectoryInfo.h"

class File
{
private:

	const std::string mName{};
	const std::string mDirectory{};
	const std::string mFileType{};
	const std::string mFilePath{};

public:

	File(const std::string& name, const std::string& directory, const std::string& fileType) : 
		mName{ name }, mDirectory{ directory }, mFileType{ fileType }, mFilePath{ directory + name + fileType }  {}

	const std::string& getFileName() const { return this->mName; }
	const std::string& getDirectoryName() const { return this->mDirectory; }
	const std::string& getFileType() const { return this->mFileType; }
};

class FileHandler
{
private:

	const File mInputFile;
	const File mStopWordsFile;
	//const File mOutputFile;

	const bool checkFileExists(const std::string& fileName) { return std::filesystem::exists(fileName); }

	const std::string processFileName(const std::string& directory, const std::string& type)
	{
		std::string file{ "" };

		do
		{
			std::getline(std::cin, file);
			if (checkFileExists(directory + file + type)) { std::cout << file + type << " found.\n\n"; return file; }
			std::cout << "File doesn't exist. Enter a valid file name: ";

		} while (true);
	}

	const std::string selectInputFile(const std::string& directory, const std::string& type)
	{
		std::cout << "Enter an input file name you wish to read from (file type is not required): ";
		return processFileName(directory, type);
	}

	const std::string selectStopWordFile(const std::string& directory, const std::string& type)
	{
		std::cout << "Enter stop words file name you wish to read from (file type is not required): ";
		return processFileName(directory, type);
	}

public:

	FileHandler() = delete;

	FileHandler(const std::string& inputFileDir, const std::string& stopWordsDir, const std::string& type)
		: mInputFile{this->selectInputFile(inputFileDir, type), inputFileDir, type}, mStopWordsFile{this->selectStopWordFile(stopWordsDir, type), stopWordsDir, type} {}
};

int main()
{
	FileHandler FileHandler(DirectoryInfo::inputFolderDir, DirectoryInfo::stopWordsFolderDir, DirectoryInfo::fileType);

	return EXIT_SUCCESS;
}

/*
* Program Flow:
* 1. Prompt user to enter input file name they wish to read from (e.g. inFile.txt)
* 2. Prompt user fo enter summarisation factor (SF) | SF = totalSummarisedWordCount: (SF / 100) * totalInputFileWordCount. (IGNORE MODULE PDF AS IT HAS INCORRECT FORMULA TO CALCULATE PERCENTAGE)
*For example, if a text file has 107 words and the user inputs a 45% SF: (45 / 100) * 107 = we only want 48 (round down, we can't have half a word) words in our summarisation
* 3. Read inFile.txt
* 4. Read stopWords.txt
* 5. Process text from inFile.txt accordingly
* 6. Output summary text to new file (e.g. outFile.txt)
* 7. Display to console some appropriate statistics
*/

/*
* Classes:
* FileHandler - open | close | edit files
* TextStatistics - collects following data:
				   least/most freq word and letter | shortest/longest word | most removed/unremoved word | least/most freq stopWords encountered | summarisation factor |
*				   sentence with most/least words pre and post summarisation | total words pre and post summarisation
* TextSummariser - analyses text and outputs summarised text | uses TextStatistics to collect data | uses TextFilter to filter words using stopWords.txt
* TextFilter - filters text using stopWords.txt
*/