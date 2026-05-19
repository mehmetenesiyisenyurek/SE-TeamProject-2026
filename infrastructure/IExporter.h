#pragma once

#include "AnalysisResult.h"
#include <string>

// Genel raporlama arayüzü
class IExporter {
public:
    virtual ~IExporter() = default;
    
    virtual bool exportData(const AnalysisResult& result, const std::string& filePath) = 0;
};
