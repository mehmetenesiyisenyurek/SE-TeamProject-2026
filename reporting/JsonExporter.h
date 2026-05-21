#pragma once

#include "../infrastructure/IExporter.h"

// JSON formatında rapor üretir
class JsonExporter : public IExporter {
public:
    bool exportData(const AnalysisResult& result, const std::string& filePath) override;
};
