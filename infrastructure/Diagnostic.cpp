//
// Created by hseyi on 1.05.2026.
//

#include "Diagnostic.h"

Diagnostic::Diagnostic(int line, int column, const std::string &message, DiagnosticSeverity severity, const std::string &source, const std::string &ruleId, const std::string &codeSnippet) {
    this->line = line;
    this->column = column;
    this->message = message;
    this->severity = severity;
    this->source = source;
    this->ruleId = ruleId;
    this->codeSnippet = codeSnippet;
}

int Diagnostic::getLine() const {
    return line;
}

int Diagnostic::getColumn() const {
    return column;
}

std::string Diagnostic::getMessage() const {
    return message;
}

DiagnosticSeverity Diagnostic::getSeverity() const {
    return severity;
}

std::string Diagnostic::getSource() const {
    return source;
}

std::string Diagnostic::getRuleId() const {
    return ruleId;
}

std::string Diagnostic::getCodeSnippet() const {
    return codeSnippet;
}
