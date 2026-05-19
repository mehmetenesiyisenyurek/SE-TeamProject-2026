#include "ReportManager.h"

void ReportManager::setExporter(std::shared_ptr<IExporter> exporter) {
    currentExporter = exporter;
}

// Seçili strateji ile rapor oluşturur
bool ReportManager::generateReport(const AnalysisResult& result, const std::string& filePath) {
    if (currentExporter) {
        return currentExporter->exportData(result, filePath);
    }
    return false;
}
