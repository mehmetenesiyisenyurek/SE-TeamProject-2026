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
        infrastructure/AnalysisResult.cpp
        infrastructure/AnalysisResult.h
        infrastructure/IExporter.h
        infrastructure/Preprocessor.cpp
        infrastructure/Preprocessor.h
        infrastructure/MetricCalculator.cpp
        infrastructure/MetricCalculator.h
        infrastructure/FileLoader.h
        infrastructure/SeverityClassifier.h
        reporting/TxtExporter.cpp
        reporting/TxtExporter.h
        reporting/JsonExporter.cpp
        reporting/JsonExporter.h
        reporting/HtmlExporter.cpp
        reporting/HtmlExporter.h
        reporting/ReportManager.cpp
        reporting/ReportManager.h
        lexer/Lexer.h
        lexer/Token.h
        parser/ASTNode.cpp
        parser/ASTNode.h
        parser/Parser.h
        syntax_analyzer/RuleEngine.h
        syntax_analyzer/SyntaxAnalyzer.h
        analysis_engine/AnalysisEngine.cpp
        analysis_engine/AnalysisEngine.h)