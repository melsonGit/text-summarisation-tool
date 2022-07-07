#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <unordered_map>
#include <iterator>
#include <array>
#include <algorithm>

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
				std::cout << "File doesn't exist. Enter a valid file name:\n";

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
				std::cout << file + type << " already exists. Enter a new file name:\n";

		} while (true);
	}
	std::string selectInputFile(const std::string& directory, const std::string& type) const
	{
		std::cout << "Enter an input file name you wish to read from (file type is not required):\n";
		return this->userFindFile(directory, type);
	}
	std::string selectStopWordFile(const std::string& directory, const std::string& type) const
	{
		std::cout << "Enter stop words file name you wish to read from (file type is not required | stop words must be stored as lower case):\n";
		return this->userFindFile(directory, type);
	}
	std::string createOutputFile(const std::string& directory, const std::string& type) const
	{
		std::cout << "Enter output file name you wish to create (file type is not required):\n";
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
struct Word
{
private:

	// Our word (lower case for key functionality purposes)
	const std::string mWord{ "" };
	// How many times this word appears in prefiltered text
	int mWordFreq{};
	// How many times this word was removed for postfiltered text
	int mRemoveFreq{}; 
	// How many times this word appears in summarised text
	int mSummFreq{};
	// Determines word type
	const bool mIsStopWord{};

	Word() = delete;

public:

	// Normal word init
	Word(const std::string& wordToArchive, const bool& isStopWord, const int& initFreq)
		: mWord{ wordToArchive }, mWordFreq{ initFreq }, mIsStopWord{ isStopWord }{}
	// Stop word init
	Word(const std::string& stopWordToArchive, const bool& isStopWord, const int& initFreq, const int& initRemoveFreq)
		: mWord{ stopWordToArchive }, mWordFreq{ initFreq }, mRemoveFreq{ initRemoveFreq }, mIsStopWord{ isStopWord }{}

	void incrementWordFreq() { ++this->mWordFreq; }
	void incrementRemoveFreq() { ++this->mRemoveFreq; }
	void incrementSummFreq() { ++this->mSummFreq; }

	const std::string& getWord() const { return this->mWord; }
	const int& getWordFreq() const { return this->mWordFreq; }
	const int& getWordRemoveFreq() const { return this->mRemoveFreq; }
	const int& getWordSummFreq() const { return this->mSummFreq; }
	const bool& getWordType() const { return this->mIsStopWord; }

};
class TextStatistics
{
private:

	std::unordered_map<std::string, Word> mWordArchive{};
	std::array<std::string, 5> mTopFiveFreqWords{};
	std::array<std::string, 5> mTopFiveFreqStopWords{};
	std::string mLongestWord{ "" };
	std::string mShortestWord{ "" };
	int mTotWordsPreSum{};
	int mTotWordsPostSum{};

public:

	void processWordStats(const std::string& wordToInsert, const bool& isStopWord)
	{
		auto itr{ this->mWordArchive.find(wordToInsert) };

		// If we've not seen this word yet
		if (itr == this->mWordArchive.end())
		{
			// First instance of processing this word means we need to assign frequency to 1 in relevant vars
			const int firstWordInitFreq{ 1 };

			if (isStopWord)
			{
				Word newStopWord{ wordToInsert, isStopWord, firstWordInitFreq, firstWordInitFreq };
				this->mWordArchive.insert(std::pair(wordToInsert, newStopWord));
			}
			else
			{
				Word newNonStopWord{ wordToInsert, isStopWord, firstWordInitFreq };
				this->mWordArchive.insert(std::pair(wordToInsert, newNonStopWord));
			}
		}
		else // If we've already seen this word
		{
			if (isStopWord)
			{
				// Increment word freq / remove freq if it already exists in our map
				itr->second.incrementWordFreq();
				itr->second.incrementRemoveFreq();
			}
			else // Increment word freq if it already exists in our map
				itr->second.incrementWordFreq();
		}
	}
	void printWordArchive() // temp func
	{
		const int widthAp{ 15 };

		for (const auto& itr : this->mWordArchive)
			std::cout << std::left << std::setw(widthAp) << itr.second.getWord() 
			<< "\t Word Freq: " << std::setw(widthAp) << itr.second.getWordFreq()
			<< "\t Word Freq Remov: " << std::setw(widthAp) << itr.second.getWordRemoveFreq()
			<< "\t Is Stop Word: " << std::setw(widthAp) << std::boolalpha << itr.second.getWordType() << '\n';
	}

	void setLongestWord(const std::string& newLongestWord) { this->mLongestWord = newLongestWord; }
	void setShortestWord(const std::string& newShortestWord) { this->mShortestWord = newShortestWord; }

	const std::string& getLongestWord() const { return this->mLongestWord; }
	const std::string& getShortestWord() const { return this->mShortestWord; }
	const int& getTotWordsPreSum() const { return this->mTotWordsPreSum; }
	const int& getTotWordsPostSum() const { return this->mTotWordsPostSum; }
};
class TextParser
{
private:

	// Used by TextParser to collate and output relevant statistics
	TextStatistics mTextStatistics;

	// Summarisation Factor Variables
	const int mSummFactor{};
	int mCurrentSummFactor{};

	// Containers
	const std::unordered_map<std::string, std::string> mStopWordsList{};

	std::vector<std::string> mFirstFilterSentenceHolder{};
	std::vector<std::string> mFinalFilterSentenceHolder{};

	// Text Paths
	const std::string mInputFilePath{};
	const std::string mOutputFilePath{};

	// Text State Variables
	const std::string mPrefilteredText{};
	std::string mSummarisedText{};

	// Helper Functions
	bool hValidSummFactor(const int& factor) const { return !(factor < 1 || factor > 100); }
	bool hReachedSummFactorLimit() const { return this->mCurrentSummFactor == this->mSummFactor; }
	bool hIsFirstCharUpper(const std::string& word) const { return word.size() && std::isupper(word[0]); }
	bool hIsFirstSpecChar(const std::string& word) const { return (word.find_first_not_of("abcdefghijklmnopqrstuvwxyz01234567890") ? false : true); }
	//bool hIsLastSpecChar(const std::string& word) const { return (word.find_first_not_of("abcdefghijklmnopqrstuvwxyz01234567890", word.size()) ? false : true); }
	void hRemoveFirstChar(std::string& word) const { word.erase(word.begin()); }
	void hRemoveLastChar(std::string& word) const { if (word.empty()) return; else word.pop_back(); }
	int hRemainingSummFactor() { return this->mSummFactor - this->mCurrentSummFactor; }

	// Init Functions
	int setSummFactor()
	{
		std::cout << "Please enter a summarisation factor (SF) between 1% - 100%: ";

		int SF{};

		do
		{
			std::cin >> SF;

			if (this->hValidSummFactor(SF)) { std::cout << "SF valid.\n\n"; return SF; }
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
	std::string decapitaliseWord(const std::string& origWord)
	{
		std::string tempWord{ origWord };
		for (auto& c : tempWord)
			c = std::tolower(c);

		return tempWord;
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
		mStopWordsList{ this->populateStopWordMap(FileHandler.getStopWordsFilePath()) },
		mPrefilteredText{this->setPrefilteredText(FileHandler.getInputFilePath())},
		mInputFilePath{FileHandler.getInputFilePath()},
		mOutputFilePath{FileHandler.getOutputFilePath()}{}

	void summariseFile()
	{
		constexpr char delimiter{ '.' };
		std::ifstream outputFile{ this->mOutputFilePath };
		std::ifstream inputFile{ this->mInputFilePath };

		std::string prefilteredSentence{ "" };
		std::string postfilteredSentence{ "" };

		// First Filter Stage - Removing Stop Words & Archiving all words for the next filter stage
		// Grab sentence until we reach a fullstop
		while (std::getline(inputFile, prefilteredSentence, delimiter))
		{
			// Push this sentence into a stringstream
			std::istringstream stringStream{ prefilteredSentence };

			// StringStreams check token by token (tabs, whitespace, etc); therefore it will stop evaluating a string when we reach a e.g. whitespace 
			// Evaluate each word of the current stringstream
			do
			{
				std::string originalWord{ "" };
				std::string processedWord{ "" };
				bool isStopWord{};

				stringStream >> originalWord;

				// If first letter is upper case
				if (this->hIsFirstCharUpper(originalWord))
					// change to lower case and assign to processed word
					processedWord = this->decapitaliseWord(originalWord);
				else
					// no change needed and assign to processed word
					processedWord = originalWord;

				// !We want quotes to skip this section!
				// If word first char is special character, remove it
				if (this->hIsFirstSpecChar(processedWord))
					this->hRemoveFirstChar(processedWord);
				// If word last char is special character, remove it
				//if (this->hIsLastSpecChar(processedWord))
				//	this->hRemoveLastChar(processedWord);

				// Check if this word is in our stop word list
				if (this->mStopWordsList.contains(processedWord))
					isStopWord = true; // If the word is in our stop word list, don't append to our filtered sentence
				else
				{
					isStopWord = false;
					// If not, we append the word onto our post filtered sentence
					postfilteredSentence.append(originalWord + " ");
				}

				this->mTextStatistics.processWordStats(processedWord, isStopWord);


			} while (stringStream); // Continue until we reach the end of the stream

			// Once we've reach the end of the stringstream, remove whitespace at end of sentence and append full stop
			postfilteredSentence.erase(std::find_if(postfilteredSentence.rbegin(), postfilteredSentence.rend(), [](unsigned char ch) 
				{return !std::isspace(ch);}).base(), postfilteredSentence.end());

			postfilteredSentence.append(".");

			// Push post filter sentence into our sentence holder
			this->mFirstFilterSentenceHolder.push_back(postfilteredSentence);

			// Reset post filter sentence
			postfilteredSentence = "";
		}

		//	Final Filter Stage - WIP
		//	Repeat the following until SF is exceeded >
		//	{
		//	For each sentence, count the number of the words that matches the top word(most frequent) in the filtered word list.
		//  Find the sentence that has the highest number of occurrences of the most frequent word.
		//  If the length of words in the summary text added to the current selected sentence word length exceeds the summary word length limit the sentence is ignored.
		//	Else add the sentence to the summary text. Remove the word from the top of the frequency word list and the sentence from the listing of word sentences.
		//	}
		//	Output

		// print contents of sentence holder
		for (auto i{ 0 }; i < this->mFirstFilterSentenceHolder.size(); ++i)
			std::cout << this->mFirstFilterSentenceHolder[i] << '\n';

		std::cout << "\n\n\n";

		this->mTextStatistics.printWordArchive();
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
* (DONE) File - acts as file itself
* (DONE) FileHandler - open | close | edit files
* TextStatistics - collects following data:
				   least/most freq word | shortest/longest word | most removed/unremoved word | least/most freq stopWordFile encountered | summarisation factor |
*				   | total words pre and post summarisation
* TextParser - analyses text and outputs summarised text | uses TextStatistics to collect data | uses TextFilter to filter words using stopWordFile.txt
*/