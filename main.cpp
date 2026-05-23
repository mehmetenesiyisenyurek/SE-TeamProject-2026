#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "services/AnalysisService.h"

using namespace std;

static void printDiagnostics(const vector<Diagnostic>& diagnostics, ostream& out) {
    if (diagnostics.empty()) {
        out << "Herhangi bir hata veya uyari bulunamadi.\n";
        return;
    }

    out << left << setw(8) << "Satir"
        << setw(10) << "Seviye"
        << setw(8) << "Kural"
        << "Mesaj\n";
    out << string(80, '-') << "\n";

    for (const Diagnostic& diagnostic : diagnostics) {
        out << left << setw(8) << diagnostic.getLine()
            << setw(10) << severityToText(diagnostic.getSeverity())
            << setw(8) << diagnostic.getRuleId()
            << diagnostic.getMessage() << "\n";
    }
}

static void printMetrics(const vector<CodeMetric>& metrics, ostream& out) {
    if (metrics.empty()) {
        out << "Metrik hesaplanamadi.\n";
        return;
    }

    out << left << setw(30) << "Metrik"
        << setw(12) << "Deger"
        << setw(12) << "Birim"
        << "Aciklama\n";
    out << string(80, '-') << "\n";

    for (const CodeMetric& metric : metrics) {
        out << left << setw(30) << metric.getName()
            << setw(12) << metric.getValue()
            << setw(12) << metric.getUnit()
            << metric.getDescription() << "\n";
    }
}

int main(int argc, char* argv[]) {
    string filePath = "test_input.c";
    if (argc > 1) {
        filePath = argv[1];
    }

    cout << "CodeSentinel statik analiz baslatildi.\n";
    cout << "Analiz edilen dosya: " << filePath << "\n\n";

    AnalysisService service;
    AnalysisOutput output = service.analyzeFile(filePath);

    if (!output.success) {
        cerr << "Hata: " << output.errorMessage << "\n";
        return 1;
    }

    cout << "===== DIAGNOSTIC SONUCLARI =====\n";
    printDiagnostics(output.diagnostics, cout);

    cout << "\n===== METRIK SONUCLARI =====\n";
    printMetrics(output.metrics, cout);

    ofstream report("analysis_report.txt");
    if (report.is_open()) {
        report << "CodeSentinel Analiz Raporu\n";
        report << "Dosya: " << output.fileName << "\n";
        report << "Boyut: " << output.fileSize << " byte\n";
        report << "Kural sayisi: " << output.ruleCount << "\n";
        report << "Yorum satiri sayisi: " << output.commentLineCount << "\n\n";

        report << "===== DIAGNOSTIC SONUCLARI =====\n";
        printDiagnostics(output.diagnostics, report);

        report << "\n===== METRIK SONUCLARI =====\n";
        printMetrics(output.metrics, report);
        report.close();

        cout << "\nRapor olusturuldu: analysis_report.txt\n";
    }

    return 0;
}
