#ifndef CODESENTINEL_MAIN_WINDOW_H
#define CODESENTINEL_MAIN_WINDOW_H

#include <QMainWindow>
#include <QString>

#include "services/AnalysisService.h"

class QLabel;
class QPlainTextEdit;
class QPushButton;
class QTableWidget;
class QTextEdit;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void selectFile();
    void runAnalysis();
    void saveReport();

private:
    void buildUi();
    void applyStyle();
    void showOutput(const AnalysisOutput& output);
    void fillDiagnostics(const AnalysisOutput& output);
    void fillMetrics(const AnalysisOutput& output);
    QString buildTextReport(const AnalysisOutput& output) const;

    AnalysisService analysisService;
    AnalysisOutput lastOutput;
    QString selectedFilePath;

    QLabel* fileLabel = nullptr;
    QLabel* statusLabel = nullptr;
    QPlainTextEdit* codeView = nullptr;
    QTableWidget* diagnosticsTable = nullptr;
    QTableWidget* metricsTable = nullptr;
    QTextEdit* reportView = nullptr;
    QPushButton* analyzeButton = nullptr;
    QPushButton* saveButton = nullptr;
};

#endif
