# Code Sentinel

## Proje Amacı

Code Sentinel, C projelerinde statik kod analizi yaparak yazılım hatalarını erken aşamada tespit etmeyi amaçlayan bir masaüstü analiz platformudur.

Projenin temel amacı;

- Bellek güvenliği problemlerini tespit etmek,
- Kod kalitesini analiz etmek,
- Riskli kullanım senaryolarını belirlemek,
- Yazılımcıya hata raporları sunmak,
- Daha güvenli ve sürdürülebilir yazılım geliştirme süreçlerine katkı sağlamaktır.

Sistem; kaynak kodu çalıştırmadan analiz ederek potansiyel hata durumlarını belirler ve kullanıcıya detaylı raporlar halinde sunar.

---

# Desteklenen Analizler

## Pointer ve Bellek Güvenliği Analizi

- Null Dereference
- Use After Free
- Double Free
- Dangling Pointer
- Memory Leak

## Kod Kalitesi Analizi

- Kullanılmayan değişkenler
- Başlatılmamış değişkenler
- Başlatılmamış pointer kullanımları
- Uzun fonksiyon kontrolleri
- Magic number kullanımları
- Goto kullanımları

## Sözdizimsel Analizler

- AST tabanlı analiz
- Lexer & Parser kontrolleri
- Basit syntax doğrulamaları
- Kod yapısı incelemeleri



---

# Kullanılan Teknolojiler

- C++
- CMake
- WinAPI
- AST (Abstract Syntax Tree)
- Static Code Analysis
- Object-Oriented Programming

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

# Masaüstü Uygulaması

Proje içerisinde WinAPI tabanlı modern bir masaüstü uygulaması bulunmaktadır.

İndirme Linki:https://drive.google.com/drive/folders/15-AZ9rvSn0oFKuQJ6io1OOEKRS2AgB41

Bu arayüz sayesinde kullanıcı;

- Kod dosyalarını seçebilir,
- Analizi başlatabilir,
- Hataları görüntüleyebilir,
- Analiz sonuçlarını inceleyebilir,
- Rapor oluşturabilir.

---

# Hedef

Code Sentinel’in temel hedefi;

- Daha güvenli C yazılımları geliştirilmesine yardımcı olmak,
- Bellek yönetimi kaynaklı kritik hataları azaltmak,
- Statik analiz süreçlerini daha erişilebilir hale getirmek,
- Geliştiricilere modern bir analiz ortamı sunmaktır.

---

# Genel Durum

Proje aktif geliştirme sürecindedir.

Mevcut durumda sistem;

- AST üretimi,
- Pointer analizi,
- Statik kural çalıştırma,
- Diagnostic üretimi,
- Masaüstü arayüz desteği,
- TXT / HTML / JSON raporlama

özelliklerini desteklemektedir.

---

# Geliştirici Ekibi

- Ferhat Enes Kocabıyık
- Hacı Ömer Oğuz
- Hüseyin Mert Kanlıdere
- Kemal Babaoğlan
- Mehmet Enes İyişenyürek
- Mehmet Uçar