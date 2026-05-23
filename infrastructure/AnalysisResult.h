#ifndef ANALYSIS_RESULT_H
#define ANALYSIS_RESULT_H

#include <string>
#include <vector>

class AnalysisResult {
private:
    int score;
    std::vector<std::string> issues;

public:
    AnalysisResult() : score(100) {}

    void addIssue(const std::string& issue) {
        issues.push_back(issue);
        if (score > 0) score -= 5;
    }

    int getScore() const { return score; }
    const std::vector<std::string>& getIssues() const { return issues; }
};

#endif
