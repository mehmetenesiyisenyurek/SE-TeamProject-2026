#include "AnalysisResult.h"

AnalysisResult::AnalysisResult() : score(0) {
}

AnalysisResult::~AnalysisResult() {
}

// Hata mesajını listeye ekler
void AnalysisResult::addIssue(const std::string& issue) {
    issues.push_back(issue);
}

void AnalysisResult::setScore(int s) {
    score = s;
}

const std::vector<std::string>& AnalysisResult::getIssues() const {
    return issues;
}

int AnalysisResult::getScore() const {
    return score;
}
