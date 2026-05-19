#pragma once

#include "AnalysisResult.h"
#include <string>

// Kod metriklerini hesaplar
class MetricCalculator {
public:
    void calculateMetrics(const std::string& sourceCode, AnalysisResult& result);
};
