#include "../inc/File.h"

const std::string& File::getFileName() const { return this->mName; }

const std::string& File::getDirectoryName() const { return this->mDirectory; }

const std::string& File::getFileType() const { return this->mFileType; }

const std::string& File::getFilePath() const { return this->mFilePath; }