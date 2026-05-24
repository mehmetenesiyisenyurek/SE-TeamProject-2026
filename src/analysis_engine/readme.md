
# Analysis Engine

## Genel Amaç

`analysis_engine` modülü, Code Sentinel sisteminin analiz sürecini yöneten ana katmandır.

Bu modül;

- Kaynak kodun alınması,
- Ön işleme sürecinin başlatılması,
- Lexer ve parser işlemlerinin çalıştırılması,
- AST oluşturulması,
- Statik analiz kurallarının yürütülmesi,
- Diagnostic sonuçlarının toplanması

işlemlerini merkezi olarak yönetmektedir.

Sistem içerisindeki tüm analiz akışı bu yapı üzerinden kontrol edilmektedir.

---

# Çalışma Mantığı

Analysis Engine, diğer modüller ile bağlantılı şekilde çalışmaktadır.

Genel analiz süreci:

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
```

Bu yapı sayesinde analiz işlemleri modüler ve düzenli şekilde yürütülmektedir.

---

# Bağlantılı Modüller

| Modül | Görev |
|---|---|
| infrastructure | Temel altyapı işlemleri |
| lexer | Token üretimi |
| parser | AST oluşturma |
| syntax_analyzer | Statik analiz kuralları |
| pointer | Pointer analizleri |
| reporting | Raporlama sistemi |

Bu modüllerin detaylı açıklamaları kendi README dosyalarında bulunmaktadır.

---

# Temel Görevleri

Analysis Engine modülünün başlıca görevleri:

- Analiz sürecini başlatmak
- Kaynak kodu analiz pipeline’ına aktarmak
- Modüller arası veri akışını yönetmek
- Diagnostic sonuçlarını toplamak
- Analiz çıktısını raporlama sistemine göndermek
- CLI ve GUI sistemleri ile bağlantı kurmak

---

# Diagnostic Yönetimi

Analiz sırasında oluşan hata ve uyarılar `Diagnostic` yapıları içerisinde tutulmaktadır.

Örnek diagnostic çıktıları:

```text
R007 -> malloc sonrasi NULL kontrolu yapilmamis
R008 -> free sonrasi pointer tekrar kullaniliyor
R010 -> double free tespit edildi
```

Bu çıktılar daha sonra GUI veya reporting sistemi üzerinden kullanıcıya sunulmaktadır.

---

# CLI ve Desktop Bağlantısı

Analysis Engine hem CLI hem de masaüstü sürümü tarafından kullanılmaktadır.

## CodeSentinelCLI

Konsol tabanlı kullanım sağlar.

Sonuçlar terminal ekranına yazdırılır.

---

## CodeSentinelDesktop

WinAPI tabanlı masaüstü uygulamasıdır.

Analysis Engine üzerinden gelen diagnostic sonuçları GUI üzerinde gösterilir.

---

# Kullanılan Yapılar

Bu modül içerisinde:

- AST tabanlı analiz
- Rule engine yapısı
- Diagnostic yönetimi
- Modüler analiz sistemi
- Pipeline mantığı

aktif olarak kullanılmaktadır.

---

# Genel Değerlendirme

Analysis Engine, Code Sentinel projesinin merkezi analiz sistemidir.

Tüm modüller bu yapı üzerinden birbirine bağlanmakta ve analiz süreci buradan yönetilmektedir.

Bu yapı sayesinde sistem:

- Daha modüler,
- Daha sürdürülebilir,
- Daha düzenli,
- Daha genişletilebilir

hale gelmektedir.

---

# Geliştirilebilecek Noktalar

- Paralel analiz desteği
- Çoklu dosya analizi
- Performans optimizasyonları
- Daha gelişmiş diagnostic yönetimi
- Rule execution optimizasyonu
- Gerçek zamanlı analiz desteği