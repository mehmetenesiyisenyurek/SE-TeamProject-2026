//
// Created by hseyi on 14.05.2026.
//

#ifndef SE_TEAMPROJECT_2026_FILELOADER_H
#define SE_TEAMPROJECT_2026_FILELOADER_H

#include <string>

#include "SourceFile.h"


class FileLoader {
public:
    SourceFile load(const std::string& path) const;

private:
    std::string readFromDisk(const std::string& path) const;
};


#endif //SE_TEAMPROJECT_2026_FILELOADER_H
