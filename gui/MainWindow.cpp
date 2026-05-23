#include "MainWindow.h"

#include <QApplication>
#include <QFileDialog>
#include <QFile>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSplitter>
#include <QTableWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QStatusBar>
#include <QTextStream>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    buildUi();
    applyStyle();
}

void MainWindow::buildUi() {
    setWindowTitle("CodeSentinel Desktop");
    resize(1200, 760);

    auto* central = new QWidget(this);
    auto* rootLayout = new QVBoxLayout(central);

    auto* topBar = new QHBoxLayout();
    auto* selectButton = new QPushButton("Dosya Sec", this);
    analyzeButton = new QPushButton("Analiz Et", this);
    saveButton = new QPushButton("Raporu Kaydet", this);
    saveButton->setEnabled(false);

    fileLabel = new QLabel("Secilen dosya yok", this);
    fileLabel->setMinimumWidth(450);
    statusLabel = new QLabel("Hazir", this);

    topBar->addWidget(selectButton);
    topBar->addWidget(analyzeButton);
    topBar->addWidget(saveButton);
    topBar->addWidget(fileLabel, 1);
    topBar->addWidget(statusLabel);
    rootLayout->addLayout(topBar);

    codeView = new QPlainTextEdit(this);
    codeView->setReadOnly(true);
    codeView->setPlaceholderText("Sol tarafta analiz edilen C kodu gorunecek.");

    diagnosticsTable = new QTableWidget(this);
    diagnosticsTable->setColumnCount(4);
    diagnosticsTable->setHorizontalHeaderLabels({"Satir", "Seviye", "Kural", "Mesaj"});
    diagnosticsTable->horizontalHeader()->setStretchLastSection(true);
    diagnosticsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    diagnosticsTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    metricsTable = new QTableWidget(this);
    metricsTable->setColumnCount(4);
    metricsTable->setHorizontalHeaderLabels({"Metrik", "Deger", "Birim", "Aciklama"});
    metricsTable->horizontalHeader()->setStretchLastSection(true);
    metricsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    metricsTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    reportView = new QTextEdit(this);
    reportView->setReadOnly(true);
    reportView->setPlaceholderText("Analiz raporu burada olusacak.");

    auto* rightTabs = new QSplitter(Qt::Vertical, this);
    rightTabs->addWidget(diagnosticsTable);
    rightTabs->addWidget(metricsTable);
    rightTabs->addWidget(reportView);
    rightTabs->setSizes({300, 180, 220});

    auto* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(codeView);
    splitter->addWidget(rightTabs);
    splitter->setSizes({520, 680});
    rootLayout->addWidget(splitter, 1);

    setCentralWidget(central);
    statusBar()->showMessage("CodeSentinel Desktop hazir");

    connect(selectButton, &QPushButton::clicked, this, &MainWindow::selectFile);
    connect(analyzeButton, &QPushButton::clicked, this, &MainWindow::runAnalysis);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveReport);
}

void MainWindow::applyStyle() {
    qApp->setStyleSheet(
        "QMainWindow { background: #f3f4f6; }"
        "QPushButton { padding: 8px 14px; border-radius: 8px; background: #1f2937; color: white; font-weight: 600; }"
        "QPushButton:hover { background: #374151; }"
        "QPushButton:disabled { background: #9ca3af; }"
        "QPlainTextEdit, QTextEdit, QTableWidget { background: white; border: 1px solid #d1d5db; border-radius: 8px; }"
        "QHeaderView::section { background: #111827; color: white; padding: 6px; border: 0; }"
        "QLabel { color: #111827; }"
    );
}

void MainWindow::selectFile() {
    const QString path = QFileDialog::getOpenFileName(
        this,
        "Analiz edilecek C dosyasini sec",
        QString(),
        "C/C++ Kaynak Dosyalari (*.c *.h *.cpp *.hpp);;Tum Dosyalar (*.*)"
    );

    if (path.isEmpty()) {
        return;
    }

    selectedFilePath = path;
    fileLabel->setText(path);

    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        codeView->setPlainText(QString::fromUtf8(file.readAll()));
        file.close();
    }

    statusLabel->setText("Dosya secildi");
}

void MainWindow::runAnalysis() {
    if (selectedFilePath.isEmpty()) {
        QMessageBox::warning(this, "Dosya yok", "Once analiz edilecek bir .c dosyasi secmelisin.");
        return;
    }

    statusLabel->setText("Analiz ediliyor...");
    QApplication::processEvents();

    lastOutput = analysisService.analyzeFile(selectedFilePath.toStdString());
    showOutput(lastOutput);
}

