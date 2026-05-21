//
// Created by hseyi on 1.05.2026.
//

#ifndef SE_TEAMPROJECT_2026_DIAGNOSTIC_H
#define SE_TEAMPROJECT_2026_DIAGNOSTIC_H

#include <string>
#include "DiagnosticSeverity.h"


class Diagnostic {
private:
    int line;
    int column;
    std::string message;
    DiagnosticSeverity severity;
    std::string source;
    std::string ruleId;
    std::string codeSnippet;

public:
    //Tüm alanları alan constuctor
    Diagnostic(int line,int column,const std::string& message,DiagnosticSeverity severity,const std::string& source,const std::string& ruleId,const std::string& codeSnippet);

    [[nodiscard]] int getLine() const;
    [[nodiscard]] int getColumn() const;
    [[nodiscard]] std::string getMessage() const;
    [[nodiscard]] DiagnosticSeverity getSeverity() const;
    [[nodiscard]] std::string getSource() const;
    [[nodiscard]] std::string getRuleId() const;
    [[nodiscard]] std::string getCodeSnippet() const;
};



#endif //SE_TEAMPROJECT_2026_DIAGNOSTIC_H
