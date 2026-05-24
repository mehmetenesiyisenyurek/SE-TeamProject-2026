# Reporting Modülü

## Genel Amaç

`reporting` modülü, Code Sentinel analiz sistemi tarafından üretilen diagnostic sonuçlarının dışa aktarılmasından sorumludur.

Bu modül sayesinde analiz sonuçları kullanıcıya daha okunabilir ve düzenli formatlarda sunulabilmektedir.

Reporting sistemi;

- Diagnostic sonuçlarını toplar,
- Çıktıları düzenler,
- Farklı dosya formatlarında rapor üretir.

---

# Desteklenen Çıktılar

Reporting modülü aşağıdaki çıktı formatlarını desteklemektedir:

- TXT
- HTML
- JSON

Bu yapı sayesinde analiz sonuçları farklı kullanım senaryolarına uygun şekilde dışa aktarılabilmektedir.

---

# Genel Yapı

```text
reporting/
│
├── ReportManager
├── TxtExporter
├── HtmlExporter
├── JsonExporter
└── ilgili yardımcı yapılar
```

---

# Çalışma Mantığı

Reporting sistemi analiz tamamlandıktan sonra çalışmaktadır.

Genel süreç:

```text
Analiz Tamamlanır
        ↓
Diagnostic Sonuçları Toplanır
        ↓
Reporting Sistemi Çalışır
        ↓
Uygun Exporter Seçilir
        ↓
Rapor Dosyası Oluşturulur
```

---

# ReportManager

`ReportManager`, raporlama sürecini yöneten ana yapıdır.

Görevleri:

- Diagnostic sonuçlarını almak
- Uygun exporter sistemini çalıştırmak
- Rapor oluşturma sürecini yönetmek
- Çıktıları organize etmek

---

# Exporter Yapıları

## TxtExporter

Analiz sonuçlarını düz metin formatında üretir.

Örnek kullanım alanları:

- Konsol çıktıları
- Basit log sistemleri
- Hafif raporlama ihtiyaçları

---

## HtmlExporter

Diagnostic sonuçlarını HTML formatında üretir.

Avantajları:

- Daha okunabilir yapı
- Tarayıcı üzerinden görüntüleme
- Daha düzenli hata listeleri

---

## JsonExporter

Analiz sonuçlarını JSON formatında üretir.

Avantajları:

- API entegrasyonu
- Veri işleme sistemleri
- Otomasyon süreçleri
- Makine tarafından okunabilir çıktı

---

# Diagnostic Yapısı

Reporting sistemi diagnostic sonuçlarını kullanmaktadır.

Örnek diagnostic çıktısı:

```text
R007 -> malloc sonrasi NULL kontrolu yapilmamis
R008 -> free sonrasi pointer tekrar kullaniliyor
R010 -> double free tespit edildi
```

Her diagnostic çıktısı genellikle:

- Hata kodu,
- Açıklama,
- Satır bilgisi,
- Severity bilgisi

içermektedir.

---

# Bağlantılı Modüller

| Modül | Görev |
|---|---|
| analysis_engine | Analiz sonuçlarını üretir |
| syntax_analyzer | Statik analiz kurallarını çalıştırır |
| pointer | Pointer diagnostic sonuçlarını üretir |
| gui_winapi | Sonuçları kullanıcıya gösterir |

Bu modüllerin detaylı açıklamaları kendi README dosyalarında bulunmaktadır.

---

# Kullanım Amaçları

Reporting sistemi aşağıdaki amaçlarla kullanılmaktadır:

- Analiz sonuçlarını kaydetmek
- Diagnostic geçmişi oluşturmak
- Hata raporları üretmek
- Kullanıcıya okunabilir çıktı sunmak
- GUI ve CLI sistemlerine çıktı sağlamak

---

# Genel Değerlendirme

Reporting modülü, Code Sentinel sisteminin analiz sonuçlarını kullanıcıya aktaran önemli bileşenlerinden biridir.

Bu yapı sayesinde:

- Diagnostic sonuçları daha düzenli sunulabilir,
- Analiz çıktıları dışa aktarılabilir,
- Farklı sistemlerle entegrasyon sağlanabilir,
- Daha profesyonel raporlama yapılabilir.

---

# Geliştirilebilecek Noktalar

- PDF export desteği
- Grafiksel raporlama
- İstatistiksel özetler
- Severity filtreleme
- Zaman damgalı rapor sistemi
- Çoklu rapor birleştirme
- Gerçek zamanlı rapor güncelleme