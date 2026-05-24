//
// Created by hseyi on 8.05.2026.
//

#include "Preprocessor.h"

Preprocessor::Preprocessor() {
    this->rawCode = "";//Orijinal kaynak kod
    this->cleanedCode = "";//Yorumlardan temizlenmiş kod
    this->commentCount = 0;//Yorum satır sayısı
}

std::string Preprocessor::process(const std::string &code) {
    this->commentCount = 0;
    this->rawCode = code;

    //Once /* */ bu yorum satırları temizlenir
    std::string tempCode = removeMultiLineComments(this->rawCode);
    //Sonra // tekli yorum satırları temizlenir
    this->cleanedCode = removeSingleLineComments(tempCode);

    return this->cleanedCode;
}

int Preprocessor::getCommentLineCount() const {
    return this->commentCount;
}


std::string Preprocessor::removeSingleLineComments(const std::string& code) {
    std::string result;
    bool inString = false;
    for (size_t i = 0; i < code.length(); ++i) {
        if (code[i] == '"' && (i == 0 || code[i-1] != '\\')) {
            inString = !inString;
            result += code[i];
        }
        else if (!inString && i + 1 < code.length() && code[i] == '/' && code[i+1] == '/') {
            this->commentCount++;
            while (i < code.length() && code[i] != '\n') {
                i++;
            }
            if (i < code.length() && code[i] == '\n') {
                result += '\n';
            }
        }
        else {
            result += code[i];
        }
    }
    return result;
}

std::string Preprocessor::removeMultiLineComments(const std::string& code) {
    std::string result;
    bool inString = false;
    bool inComment = false;
    for (size_t i = 0; i < code.length(); ++i) {
        if (!inComment && code[i] == '"' && (i == 0 || code[i-1] != '\\')) {
            inString = !inString;
            result += code[i];
        }
        else if (!inString && !inComment && i + 1 < code.length() && code[i] == '/' && code[i+1] == '*') {
            inComment = true;
            this->commentCount++;
            i++;
        }
        else if (inComment && i + 1 < code.length() && code[i] == '*' && code[i+1] == '/') {
            inComment = false;
            i++;
        }
        else if (inComment) {
            if (code[i] == '\n') {
                result += '\n';
                this->commentCount++;
            }
        }
        else {
            result += code[i];
        }
    }
    return result;
}

bool Preprocessor::isInsideStringLiteral(const std::string& code,int position) {
    bool inString = false;

    for (int i=0; i<=position && i< code.length(); ++i) {
        if (code[i] == '"' && (i == 0 || code[i-1] != '\\')) {
            inString = !inString;
        }
    }
    return inString;
}