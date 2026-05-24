//
// Created by hseyi on 14.05.2026.
//

#include "FileLoader.h"
#include "FileValidator.h"
#include "ValidationResult.h"

#include <fstream>
#include <stdexcept>

SourceFile FileLoader::load(const std::string& path) const {
    std::string content = readFromDisk(path);

    std::size_t lastSlashPosition = path.find_last_of("/\\");// Windows, macOS ve Linux yolları için
    std::string fileName;

    if (lastSlashPosition == std::string::npos) {
        fileName = path;
    } else {
        fileName = path.substr(lastSlashPosition + 1);
    }

    SourceFile sourceFile(
    path,
    fileName,
    content,
    static_cast<long>(content.size())
    );

    FileValidator validator;
    ValidationResult result = validator.validate(sourceFile);

    if (!result.isValid()) {
        throw std::runtime_error(result.getErrorMessage());
    }

    return sourceFile;
}

std::string FileLoader::readFromDisk(const std::string& path) const {
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Dosya açılamadı: " + path);
    }

    std::string content;
    char ch;

    while (file.get(ch)) {
        content += ch;
    }


    return content;
}
