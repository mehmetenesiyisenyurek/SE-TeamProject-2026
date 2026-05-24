#pragma once

#include <string>
#include "../infrastructure/AnalysisResult.h"

// Tüm analiz sürecini yöneten merkezi motor
class AnalysisEngine {
public:
    AnalysisEngine();
    ~AnalysisEngine();

    AnalysisResult runAnalysis(const std::string& filePath);
};
