#include "RuleEngine.h"

#include "UnusedVariableRule.h"
#include "UninitializedVarRule.h"
#include "UninitializedPointerRule.h"
#include "../lexer/MagicNumberRule.h"
#include "../lexer/FunctionLengthRule.h"
#include "../lexer/GotoUsageRule.h"
#include "../parser/InfiniteLoopRule.h"
#include "../pointer/NullDereferenceRule.h"
#include "../pointer/UseAfterFreeRule.h"
#include "../pointer/MemoryLeakRule.h"
#include "../pointer/DoubleFreeRule.h"
#include "../pointer/DanglingPointerRule.h"

/*
 * RuleEngine oluşturulduğunda bu modüle ait varsayılan analiz kurallarını ekler.
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
    addRule(ownedRules.back().get());

    ownedRules.push_back(std::make_unique<UninitializedVarRule>());
    addRule(ownedRules.back().get());

    ownedRules.push_back(std::make_unique<UninitializedPointerRule>());
    addRule(ownedRules.back().get());

    ownedRules.push_back(std::make_unique<MagicNumberRule>());
    addRule(ownedRules.back().get());

    ownedRules.push_back(std::make_unique<FunctionLengthRule>());
    addRule(ownedRules.back().get());

    ownedRules.push_back(std::make_unique<GotoUsageRule>());
    addRule(ownedRules.back().get());

    ownedRules.push_back(std::make_unique<InfiniteLoopRule>());
    addRule(ownedRules.back().get());

    ownedRules.push_back(std::make_unique<NullDereferenceRule>());
    addRule(ownedRules.back().get());

    ownedRules.push_back(std::make_unique<UseAfterFreeRule>());
    addRule(ownedRules.back().get());

    ownedRules.push_back(std::make_unique<MemoryLeakRule>());
    addRule(ownedRules.back().get());

    ownedRules.push_back(std::make_unique<DoubleFreeRule>());
    addRule(ownedRules.back().get());

    ownedRules.push_back(std::make_unique<DanglingPointerRule>());
    addRule(ownedRules.back().get());
}

/*
 * Sisteme yeni bir analiz kuralı ekler.
 * Null pointer veya aynı ID'ye sahip kural tekrar eklenmez.
 */
void RuleEngine::addRule(IRule* rule) {

    if (rule == nullptr) {
        return;
    }

    if (hasRule(rule->getId())) {
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

/*
 * Diagnostic listesinde herhangi bir bulgu olup olmadığını kontrol eder.
 */
bool RuleEngine::hasDiagnostics(
    const std::vector<Diagnostic>& diagnostics
) const {

    return !diagnostics.empty();
}

/*
 * Warning seviyesindeki diagnostic sayısını döndürür.
 */
int RuleEngine::countWarningDiagnostics(
    const std::vector<Diagnostic>& diagnostics
) const {

    int count = 0;

    for (const Diagnostic& diagnostic : diagnostics) {

        if (diagnostic.getSeverity() == DiagnosticSeverity::WARNING) {
            count++;
        }
    }

    return count;
}

/*
 * Verilen ID'ye sahip kuralın ekli olup olmadığını kontrol eder.
 */
bool RuleEngine::hasRule(const std::string& ruleId) const {

    for (const IRule* rule : rules) {

        if (rule == nullptr) {
            continue;
        }

        if (rule->getId() == ruleId) {
            return true;
        }
    }

    return false;
}

/*
 * Verilen ID'ye sahip kuralı listeden kaldırır.
 */
bool RuleEngine::removeRuleById(const std::string& ruleId) {

    for (auto it = rules.begin(); it != rules.end(); ++it) {

        if (*it == nullptr) {
            continue;
        }

        if ((*it)->getId() == ruleId) {
            rules.erase(it);
            return true;
        }
    }

    return false;
}

/*
 * Sistemde kayıtlı tüm kural ID'lerini döndürür.
 */
std::vector<std::string> RuleEngine::getRuleIds() const {

    std::vector<std::string> ruleIds;

    for (const IRule* rule : rules) {

        if (rule == nullptr) {
            continue;
        }

        ruleIds.push_back(rule->getId());
    }

    return ruleIds;
}

/*
 * Sistemde kayıtlı tüm kural isimlerini döndürür.
 */
std::vector<std::string> RuleEngine::getRuleNames() const {

    std::vector<std::string> ruleNames;

    for (const IRule* rule : rules) {

        if (rule == nullptr) {
            continue;
        }

        ruleNames.push_back(rule->getName());
    }

    return ruleNames;
}

/*
 * Kritik diagnostic bulunup bulunmadığını kontrol eder.
 */
bool RuleEngine::hasCriticalDiagnostics(
    const std::vector<Diagnostic>& diagnostics
) const {

    return countCriticalDiagnostics(diagnostics) > 0;
}

/*
 * Warning diagnostic bulunup bulunmadığını kontrol eder.
 */
bool RuleEngine::hasWarningDiagnostics(
    const std::vector<Diagnostic>& diagnostics
) const {

    return countWarningDiagnostics(diagnostics) > 0;
}

/*
 * Toplam diagnostic sayısını döndürür.
 */
int RuleEngine::countDiagnostics(
    const std::vector<Diagnostic>& diagnostics
) const {

    return static_cast<int>(diagnostics.size());
}

/*
 * Verilen ID'ye sahip kuralı kaldırır.
 */
void RuleEngine::removeRule(const std::string& ruleId) {

    removeRuleById(ruleId);
}