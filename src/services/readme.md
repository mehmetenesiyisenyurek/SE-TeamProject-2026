# Services Modülü

## Genel Amaç

`services` modülü, Code Sentinel sistemi içerisindeki modüller arası bağlantıyı ve servis katmanını yönetmek amacıyla kullanılmaktadır.

Bu yapı sayesinde:

- GUI ve backend bağlantısı sadeleşir,
- Kod tekrarları azaltılır,
- Modüller arası iletişim düzenlenir,
- Sistem daha modüler hale gelir.

Services katmanı, doğrudan analiz yapan bölüm değil; analiz süreçlerini organize eden yardımcı katmandır.

---

# Temel Görevleri

Services modülünün temel görevleri:

- Modüller arası veri aktarımını yönetmek
- Analiz işlemlerini servis yapıları üzerinden çalıştırmak
- GUI ile backend arasındaki bağlantıyı sadeleştirmek
- Analiz süreçlerini daha düzenli hale getirmek
- Kod organizasyonunu iyileştirmek

---

# Çalışma Mantığı

Services sistemi genellikle GUI veya CLI tarafından çağrılır ve ilgili backend modüllerini çalıştırır.

Genel süreç:

```text
GUI / CLI
    ↓
Services Katmanı
    ↓
Analysis Engine
    ↓
Syntax Analyzer / Pointer Rules
    ↓
Diagnostic Sonuçları
```

Bu yapı sayesinde kullanıcı arayüzü doğrudan tüm backend detaylarına bağımlı kalmaz.

---

# Bağlantılı Modüller

| Modül | Görev |
|---|---|
| analysis_engine | Analiz sürecini yönetir |
| gui_winapi | Masaüstü arayüzü |
| syntax_analyzer | Statik analiz kuralları |
| pointer | Pointer analizleri |
| reporting | Raporlama sistemi |

Bu modüllerin detaylı açıklamaları kendi README dosyalarında bulunmaktadır.

---

# Kullanım Amaçları

Services modülü aşağıdaki amaçlarla kullanılmaktadır:

- Analiz çağrılarını merkezileştirmek
- GUI tarafındaki kod karmaşasını azaltmak
- Backend erişimini düzenlemek
- Modüler yapı oluşturmak
- Genişletilebilir sistem mimarisi sağlamak

---

# Mimari Avantajlar

Bu yapı sayesinde sistem:

- Daha okunabilir,
- Daha sürdürülebilir,
- Daha modüler,
- Daha yönetilebilir

hale gelmektedir.

Ayrıca yeni analiz sistemleri veya GUI yapıları eklendiğinde mevcut kod yapısı daha az etkilenmektedir.

---

# Genel Değerlendirme

Services modülü, Code Sentinel mimarisindeki yardımcı bağlantı katmanıdır.

Bu yapı sayesinde GUI, CLI ve backend sistemleri arasındaki iletişim daha düzenli şekilde yönetilebilmektedir.

Services katmanı sistem mimarisinin daha temiz ve profesyonel görünmesine katkı sağlamaktadır.

---

# Geliştirilebilecek Noktalar

- Async servis yapıları
- Çoklu analiz yönetimi
- Task queue sistemi
- Service manager yapısı
- Event tabanlı servis mimarisi
- Gerçek zamanlı analiz desteği