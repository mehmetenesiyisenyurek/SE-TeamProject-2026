cmake_minimum_required(VERSION 3.20)

project("SE-TeamProject-2026")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tüm kaynak dosyalarını tek bir add_executable içinde topluyoruz
add_executable("SE-TeamProject-2026"
        main.cpp
        # Parser klasörü
        parser/ASTNodeType.h
        parser/ASTNode.cpp
        parser/ASTNode.h
        parser/Parser.cpp
        parser/Parser.h
        # Infrastructure klasörü
        infrastructure/SourceFile.cpp
        infrastructure/SourceFile.h
        infrastructure/DiagnosticSeverity.h
        infrastructure/Diagnostic.cpp
        infrastructure/Diagnostic.h
        infrastructure/CodeMetric.cpp
        infrastructure/CodeMetric.h
        infrastructure/ValidationResult.cpp
        infrastructure/ValidationResult.h
        infrastructure/Preprocessor.cpp
        infrastructure/Preprocessor.h
        # Syntax Analyzer klasörü
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
        syntax_analyzer/SyntaxAnalyzer.cpp
        # Lexer klasörü
        lexer/Lexer.cpp
        lexer/Lexer.h
        lexer/Token.cpp
        lexer/Token.h
        lexer/TokenType.h
        # Pointer kuralları
        pointer/NullDereferenceRule.cpp
        pointer/NullDereferenceRule.h
        pointer/UseAfterFreeRule.cpp
        pointer/UseAfterFreeRule.h
        pointer/MetricCalculator.cpp
        pointer/MetricCalculator.h
        pointer/DoubleFreeRule.h
        pointer/DoubleFreeRule.cpp
        pointer/DanglingPointerRule.h
        pointer/DanglingPointerRule.cpp

)

# HATAYI ÇÖZEN KRİTİK SATIR:
# Bu satır sayesinde #include "DiagnosticSeverity.h" yazdığında
# derleyici infrastructure klasörünün içine bakması gerektiğini anlar.
target_include_directories("SE-TeamProject-2026" PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/infrastructure
        ${CMAKE_CURRENT_SOURCE_DIR}/parser
        ${CMAKE_CURRENT_SOURCE_DIR}/syntax_analyzer
        ${CMAKE_CURRENT_SOURCE_DIR}/lexer
        ${CMAKE_CURRENT_SOURCE_DIR}/pointer
        ${CMAKE_CURRENT_SOURCE_DIR}/analysis_engine
)