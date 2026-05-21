#pragma once

#include "../infrastructure/IExporter.h"

// HTML formatında rapor üretir
class HtmlExporter : public IExporter {
public:
    bool exportData(const AnalysisResult& result, const std::string& filePath) override;
};
