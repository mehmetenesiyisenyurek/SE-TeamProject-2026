//
// Created by hseyi on 17.05.2026.
//

#include "SeverityClassifier.h"

std::string SeverityClassifier::getColorCode(DiagnosticSeverity severity) const {
    switch (severity) {
        case DiagnosticSeverity::CRITICAL:
            return "#FF0000";
        case DiagnosticSeverity::WARNING:
            return "#FFA500";
        case DiagnosticSeverity::INFO:
            return "#2196F3";
        default:
            return "#2196F3";
    }

}

std::string SeverityClassifier::getLabel(DiagnosticSeverity severity) const {
    switch (severity) {
        case DiagnosticSeverity::CRITICAL:
            return "Kritik";
        case DiagnosticSeverity::WARNING:
            return "Uyarı";
        case DiagnosticSeverity::INFO:
            return "Bilgi";
        default:
            return "Bilgi";
    }
}

DiagnosticSeverity SeverityClassifier::classify(const Diagnostic& diagnostic) const {
    std::string source = diagnostic.getSource();
    std::string ruleId = diagnostic.getRuleId();
    if (source == "syntax") {
        return DiagnosticSeverity::CRITICAL;
    }

    //Pointer ve bellek güvenliği kuralları
    if (ruleId == "R008" || ruleId == "R010" || ruleId == "R012") {
        return DiagnosticSeverity::CRITICAL;
    }

    if (ruleId == "R011") {
        return DiagnosticSeverity::INFO;
    }

    if (ruleId == "R001" || ruleId == "R002" || ruleId == "R003" || ruleId == "R004" || ruleId == "R005" || ruleId == "R006" || ruleId == "R007" || ruleId == "R009") {
        return DiagnosticSeverity::WARNING;
    }

    //Bilinmeyen veya bilgilendirici durumlar
    return DiagnosticSeverity::INFO;
}
