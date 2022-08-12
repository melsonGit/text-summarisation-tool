#pragma once
#ifndef TEXT_PARSER
#define TEXT_PARSER

#include "FileHandler.h"
#include "TextStatistics.h"
#include "SummUtils.h"

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>

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
	const std::unordered_map<char, char> mStopCharList{};
	std::vector<std::string> mFirstFilterSentenceHolder{};
	std::vector<std::string> mFinalFilterSentenceHolder{};

	// Text Paths
	const std::string mInputFilePath{};
	const std::string mOutputFilePath{};

	// Text State Variables
	const std::string mPrefilteredText{};
	std::string mSummarisedText{};

	// Helper Functions
	bool hValidSummFactor(const int& factor) const;
	bool hReachedSummFactorLimit() const;
	bool hIsFirstCharUpper(const std::string& word) const;
	bool hIsSpecChar(const char& letter) const;
	void hRemoveFirstChar(std::string& word) const;
	void hRemoveLastChar(std::string& word) const;
	int hRemainingSummFactor();

	// Init Functions
	int setSummFactor();
	std::unordered_map<std::string, std::string> populateStopWordMap(const std::string& stopWordsFilePath);
	std::unordered_map<char, char> populateCharWordMap(const std::string& stopCharFilePath);
	std::string setPrefilteredText(const std::string& prefilteredTextFilePath);
	std::string decapitaliseWord(const std::string& origWord);
	bool isOnlyWhitespace(const std::string& word);

	// Parsing Functions - Impl. WIP
	void readSentence() {}
	void filterSentence() {}
	void writeParsedSentence() {}
	void updateLiveSummFactor() {}

public:

	TextParser() = delete;

	TextParser(const FileHandler& FileHandler)
		:
		mSummFactor{ this->setSummFactor() },
		mStopWordsList{ this->populateStopWordMap(FileHandler.getStopWordsFilePath()) },
		mStopCharList{ this->populateCharWordMap(FileHandler.getStopCharFilePath()) },
		mPrefilteredText{ this->setPrefilteredText(FileHandler.getInputFilePath()) },
		mInputFilePath{ FileHandler.getInputFilePath() },
		mOutputFilePath{ FileHandler.getOutputFilePath() }{}

	void summariseFile();
};
#endif