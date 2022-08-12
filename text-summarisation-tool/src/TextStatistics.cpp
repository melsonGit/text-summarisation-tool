#include "../inc/TextStatistics.h"

void TextStatistics::processWordStats(const std::string& wordToInsert, const bool& isStopWord)
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

void TextStatistics::printWordArchive() // temp func
{
	const int widthAp{ 15 };

	for (const auto& itr : this->mWordArchive)
		std::cout << std::left << std::setw(widthAp) << itr.second.getWord()
		<< "\t Word Freq: " << std::setw(widthAp) << itr.second.getWordFreq()
		<< "\t Word Freq Remov: " << std::setw(widthAp) << itr.second.getWordRemoveFreq()
		<< "\t Is Stop Word: " << std::setw(widthAp) << std::boolalpha << itr.second.getWordType() << '\n';
}

void TextStatistics::setLongestWord(const std::string& newLongestWord) { this->mLongestWord = newLongestWord; }

void TextStatistics::setShortestWord(const std::string& newShortestWord) { this->mShortestWord = newShortestWord; }

const std::string& TextStatistics::getLongestWord() const { return this->mLongestWord; }

const std::string& TextStatistics::getShortestWord() const { return this->mShortestWord; }

const int& TextStatistics::getTotWordsPreSum() const { return this->mTotWordsPreSum; }

const int& TextStatistics::getTotWordsPostSum() const { return this->mTotWordsPostSum; }