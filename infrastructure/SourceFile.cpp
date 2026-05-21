//
// Created by hseyi on 26.04.2026.
//

#include "SourceFile.h"

SourceFile::SourceFile(const std::string& filePath,const std::string& fileName,const std::string& content,long fileSize) {
    this->filePath = filePath;
    this->fileName = fileName;
    this->content = content;
    this->fileSize = fileSize;

    this->loadDate = std::chrono::system_clock::now();//şuanki zamanı atadık
}

std::string SourceFile::getContent() const{
    return content;
}


std::string SourceFile::getFileName() const{
    return fileName;
}

std::string SourceFile::getFilePath() const{
    return filePath;
}

long SourceFile::getFileSize() const{
    return fileSize;
}

bool SourceFile::isEmpty() const{
    return content.empty();
}
