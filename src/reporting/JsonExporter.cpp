#include "JsonExporter.h"
#include <fstream>

bool JsonExporter::exportData(const AnalysisResult& result, const std::string& filePath) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        return false;
    }

    file << "{\n";
    file << "  \"skor\": " << result.getScore() << ",\n";
    file << "  \"bulgular\": [\n";
    
    const auto& issues = result.getIssues();
    for (size_t i = 0; i < issues.size(); ++i) {
        file << "    \"" << issues[i] << "\"";
        if (i < issues.size() - 1) {
            file << ",";
        }
        file << "\n";
    }
    
    file << "  ]\n";
    file << "}\n";

    file.close();
    return true;
}
