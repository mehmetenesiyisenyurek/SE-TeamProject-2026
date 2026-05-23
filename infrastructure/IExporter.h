#ifndef IEXPORTER_H
#define IEXPORTER_H

#include <string>
#include "AnalysisResult.h"

class IExporter {
public:
    virtual ~IExporter() = default;
    virtual bool exportData(const AnalysisResult& result, const std::string& filePath) = 0;
};

#endif
