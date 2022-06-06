#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>

#include "../inc/DirectoryInfo.h"

void clearInputStream()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

class File
{
private:

	const std::string mName{};
	const std::string mDirectory{};
	const std::string mFileType{};
	const std::string mFilePath{};

public:

	std::ifstream mOFile{};
	std::ofstream mIFile{};

	File() = delete;

	File(const std::string& name, const std::string& directory, const std::string& fileType) :
		mName{ name }, mDirectory{ directory }, mFileType{ fileType }, mFilePath{ directory + name + fileType }, mOFile{ mFilePath.c_str() }, mIFile{ mFilePath.c_str() }  {}

	~File() { mIFile.close(); mOFile.close(); }

	const std::string& getFileName() const { return this->mName; }
	const std::string& getDirectoryName() const { return this->mDirectory; }
	const std::string& getFileType() const { return this->mFileType; }
	const std::string& getFilePath() const { return this->mFilePath; }
};
class FileHandler
{
private:

	const File mInputFile;
	const File mStopWordsFile;
	const File mOutputFile;

	bool checkFileExists(const std::string& fileName) const { return std::filesystem::exists(fileName); }
	std::string userFindFile(const std::string& directory, const std::string& type) const
	{
		std::string file{ "" };

		do
		{
			std::getline(std::cin, file);

			if (checkFileExists(directory + file + type)) { std::cout << file + type << " found.\n\n"; return file; }
			else
				std::cout << "File doesn't exist. Enter a valid file name: ";

		} while (true);
	}
	std::string userCreateFile(const std::string& directory, const std::string& type) const
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
				std::cout << file + type << " already exists. Enter a new file name: ";

		} while (true);
	}
	std::string selectInputFile(const std::string& directory, const std::string& type) const
	{
		std::cout << "Enter an input file name you wish to read from (file type is not required): ";
		return userFindFile(directory, type);
	}
	std::string selectStopWordFile(const std::string& directory, const std::string& type) const
	{
		std::cout << "Enter stop words file name you wish to read from (file type is not required): ";
		return this->userFindFile(directory, type);
	}
	std::string createOutputFile(const std::string& directory, const std::string& type) const
	{
		std::cout << "Enter output file name you wish to create (file type is not required): ";
		return this->userCreateFile(directory, type);
	}

public:

	FileHandler() = delete;

	FileHandler(const std::string& inputFileDir, const std::string& stopWordsDir, const std::string& outputFileDir, const std::string& type)
		: 
		mInputFile{this->selectInputFile(inputFileDir, type), inputFileDir, type}, 
		mStopWordsFile{this->selectStopWordFile(stopWordsDir, type), stopWordsDir, type},
		mOutputFile{ this->createOutputFile(outputFileDir, type), outputFileDir, type }{}
};
class TextSummariser
{
private:

	const int summFactor{};

	bool validSummFactor(const int& factor) const { return !(factor < 1 || factor > 100); }
	int createSummFactor()
	{
		std::cout << "Please enter a summarisation factor (SF) between 1% - 100%: ";

		int SF{};

		do 
		{
			std::cin >> SF;

			if (validSummFactor(SF)) { std::cout << "SF valid.\n\n"; return SF; }
			else
				std::cout << "SF outside specified range. Enter a valid SF: ";

			clearInputStream();
		}
		while (true);
	}

public:

	TextSummariser() : summFactor{ this->createSummFactor() } {}

};

int main()
{
	FileHandler FileHandler(DirectoryInfo::inputFolderDir, DirectoryInfo::stopWordsFolderDir, DirectoryInfo::outputFolderDir, DirectoryInfo::fileType);
	TextSummariser TextSummariser;

	return EXIT_SUCCESS;
}

/*
* Program Flow:
*  (DONE) 1. Prompt user to enter input file name they wish to read from (e.g. inFile.txt)
*  (DONE)  2. Prompt user fo enter summarisation factor (SF) | SF = totalSummarisedWordCount: (SF / 100) * totalInputFileWordCount. (IGNORE MODULE PDF AS IT HAS INCORRECT FORMULA TO CALCULATE PERCENTAGE)
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