#pragma once

#include "../infrastructure/IExporter.h"
#include <memory>

// Raporlama sürecini yönetir
class ReportManager {
public:
    void setExporter(std::shared_ptr<IExporter> exporter);
    bool generateReport(const AnalysisResult& result, const std::string& filePath);

private:
    std::shared_ptr<IExporter> currentExporter;
};
