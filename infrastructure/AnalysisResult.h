#pragma once

#include <string>
#include <vector>

// Analiz sonuçlarını tutan veri modeli
class AnalysisResult {
public:
    AnalysisResult();
    ~AnalysisResult();

    void addIssue(const std::string& issue);
    void setScore(int score);
    
    const std::vector<std::string>& getIssues() const;
    int getScore() const;

private:
    std::vector<std::string> issues;
    int score;
};
