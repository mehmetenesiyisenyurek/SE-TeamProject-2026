#include "RuleEngine.h"

/*
 * Sisteme yeni bir analiz kuralı ekler.
 * Null pointer gelirse kural listeye eklenmez.
 */
void RuleEngine::addRule(IRule* rule) {
    if (rule == nullptr) {
        return;
    }

    rules.push_back(rule);
}

/*
 * Tüm kuralları çalıştırır ve bulunan
 * diagnostikleri tek listede toplar.
 */
std::vector<Diagnostic> RuleEngine::analyzeAll(const ASTNode& ast) {

    std::vector<Diagnostic> allDiagnostics;

    for (IRule* rule : rules) {
        if (rule == nullptr) {
            continue;
        }

        std::vector<Diagnostic> diagnostics = rule->check(ast);

        allDiagnostics.insert(
            allDiagnostics.end(),
            diagnostics.begin(),
            diagnostics.end()
        );
    }

    return allDiagnostics;
}