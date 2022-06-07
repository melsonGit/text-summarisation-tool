#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <unordered_map>
#include <iterator>

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

	File() = delete;

	File(const std::string& name, const std::string& directory, const std::string& fileType) :
		mName{ name }, mDirectory{ directory }, mFileType{ fileType }, mFilePath{ directory + name + fileType } {}

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
		return this->userFindFile(directory, type);
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

	const std::string& getInputFilePath() const { return this->mInputFile.getFilePath(); }
	const std::string& getStopWordsFilePath() const { return this->mStopWordsFile.getFilePath(); }
	const std::string& getOutputFilePath() const { return this->mOutputFile.getFilePath(); }
};
class TextParser
{
private:

	// Summarisation Factor Variables
	const int mSummFactor{};
	int mCurrentSummFactor{};

	// Containers
	const std::unordered_map<std::string, std::string> mStopWordsMap{};
	std::vector<std::string> mSentenceHolder{};

	// Text Paths
	const std::string mInputFilePath{};
	const std::string mOutputFilePath{};

	// Text State Variables
	const std::string mPrefilteredText{};
	std::string mSummarisedText{};

	// Helper Functions
	bool validSummFactor(const int& factor) const { return !(factor < 1 || factor > 100); }
	bool reachedSummFactorLimit() const { return this->mCurrentSummFactor == this->mSummFactor; }
	int remainingSummFactor() { return this->mSummFactor - this->mCurrentSummFactor; }

	// Init Functions
	int setSummFactor()
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
		} while (true);
	}
	std::unordered_map<std::string, std::string> populateStopWordMap(const std::string& stopWordsFilePath)
	{
		std::unordered_map<std::string, std::string> tempList{};
		std::ifstream stopWordFile{ stopWordsFilePath };
		std::string word{ "" };

		while (stopWordFile >> word)
			tempList[word] = word;

		stopWordFile.close();
		return tempList;
	}
	std::string setPrefilteredText(const std::string& prefilteredTextFilePath)
	{
		// Push input stream buffer into single string from a file
		std::ifstream tempFile(prefilteredTextFilePath);
		std::string tempStr(std::istreambuf_iterator<char>{tempFile}, {});
		tempFile.close();
		return tempStr;
	}

	// Parsing Functions - Impl. WIP
	void readSentence(){}
	void filterSentence(){}
	void writeParsedSentence(){}
	void updateLiveSummFactor(){}

public:

	TextParser() = delete;

	TextParser(const FileHandler& FileHandler) 
		: 
		mSummFactor{ this->setSummFactor() },
		mStopWordsMap{ this->populateStopWordMap(FileHandler.getStopWordsFilePath()) },
		mPrefilteredText{this->setPrefilteredText(FileHandler.getInputFilePath())},
		mInputFilePath{FileHandler.getInputFilePath()},
		mOutputFilePath{FileHandler.getOutputFilePath()}{}

	void summariseFile()
	{

		// For next impl. we need to split a delimited string into multiple strings..
		// ..... removing stopwords from each substring before inserting it into out mSentenceHolder and then returning back to where we ended on the last sub string

		constexpr char delimiter{ '.' };
		std::ifstream outputFile{ this->mOutputFilePath };
		std::ifstream inputFile{ this->mInputFilePath };

		std::string prefilteredSentence{ "" };
		std::string postfilteredSentence{ "" };
		std::string summarisedSentence{ "" };

		// Grab sentence until we reach a fullstop
		while (std::getline(inputFile, prefilteredSentence, delimiter))
		{
			// Push this sentence into a stringstream
			std::istringstream stringStream{ prefilteredSentence };

			// Evaluate each word of the current stringstream
			do
			{
				std::string word{ "" };
				stringStream >> word;

				// Check if this word is in our stop word list
				if (!this->mStopWordsMap.contains(word))
					// If not, we append the word onto our post filtered sentence
					postfilteredSentence.append(word + " ");
				// If so, we just skip the word and don't append to our filtered sentence

			} while (stringStream); // Continue until we reach the end of the stream

			// Once we've reach teh end of the stringstream, remove whitespace at end of sentence and append full stop
			postfilteredSentence.erase(std::find_if(postfilteredSentence.rbegin(), postfilteredSentence.rend(), [](unsigned char ch) {
				return !std::isspace(ch);
				}).base(), postfilteredSentence.end());
			postfilteredSentence.append(".");

			// Push post filter sentence into our sentence holder
			this->mSentenceHolder.push_back(postfilteredSentence);

			// Reset post filter sentence
			postfilteredSentence = "";
		}

		// print contents of sentence holder
		for (auto i{ 0 }; i < mSentenceHolder.size(); ++i)
			std::cout << mSentenceHolder[i] << '\n';
	}
};

int main()
{
	FileHandler FileHandler(DirectoryInfo::inputFolderDir, DirectoryInfo::stopWordsFolderDir, DirectoryInfo::outputFolderDir, DirectoryInfo::fileType);

	TextParser TextParser(FileHandler);
	TextParser.summariseFile();

	return EXIT_SUCCESS;
}

/*
* Program Flow:
*  (DONE) 1. Prompt user to enter input file name they wish to read from (e.g. inFile.txt)
*  (DONE) 2. Prompt user fo enter summarisation factor (SF) | SF = totalSummarisedWordCount: (SF / 100) * totalInputFileWordCount. (IGNORE MODULE PDF AS IT HAS INCORRECT FORMULA TO CALCULATE PERCENTAGE)
*For example, if a text file has 107 words and the user inputs a 45% SF: (45 / 100) * 107 = we only want 48 (round down, we can't have half a word) words in our summarisation
*  (DONE) 3. Read inFile.txt
*  (DONE) 4. Read stopWordFile.txt
* 5. Process text from inFile.txt accordingly
* 6. Output summary text to new file (e.g. outFile.txt)
* 7. Display to console some appropriate statistics
*/

/*
* Classes:
* File - acts as file itself
* FileHandler - open | close | edit files
* TextStatistics - collects following data:
				   least/most freq word and letter | shortest/longest word | most removed/unremoved word | least/most freq stopWordFile encountered | summarisation factor |
*				   sentence with most/least words pre and post summarisation | total words pre and post summarisation
* TextParser - analyses text and outputs summarised text | uses TextStatistics to collect data | uses TextFilter to filter words using stopWordFile.txt
*/