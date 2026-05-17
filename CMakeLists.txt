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
        infrastructure/Preprocessor.cpp
        infrastructure/Preprocessor.h
        infrastructure/FileValidator.cpp
        infrastructure/FileValidator.h
        infrastructure/FileLoader.cpp
        infrastructure/FileLoader.h
        infrastructure/SeverityClassifier.cpp
        infrastructure/SeverityClassifier.h
        infrastructure/Application.cpp
        infrastructure/Application.h)