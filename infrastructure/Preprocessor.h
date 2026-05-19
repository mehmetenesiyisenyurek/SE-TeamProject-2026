#pragma once

#include <string>

// Analiz öncesi kodu hazırlar
class Preprocessor {
public:
    std::string process(const std::string& sourceCode);
};
