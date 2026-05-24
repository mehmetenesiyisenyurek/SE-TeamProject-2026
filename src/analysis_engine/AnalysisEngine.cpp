#include "AnalysisEngine.h"

AnalysisEngine::AnalysisEngine() {
}

AnalysisEngine::~AnalysisEngine() {
}

// Seçilen dosya için tüm analiz adımlarını işletir
AnalysisResult AnalysisEngine::runAnalysis(const std::string& filePath) {
    AnalysisResult result;
    result.addIssue("Örnek dosya analizi yapıldı: " + filePath);
    return result;
}
