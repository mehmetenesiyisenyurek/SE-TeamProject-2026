#include "RuleEngine.h"

/*
 * Sisteme yeni bir analiz kuralı ekler.
 */
void RuleEngine::addRule(IRule* rule) {
    rules.push_back(rule);
}

/*
 * Tüm kuralları çalıştırır ve bulunan
 * diagnostikleri tek listede toplar.
 */
std::vector<Diagnostic> RuleEngine::analyzeAll(const ASTNode& ast) {

    std::vector<Diagnostic> allDiagnostics;

    for (IRule* rule : rules) {

        std::vector<Diagnostic> diagnostics = rule->check(ast);

        allDiagnostics.insert(
            allDiagnostics.end(),
            diagnostics.begin(),
            diagnostics.end()
        );
    }

    return allDiagnostics;
}