#pragma once
#ifndef WORD_H
#define WORD_H

#include <string>

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

	void incrementWordFreq();
	void incrementRemoveFreq();
	void incrementSummFreq();

	const std::string& getWord() const;
	const int& getWordFreq() const;
	const int& getWordRemoveFreq() const;
	const int& getWordSummFreq() const;
	const bool& getWordType() const;

};
#endif