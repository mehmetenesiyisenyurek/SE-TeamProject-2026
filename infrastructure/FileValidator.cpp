//
// Created by hseyi on 12.05.2026.
//

#include "FileValidator.h"

#include <fstream>
#include <cctype>

ValidationResult FileValidator::validate(const SourceFile& file) const {
    //Dosya .c uzantılı mı
    if (!checkExtension(file.getFilePath())) {
        return {false, "Desteklenmeyen dosya formatı. Yalnızca .c dosyaları kabul edilir."};
    }

    //Dosya içeriği boş mu
    if (!checkNotEmpty(file.getContent())) {
        return {false, "Dosya içeriği bulunamadı."};
    }

    //Dosya okunabilir mi
    if (!checkReadable(file.getFilePath())) {
        return {false, "Dosya okunamıyor."};
    }

    //Dosya binary mi
    if (!checkNotBinary(file.getContent())) {
        return {false, "Dosya metin formatında değil."};
    }

    return {true, ""};
}

bool FileValidator::checkExtension(const std::string& path) const {
    if (path.size() < 2) {
        return false;
    }

    return path.substr(path.size() - 2) == ".c";
}

bool FileValidator::checkNotEmpty(const std::string& content) const {
    for (char ch : content) {
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            return true;
        }
    }

    return false;
}

bool FileValidator::checkReadable(const std::string& path) const {
    std::ifstream file(path);

    return file.is_open() && file.good();
}

bool FileValidator::checkNotBinary(const std::string& content) const {
    return content.find('\0') == std::string::npos;
}