void MainWindow::showOutput(const AnalysisOutput& output) {
    if (!output.success) {
        statusLabel->setText("Hata olustu");
        reportView->setPlainText(QString::fromStdString("Hata: " + output.errorMessage));
        QMessageBox::critical(this, "Analiz hatasi", QString::fromStdString(output.errorMessage));
        saveButton->setEnabled(false);
        return;
    }

    codeView->setPlainText(QString::fromStdString(output.rawCode));
    fillDiagnostics(output);
    fillMetrics(output);
    reportView->setPlainText(buildTextReport(output));
    saveButton->setEnabled(true);

    statusLabel->setText(QString("Tamamlandi: %1 bulgu").arg(output.diagnostics.size()));
    statusBar()->showMessage("Analiz tamamlandi");
}

void MainWindow::fillDiagnostics(const AnalysisOutput& output) {
    diagnosticsTable->setRowCount(static_cast<int>(output.diagnostics.size()));

    for (int row = 0; row < static_cast<int>(output.diagnostics.size()); ++row) {
        const Diagnostic& d = output.diagnostics[row];
        diagnosticsTable->setItem(row, 0, new QTableWidgetItem(QString::number(d.getLine())));
        diagnosticsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(severityToText(d.getSeverity()))));
        diagnosticsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(d.getRuleId())));
        diagnosticsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(d.getMessage())));
    }

    diagnosticsTable->resizeColumnsToContents();
    diagnosticsTable->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::fillMetrics(const AnalysisOutput& output) {
    metricsTable->setRowCount(static_cast<int>(output.metrics.size()));

    for (int row = 0; row < static_cast<int>(output.metrics.size()); ++row) {
        const CodeMetric& m = output.metrics[row];
        metricsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(m.getName())));
        metricsTable->setItem(row, 1, new QTableWidgetItem(QString::number(m.getValue())));
        metricsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(m.getUnit())));
        metricsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(m.getDescription())));
    }

    metricsTable->resizeColumnsToContents();
    metricsTable->horizontalHeader()->setStretchLastSection(true);
}

QString MainWindow::buildTextReport(const AnalysisOutput& output) const {
    QString text;
    QTextStream stream(&text);

    stream << "CodeSentinel Analiz Raporu\n";
    stream << "Dosya: " << QString::fromStdString(output.fileName) << "\n";
    stream << "Boyut: " << output.fileSize << " byte\n";
    stream << "Kural sayisi: " << output.ruleCount << "\n";
    stream << "Yorum satiri sayisi: " << output.commentLineCount << "\n\n";

    stream << "===== DIAGNOSTIC SONUCLARI =====\n";
    if (output.diagnostics.empty()) {
        stream << "Herhangi bir hata veya uyari bulunamadi.\n";
    } else {
        for (const Diagnostic& d : output.diagnostics) {
            stream << "Satir " << d.getLine()
                   << " | " << QString::fromStdString(severityToText(d.getSeverity()))
                   << " | " << QString::fromStdString(d.getRuleId())
                   << " | " << QString::fromStdString(d.getMessage()) << "\n";
        }
    }

    stream << "\n===== METRIK SONUCLARI =====\n";
    if (output.metrics.empty()) {
        stream << "Metrik hesaplanamadi.\n";
    } else {
        for (const CodeMetric& m : output.metrics) {
            stream << QString::fromStdString(m.getName())
                   << ": " << m.getValue()
                   << " " << QString::fromStdString(m.getUnit())
                   << " - " << QString::fromStdString(m.getDescription()) << "\n";
        }
    }

    return text;
}

void MainWindow::saveReport() {
    if (!lastOutput.success) {
        return;
    }

    const QString path = QFileDialog::getSaveFileName(
        this,
        "Raporu kaydet",
        "analysis_report.txt",
        "Text Dosyasi (*.txt);;Tum Dosyalar (*.*)"
    );

    if (path.isEmpty()) {
        return;
    }

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Kaydetme hatasi", "Rapor dosyasi olusturulamadi.");
        return;
    }

    QTextStream stream(&file);
    stream << reportView->toPlainText();
    file.close();
    statusBar()->showMessage("Rapor kaydedildi: " + path);
}
