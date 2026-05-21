#include "TxtExporter.h"
#include <fstream>

bool TxtExporter::exportData(const AnalysisResult& result, const std::string& filePath) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        return false;
    }

    file << "Skor: " << result.getScore() << "\n";
    file << "Bulgular:\n";
    
    for (const auto& issue : result.getIssues()) {
        file << "- " << issue << "\n";
    }

    file.close();
    return true;
}
