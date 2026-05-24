# GUI Modülü

## Genel Amaç

GUI modülü, Code Sentinel projesinin kullanıcı arayüzü katmanını oluşturmaktadır.

Bu modül sayesinde kullanıcılar;

- Kod dosyalarını seçebilir,
- Analiz işlemini başlatabilir,
- Hata sonuçlarını görüntüleyebilir,
- Analiz raporlarını inceleyebilir,
- Daha kullanıcı dostu bir ortamda sistemi kullanabilir.

GUI sistemi, doğrudan WinAPI kullanılarak geliştirilmiştir.

Projede Qt veya harici GUI frameworkleri kullanılmamıştır.

---

# Kullanılan Teknolojiler

- WinAPI
- C++
- GDI / GDI+
- Windows Controls
- CMake

---

# GUI Yapısı

```text
gui/
gui_winapi/
```

GUI sistemi temel olarak iki bölümden oluşmaktadır:

| Klasör | Açıklama |
|---|---|
| gui | Genel arayüz bileşenleri |
| gui_winapi | WinAPI tabanlı masaüstü uygulaması |

---

# Arayüzün Genel Özellikleri

GUI sistemi kullanıcıya modern ve sade bir analiz ekranı sunmayı amaçlamaktadır.

Mevcut özellikler:

- Dosya seçme sistemi
- Analiz başlatma butonu
- Hata sonuçlarını listeleme
- Kod çıktı alanı
- Diagnostic görüntüleme
- Scroll desteği
- Modern renk yapısı
- Responsive pencere yerleşimi
- Tam ekran desteği

---

# Çalışma Mantığı

GUI sistemi backend analiz sistemi ile bağlantılı çalışmaktadır.

Genel süreç aşağıdaki gibidir:

```text
Kullanıcı Dosya Seçer
        ↓
GUI Dosya Yolunu Alır
        ↓
Analysis Engine Çalışır
        ↓
Kurallar Analiz Edilir
        ↓
Diagnostic Sonuçları Üretilir
        ↓
GUI Sonuçları Ekrana Yazar
```

---

# Kullanıcı Arayüzü Bileşenleri

GUI içerisinde aşağıdaki WinAPI bileşenleri kullanılmaktadır:

| Bileşen | Amaç |
|---|---|
| Button | Analiz başlatma |
| EditText | Dosya yolu gösterme |
| RichEdit | Çıktıları gösterme |
| Static Text | Başlık ve açıklamalar |
| ScrollBar | Uzun içerik desteği |
| File Dialog | Dosya seçme |

---

# Dosya Seçme Sistemi

Kullanıcı analiz edilecek dosyayı sistem üzerinden seçebilir.

Kullanılan filtre:

```
ofn.lpstrFilter =
L"C/C++ Dosyalari\0*.c\0";
```

Bu filtre sayesinde yalnızca:

- `.c`
uzantılı dosyalar seçilebilir.

---

# Diagnostic Görüntüleme

GUI sistemi analiz sonuçlarını kullanıcıya okunabilir formatta sunar.

Örnek çıktı:

```text
R007 -> malloc sonrasi NULL kontrolu yapilmamis
R008 -> free sonrasi pointer tekrar kullaniliyor
R010 -> double free tespit edildi
```

Hatalar satır numarası ve açıklama ile birlikte listelenmektedir.

---

# Modern Arayüz Yapısı

GUI sistemi klasik konsol görüntüsünden uzaklaşarak daha modern bir masaüstü deneyimi sunmayı hedeflemektedir.

Arayüzde;

- Koyu tema yapısı,
- Modern font kullanımı,
- Yuvarlatılmış görsel alanlar,
- Daha okunabilir hata listeleri,
- Daha düzenli pencere yerleşimi

kullanılmıştır.

---

# Responsive Yapı

Pencere boyutu değiştirildiğinde GUI bileşenleri otomatik olarak yeniden konumlandırılmaktadır.

Bu sayede:

- Tam ekran kullanım,
- Farklı çözünürlük desteği,
- Daha stabil görüntü

sağlanmaktadır.

---

# Backend Bağlantısı

GUI sistemi doğrudan aşağıdaki modüller ile iletişim kurmaktadır:

- analysis_engine
- syntax_analyzer
- pointer
- reporting

GUI yalnızca görsel katman görevini üstlenmektedir.

Analiz işlemleri backend tarafında gerçekleştirilmektedir.

---

# Build Sistemi

GUI sistemi CMake ile derlenmektedir.

Desktop uygulaması için kullanılan target:

```text
CodeSentinelDesktop
```

CLI sürümü için ayrı target bulunmaktadır:

```text
CodeSentinelCLI
```

---

# Genel Değerlendirme

GUI modülü, Code Sentinel projesinin kullanıcı ile etkileşim kuran ana katmanıdır.

Bu yapı sayesinde sistem:

- Daha erişilebilir,
- Daha kullanıcı dostu,
- Daha profesyonel,
- Daha görsel

bir hale getirilmiştir.

WinAPI tabanlı yapı sayesinde ek framework bağımlılığı olmadan doğrudan Windows üzerinde çalışabilmektedir.

---

# Geliştirilebilecek Noktalar

- Tema sistemi
- Çoklu dosya analizi
- Gerçek zamanlı analiz
- Kod editörü entegrasyonu
- Grafiksel hata gösterimleri
- Syntax highlighting
- Export ekranları
- İstatistik panelleri
- Daha gelişmiş responsive yapı