//
// Created by hseyi on 12.05.2026.
//

#ifndef SE_TEAMPROJECT_2026_FILEVALIDATOR_H
#define SE_TEAMPROJECT_2026_FILEVALIDATOR_H

#include <string>
#include "SourceFile.h"
#include "ValidationResult.h"


class FileValidator {
public:

    ValidationResult validate(const SourceFile& file) const;

    bool checkExtension(const std::string& path) const;
    bool checkNotEmpty(const std::string& content) const;
    bool checkReadable(const std::string& path) const;
    bool checkNotBinary(const std::string& content) const;
};


#endif //SE_TEAMPROJECT_2026_FILEVALIDATOR_H
