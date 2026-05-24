# Code Sentinel

## Genel Bakış

Code Sentinel, C projeleri üzerinde statik kod analizi gerçekleştiren masaüstü tabanlı bir analiz platformudur.

Sistem; kaynak kodları çalıştırmadan analiz ederek bellek güvenliği problemlerini, potansiyel yazılım hatalarını ve kod kalitesi sorunlarını tespit etmeyi amaçlamaktadır.

---

# Proje Yapısı

```text
src/
│
├── analysis_engine/
├── gui/
├── gui_winapi/
├── infrastructure/
├── lexer/
├── parser/
├── pointer/
├── reporting/
├── services/
├── syntax_analyzer/
└── main.cpp
```

---

# Modüller

| Modül | Açıklama |
|---|---|
| analysis_engine | Analiz sürecini yöneten ana sistem |
| gui | Genel arayüz bileşenleri |
| gui_winapi | WinAPI tabanlı masaüstü uygulaması |
| infrastructure | Temel altyapı ve yardımcı yapılar |
| lexer | Token üretim sistemi |
| parser | AST oluşturma sistemi |
| pointer | Pointer ve bellek güvenliği analizleri |
| reporting | TXT / HTML / JSON raporlama sistemi |
| services | Servis ve bağlantı katmanı |
| syntax_analyzer | Statik analiz kuralları |

Her modülün detaylı açıklaması ilgili klasör içerisindeki README dosyalarında bulunmaktadır.

---

# Çalışma Mantığı

```text
Kaynak Kod
   ↓
Preprocessor
   ↓
Lexer
   ↓
Parser
   ↓
AST Oluşturma
   ↓
Statik Analiz Kuralları
   ↓
Diagnostic Sonuçları
   ↓
Raporlama Sistemi
```

---

# Çalıştırma Yapısı

Proje iki farklı çalışma hedefi içermektedir:

## CodeSentinelCLI

Konsol tabanlı sürümdür.

Bu yapı;

- Terminal üzerinden çalışır,
- Analiz sonuçlarını konsola yazdırır,
- Daha hafif test ve debug süreçleri için kullanılır.

---

## CodeSentinelDesktop

WinAPI tabanlı masaüstü uygulamasıdır.

Bu yapı;

- Grafiksel kullanıcı arayüzü içerir,
- Dosya seçme ekranı sunar,
- Diagnostic sonuçlarını GUI üzerinde gösterir,
- Daha kullanıcı dostu bir analiz deneyimi sağlar.

---

# main.cpp

`main.cpp` dosyası, sistemin başlangıç noktasıdır.

Bu dosya temel olarak:

- Analiz sürecini başlatır,
- Gerekli modülleri çalıştırır,
- Test analizlerini yürütür,
- CLI sürümünün giriş noktası olarak görev yapar.

Desktop sürümünde ise giriş noktası doğrudan WinAPI tarafındaki `WinMain` yapısı üzerinden çalışmaktadır.

---

# Kullanılan Teknolojiler

- C++
- CMake
- WinAPI
- AST (Abstract Syntax Tree)
- Static Code Analysis

---

# Genel Durum

Mevcut durumda sistem;

- AST üretimi,
- Pointer analizi,
- Static rule engine,
- Diagnostic sistemi,
- Masaüstü GUI,
- TXT / HTML / JSON raporlama

özelliklerini desteklemektedir.

---

# Hedef

Code Sentinel’in temel hedefi;

- Daha güvenli C projeleri geliştirilmesine yardımcı olmak,
- Bellek güvenliği problemlerini azaltmak,
- Statik analiz süreçlerini modernleştirmek,
- Kullanıcı dostu bir analiz ortamı sunmaktır.