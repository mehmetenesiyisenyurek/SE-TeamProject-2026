#include "HtmlExporter.h"
#include <fstream>

bool HtmlExporter::exportData(const AnalysisResult& result, const std::string& filePath) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        return false;
    }

    file << "<!DOCTYPE html>\n<html>\n<head><title>Analiz Raporu</title></head>\n<body>\n";
    file << "<h1>Skor: " << result.getScore() << "</h1>\n";
    file << "<ul>\n";
    
    for (const auto& issue : result.getIssues()) {
        file << "<li>" << issue << "</li>\n";
    }
    
    file << "</ul>\n</body>\n</html>\n";

    file.close();
    return true;
}
