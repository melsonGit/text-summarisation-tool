#include "../inc/TextParser.h"

// Private

// Helper Functions
bool TextParser::hValidSummFactor(const int& factor) const { return !(factor < 1 || factor > 100); }

bool TextParser::hReachedSummFactorLimit() const { return this->mCurrentSummFactor == this->mSummFactor; }

bool TextParser::hIsFirstCharUpper(const std::string& word) const { return word.size() && std::isupper(word[0]); }

bool TextParser::hIsSpecChar(const char& letter) const { return this->mStopCharList.contains(letter); }

void TextParser::hRemoveFirstChar(std::string& word) const { if (word.empty()) return; else word.erase(word.begin()); }

void TextParser::hRemoveLastChar(std::string& word) const { if (word.empty()) return; else word.pop_back(); }

int TextParser::hRemainingSummFactor() { return this->mSummFactor - this->mCurrentSummFactor; }

// Init Functions
int TextParser::setSummFactor()
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
std::unordered_map<std::string, std::string> TextParser::populateStopWordMap(const std::string& stopWordsFilePath)
{
	std::unordered_map<std::string, std::string> tempList{};
	std::ifstream stopWordFile{ stopWordsFilePath };
	std::string word{ "" };

	while (stopWordFile >> word)
		tempList[word] = word;

	stopWordFile.close();
	return tempList;
}
std::unordered_map<char, char> TextParser::populateCharWordMap(const std::string& stopCharFilePath)
{
	std::unordered_map<char, char> tempList{};
	std::ifstream stopCharFile{ stopCharFilePath };
	char word{ ' ' };

	while (stopCharFile >> word)
		tempList[word] = word;

	stopCharFile.close();
	return tempList;
}
std::string TextParser::setPrefilteredText(const std::string& prefilteredTextFilePath)
{
	// Push input stream buffer into single string from a file
	std::ifstream tempFile(prefilteredTextFilePath);
	std::string tempStr(std::istreambuf_iterator<char>{tempFile}, {});
	tempFile.close();
	return tempStr;
}
std::string TextParser::decapitaliseWord(const std::string& origWord)
{
	std::string tempWord{ origWord };
	for (auto& c : tempWord)
		c = std::tolower(c);

	return tempWord;
}
bool TextParser::isOnlyWhitespace(const std::string& word)
{
	if (!word.empty())
	{
		for (auto i{ 0 }; i < word.size(); ++i)
			if (!std::isspace(word[i]))
				return false;
	}
	return true;
}

// Public

void TextParser::summariseFile()
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

			// Initial isOnlyWhitespace() check
			if (!this->isOnlyWhitespace(originalWord))
			{
				// Then after, we check for quotes
				// Then after, we check for brackets
				// Both skip these sections

				// If first char is special character, remove it
				while (!originalWord.empty() && !this->hIsSpecChar(originalWord[0]))
					this->hRemoveFirstChar(originalWord);
				// If last char is special character, remove it
				while (!originalWord.empty() && !this->hIsSpecChar(originalWord[originalWord.size() - 1]))
					this->hRemoveLastChar(originalWord);

				// If first letter is upper case
				if (this->hIsFirstCharUpper(originalWord))
					// change to lower case and assign to processed word
					processedWord = this->decapitaliseWord(originalWord);
				else
					// no change needed and assign to processed word
					processedWord = originalWord;

				// Final isOnlyWhitespace() check
				if (!this->isOnlyWhitespace(processedWord))
				{
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
				}
			}

		} while (stringStream); // Continue until we reach the end of the stream

		// Once we've reach the end of the stringstream, remove whitespace at end of sentence and append full stop
		postfilteredSentence.erase(std::find_if(postfilteredSentence.rbegin(), postfilteredSentence.rend(), [](unsigned char ch)
			{return !std::isspace(ch); }).base(), postfilteredSentence.end());
		postfilteredSentence.append(".");

		// Push post filter sentence into our sentence holder
		this->mFirstFilterSentenceHolder.push_back(postfilteredSentence);

		// Reset post filter sentence
		postfilteredSentence = "";
	}

	//	Final Filter Stage
	//do
	//{
	//} while (true)

		//	Repeat the following until SF is exceeded >
		//	{
		//		For each sentence, count the number of the words that matches the top word(most frequent) in the filtered word list.
		//		Find the sentence that has the highest number of occurrences of the most frequent word.
		//		If the length of words in the summary text added to the current selected sentence word length exceeds the summary word length limit the sentence is ignored.
		//		Else add the sentence to the summary text. Remove the word from the top of the frequency word list and the sentence from the listing of word sentences.
		//	}

	// Temp fix to remove appended fullstop
	this->mFirstFilterSentenceHolder.pop_back();
	// print contents of sentence holder
	int numSentences{ 0 };
	for (auto i{ 0 }; i < this->mFirstFilterSentenceHolder.size(); ++i)
	{
		if (numSentences == 2)
		{
			std::cout << '\n';
			numSentences = -1;
		}

		std::cout << this->mFirstFilterSentenceHolder[i] << ' ';
		++numSentences;
	}

	std::cout << "\n\n\n";
	this->mTextStatistics.printWordArchive();
}