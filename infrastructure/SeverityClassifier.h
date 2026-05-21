//
// Created by hseyi on 17.05.2026.
//

#ifndef SE_TEAMPROJECT_2026_SEVERITYCLASSIFIER_H
#define SE_TEAMPROJECT_2026_SEVERITYCLASSIFIER_H

#include <string>

#include "Diagnostic.h"
#include "DiagnosticSeverity.h"

class SeverityClassifier {
public:
    DiagnosticSeverity classify(const Diagnostic& diagnostic) const;
    std::string getColorCode(DiagnosticSeverity severity) const;
    std::string getLabel(DiagnosticSeverity severity) const;
};



#endif //SE_TEAMPROJECT_2026_SEVERITYCLASSIFIER_H
