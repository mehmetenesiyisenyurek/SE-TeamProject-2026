#pragma once

#include <string>

// Dosya yükleyici arayüzü
class FileLoader {
public:
    virtual ~FileLoader() = default;
    virtual std::string loadFile(const std::string& filePath) = 0;
};
