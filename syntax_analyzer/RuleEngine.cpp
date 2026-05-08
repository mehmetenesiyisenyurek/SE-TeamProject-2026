#include "RuleEngine.h"

#include "UnusedVariableRule.h"
#include "UninitializedVarRule.h"
#include "UninitializedPointerRule.h"

/*
 * RuleEngine oluşturulduğunda varsayılan analiz kurallarını ekler.
 */
RuleEngine::RuleEngine() {
    registerDefaultRules();
}

/*
 * Sistemin varsayılan kurallarını oluşturur ve RuleEngine'e kaydeder.
 */
void RuleEngine::registerDefaultRules() {
    clearRules();

    ownedRules.push_back(std::make_unique<UnusedVariableRule>());
    rules.push_back(ownedRules.back().get());

    ownedRules.push_back(std::make_unique<UninitializedVarRule>());
    rules.push_back(ownedRules.back().get());

    ownedRules.push_back(std::make_unique<UninitializedPointerRule>());
    rules.push_back(ownedRules.back().get());
}

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

        if (diagnostics.empty()) {
            continue;
        }

        allDiagnostics.insert(
            allDiagnostics.end(),
            diagnostics.begin(),
            diagnostics.end()
        );
    }

    return allDiagnostics;
}
/*
 * Eklenen analiz kuralı sayısını döndürür.
 */
int RuleEngine::getRuleCount() const {
    return static_cast<int>(rules.size());
}

/*
 * Sistemde kayıtlı tüm analiz kurallarını temizler.
 */
void RuleEngine::clearRules() {

    rules.clear();
    ownedRules.clear();
}

/*
 * Kritik seviyedeki diagnostic sayısını döndürür.
 */
int RuleEngine::countCriticalDiagnostics(
    const std::vector<Diagnostic>& diagnostics
) const {

    int count = 0;

    for (const Diagnostic& diagnostic : diagnostics) {

        if (diagnostic.getSeverity() == DiagnosticSeverity::CRITICAL) {
            count++;
        }
    }

    return count;
}