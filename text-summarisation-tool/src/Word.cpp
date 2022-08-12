#include "../inc/Word.h"

void Word::incrementWordFreq() { ++this->mWordFreq; }

void Word::incrementRemoveFreq() { ++this->mRemoveFreq; }

void Word::incrementSummFreq() { ++this->mSummFreq; }

const std::string& Word::getWord() const { return this->mWord; }

const int& Word::getWordFreq() const { return this->mWordFreq; }

const int& Word::getWordRemoveFreq() const { return this->mRemoveFreq; }

const int& Word::getWordSummFreq() const { return this->mSummFreq; }

const bool& Word::getWordType() const { return this->mIsStopWord; }