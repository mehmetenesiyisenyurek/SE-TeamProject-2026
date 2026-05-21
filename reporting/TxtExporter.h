#pragma once

#include "../infrastructure/IExporter.h"

// Metin formatında rapor üretir
class TxtExporter : public IExporter {
public:
    bool exportData(const AnalysisResult& result, const std::string& filePath) override;
};
