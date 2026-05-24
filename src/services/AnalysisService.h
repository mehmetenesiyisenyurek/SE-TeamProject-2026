#ifndef CODESENTINEL_ANALYSIS_SERVICE_H
#define CODESENTINEL_ANALYSIS_SERVICE_H

#include <string>
#include <vector>

#include "infrastructure/CodeMetric.h"
#include "infrastructure/Diagnostic.h"

struct AnalysisOutput {
    bool success = false;
    std::string errorMessage;
    std::string filePath;
    std::string fileName;
    std::string rawCode;
    std::string cleanCode;
    long fileSize = 0;
    int commentLineCount = 0;
    int ruleCount = 0;
    std::vector<Diagnostic> diagnostics;
    std::vector<CodeMetric> metrics;
};

class AnalysisService {
public:
    AnalysisOutput analyzeFile(const std::string& filePath) const;
};

std::string severityToText(DiagnosticSeverity severity);

#endif
