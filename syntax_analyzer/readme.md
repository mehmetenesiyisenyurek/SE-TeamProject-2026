# Syntax Analyzer

`Syntax Analyzer` modülü, proje içerisinde C kaynak kodlarının statik sözdizimi analizini gerçekleştirmek için geliştirilmiştir.  
Bu modül lexer ve parser bileşenleri ile birlikte çalışarak sözdizimsel hataları, kural ihlallerini ve şüpheli kod yapılarını tespit eder.

---

## Modülün Görevleri

- Abstract Syntax Tree (AST) yapılarını analiz etmek
- Sözdizimi (syntax) hatalarını tespit etmek
- Kural tabanlı analiz işlemlerini yürütmek
- Uyarı ve hata diagnostiklerini üretmek
- Genişletilebilir analiz mimarisi sağlamak

---

## Özellikler

- Kural tabanlı syntax kontrol sistemi
- Modüler ve geliştirilebilir yapı
- Diagnostic severity desteği
- Aşağıdaki durumların tespiti:
    - Başlatılmamış değişken kullanımı
    - Başlatılmamış pointer kullanımı
    - Eksik syntax yapıları
    - Geçersiz kod blokları
- Yeni kuralların kolayca eklenebilmesi

---

## Temel Bileşenler

| Dosya / Sınıf | Açıklama |
|---|---|
| `SyntaxAnalyzer` | Ana analiz işlemlerini yönetir |
| `RuleEngine` | Tanımlı kuralları çalıştırır |
| `IRule` | Tüm syntax kuralları için temel arayüz |
| `UninitializedVarRule` | Başlatılmamış değişkenleri kontrol eder |
| `UninitializedPointerRule` | Başlatılmamış pointer kullanımını kontrol eder |

---

## Çalışma Mantığı

1. Kaynak kod lexer tarafından token’lara ayrılır
2. Parser tarafından AST yapısı oluşturulur
3. Syntax Analyzer AST düğümlerini analiz eder
4. Rule Engine tanımlı kuralları çalıştırır
5. Tespit edilen problemler diagnostic olarak raporlanır

---

## Diagnostic Sistemi

Tespit edilen her problem için bir diagnostic oluşturulur. Bu diagnostic aşağıdaki bilgileri içerir:

- Kural ID’si
- Hata/Uyarı mesajı
- Severity seviyesi
- Konum bilgisi

### Örnek Severity Seviyeleri

- `INFO`
- `WARNING`
- `ERROR`
- `CRITICAL`

---

## Projenin Amacı

Bu modülün amacı, derleme veya çalışma zamanından önce kaynak kod içerisindeki syntax tabanlı problemleri tespit ederek kod kalitesini artırmaktır.

---

## Notlar

- Parser ile ilgili bazı bağımlılıklar ekip geliştirme süreci nedeniyle geçici olarak yorum satırında bırakılmış olabilir.
- Modül ekip çalışmasına uygun şekilde aşamalı olarak geliştirilmektedir.
- Yeni analiz kuralları `IRule` arayüzünü implemente ederek sisteme kolayca eklenebilir.