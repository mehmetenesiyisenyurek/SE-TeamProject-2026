# GUI WinAPI Modülü

## Genel Amaç

`gui_winapi` klasörü, Code Sentinel projesinin Windows masaüstü arayüzünü içeren bölümdür.

Bu modül, kullanıcıların statik kod analiz sistemini grafiksel bir arayüz üzerinden kullanabilmesini sağlamak amacıyla geliştirilmiştir.

Sistem doğrudan WinAPI kullanılarak geliştirilmiştir ve herhangi bir harici GUI frameworkü kullanılmamıştır.

Bu yapı sayesinde uygulama;

- Hafif,
- Hızlı,
- Framework bağımsız,
- Doğrudan Windows API tabanlı

bir masaüstü uygulaması olarak çalışmaktadır.

---

# Kullanılan Teknolojiler

- C++
- WinAPI
- GDI / GDI+
- Windows Controls
- CMake

---

# Modülün Temel Görevleri

GUI WinAPI modülü aşağıdaki işlemlerden sorumludur:

- Pencere oluşturma
- Buton işlemleri
- Dosya seçme ekranı
- Analiz başlatma işlemi
- Sonuçları ekrana yazdırma
- Diagnostic görüntüleme
- Scroll yönetimi
- Responsive pencere yerleşimi
- Tema ve görsel yapı yönetimi

---


# main_winapi.cpp

Bu dosya GUI sisteminin merkezidir.

Aşağıdaki işlemler burada gerçekleştirilmektedir:

- WinMain başlangıç noktası
- Pencere oluşturma
- Window procedure yönetimi
- Mesaj döngüsü
- Buton eventleri
- Analiz işlemini tetikleme
- Sonuç ekranı güncellemeleri

---

# WinAPI Çalışma Mantığı

GUI sistemi Windows mesaj tabanlı mimari ile çalışmaktadır.

Genel süreç:

```text
Uygulama Başlatılır
        ↓
WinMain Çalışır
        ↓
Ana Pencere Oluşturulur
        ↓
Windows Mesaj Döngüsü Başlar
        ↓
Kullanıcı Etkileşimleri Dinlenir
        ↓
Analiz İşlemleri Çalıştırılır
        ↓
Sonuçlar GUI Üzerinde Gösterilir
```

---

# Kullanılan WinAPI Yapıları

GUI sistemi içerisinde aşağıdaki temel WinAPI yapıları kullanılmaktadır:

| Yapı | Görev |
|---|---|
| HWND | Pencere referansı |
| MSG | Windows mesajları |
| WNDCLASS | Pencere sınıfı |
| CreateWindow | Bileşen oluşturma |
| SendMessage | Kontrol yönetimi |
| GetMessage | Mesaj alma |
| DispatchMessage | Mesaj yönlendirme |

---

# Kullanılan Arayüz Bileşenleri

## Button

Kullanıcı işlemlerini başlatmak için kullanılmaktadır.

Örnek:

- Dosya seç
- Analizi başlat
- Çıktıyı temizle

---

## EditText

Dosya yollarını veya kullanıcı girişlerini göstermek için kullanılmaktadır.

---

## RichEdit

Analiz sonuçlarını ve diagnostic çıktıları göstermek için kullanılmaktadır.

Bu alan sayesinde uzun hata çıktıları kaydırılabilir şekilde görüntülenebilmektedir.

---

## Static Text

Başlıklar ve açıklamalar için kullanılmaktadır.

---

# Dosya Seçme Sistemi

GUI sistemi Windows File Dialog kullanmaktadır.

Kullanılan filtre:

```
ofn.lpstrFilter =
L"C/C++ Dosyalari\0*.c\0";
```

Bu filtre sayesinde yalnızca:

- `.c`
  uzantılı dosyalar seçilebilir.

---

# Backend İletişimi

GUI sistemi analiz motoru ile doğrudan bağlantılı çalışmaktadır.

Bağlantı kurulan temel modüller:

- analysis_engine
- syntax_analyzer
- pointer
- reporting

GUI yalnızca kullanıcı etkileşimini yönetmektedir.

Analiz işlemleri backend tarafında gerçekleştirilmektedir.

---

# Diagnostic Gösterimi

Analiz tamamlandıktan sonra hata çıktıları GUI üzerinde gösterilmektedir.

Örnek diagnostic çıktısı:

```text
R007 -> malloc sonrasi NULL kontrolu yapilmamis
R008 -> free sonrasi pointer tekrar kullaniliyor
R010 -> double free tespit edildi
```

Her hata;

- Hata kodu,
- Açıklama,
- Satır bilgisi

ile birlikte kullanıcıya sunulmaktadır.

---

# Modern Arayüz Yapısı

GUI WinAPI modülünde klasik eski Windows görünümünden uzaklaşılmaya çalışılmıştır.

Arayüz içerisinde;

- Modern font kullanımı,
- Koyu tema yapısı,
- Daha okunabilir renkler,
- Düzenli bileşen yerleşimi,
- Daha sade tasarım yaklaşımı

kullanılmıştır.

---

# Responsive Yapı

Pencere boyutu değiştirildiğinde arayüz elemanları yeniden konumlandırılmaktadır.

Bu sayede:

- Tam ekran desteği,
- Daha stabil görünüm,
- Farklı çözünürlük uyumu

sağlanmaktadır.

---

# Build Sistemi

GUI WinAPI sistemi CMake üzerinden derlenmektedir.

Desktop uygulaması için kullanılan target:

```text
CodeSentinelDesktop
```

CLI sürümü için ayrı target bulunmaktadır:

```text
CodeSentinelCLI
```

---

# Avantajlar

Bu yapı sayesinde uygulama:

- Harici framework bağımlılığı olmadan çalışır,
- Daha düşük kaynak tüketir,
- Daha hızlı açılır,
- Windows ile doğrudan uyumlu çalışır,
- Daha taşınabilir hale gelir.

---

# Genel Değerlendirme

GUI WinAPI modülü, Code Sentinel projesinin kullanıcı ile doğrudan etkileşim kuran ana katmanıdır.

Bu modül sayesinde statik analiz sistemi yalnızca konsol tabanlı olmaktan çıkarılmış ve daha profesyonel bir masaüstü uygulamasına dönüştürülmüştür.

Kullanıcı dostu yapı sayesinde analiz sonuçları daha anlaşılır ve erişilebilir hale getirilmiştir.

---
