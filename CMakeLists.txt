cmake_minimum_required(VERSION 3.20)

# Proje ismi bir kez tanımlanır
project("SE-TeamProject-2026")

set(CMAKE_CXX_STANDARD 17)

# Alttaki boş project() komutunu sildik
add_executable("SE-TeamProject-2026" main.cpp
        infrastructure/SourceFile.cpp
        infrastructure/SourceFile.h
        infrastructure/DiagnosticSeverity.h
        infrastructure/Diagnostic.cpp
        infrastructure/Diagnostic.h
        infrastructure/CodeMetric.cpp
        infrastructure/CodeMetric.h
        infrastructure/ValidationResult.cpp
        infrastructure/ValidationResult.h
        syntax_analyzer/IRule.h
        syntax_analyzer/RuleEngine.h
        syntax_analyzer/RuleEngine.cpp
        syntax_analyzer/UnusedVariableRule.h
        syntax_analyzer/UnusedVariableRule.cpp
        syntax_analyzer/UninitializedVarRule.h
        syntax_analyzer/UninitializedVarRule.cpp
        syntax_analyzer/UninitializedPointerRule.h
        syntax_analyzer/UninitializedPointerRule.cpp
        syntax_analyzer/SyntaxAnalyzer.h
        syntax_analyzer/SyntaxAnalyzer.cpp)