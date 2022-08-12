#pragma once
#ifndef TEXT_STATISTICS
#define TEXT_STATISTICS

#include "Word.h"

#include <array>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>

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

	void processWordStats(const std::string& wordToInsert, const bool& isStopWord);

	void printWordArchive();

	void setLongestWord(const std::string& newLongestWord);
	void setShortestWord(const std::string& newShortestWord);

	const std::string& getLongestWord() const;
	const std::string& getShortestWord() const;
	const int& getTotWordsPreSum() const;
	const int& getTotWordsPostSum() const;
};
#endif