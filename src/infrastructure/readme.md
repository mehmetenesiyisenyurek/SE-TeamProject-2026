# Infrastructure Module

Bu klasör, **CodeSentinel – Statik Kod Analizi ve Hata Tespit Sistemi** projesinin altyapı ve veri katmanı bileşenlerini içerir.

Infrastructure modülü; dosya yükleme, dosya doğrulama, kaynak kod ön işleme, analiz bulgularının temsil edilmesi, metrik verilerinin tutulması ve uyarı seviyelerinin sınıflandırılması gibi temel işlemlerden sorumludur.

Bu modül, analiz sürecinin başlangıç aşamasını oluşturur ve diğer modüllerin güvenilir şekilde çalışabilmesi için gerekli temel veri yapılarını sağlar.

---

## Görev Alanı

Bu modül aşağıdaki sorumlulukları üstlenir:

- C kaynak kod dosyasının sisteme alınması
- Dosya uzantısı, boş içerik, okunabilirlik ve binary dosya kontrollerinin yapılması
- Kaynak kod içerisindeki yorum satırlarının analiz dışı bırakılması
- Hata, uyarı ve bilgi mesajlarının ortak bir veri yapısı ile temsil edilmesi
- Kod metriklerinin saklanması
- Bulgulara önem seviyesi atanması
- Uygulama yaşam döngüsünün temel seviyede yönetilmesi

---

## İçerdiği Yapılar

Bu modülde **1 enum** ve **9 temel sınıf** bulunmaktadır.

---

## DiagnosticSeverity

`DiagnosticSeverity`, analiz sonucunda oluşan bulguların önem seviyesini temsil eden enum yapısıdır.

Desteklenen seviyeler:

- `CRITICAL`: Derleme hatasına veya ciddi çalışma zamanı sorunlarına yol açabilecek hatalar
- `WARNING`: Potansiyel problem veya kötü kod pratiği içeren durumlar
- `INFO`: Bilgilendirme ve stil önerileri

Bu yapı, hata ve uyarıların arayüzde renk kodlarıyla gösterilmesi ve filtrelenmesi için kullanılır.

---

## Diagnostic

`Diagnostic`, analiz sonucunda oluşan tek bir bulguyu temsil eden veri sınıfıdır.

Bir syntax hatası, kural ihlali veya bilgilendirme mesajı bu sınıf üzerinden saklanır.

Tuttuğu temel bilgiler:

- Satır numarası
- Sütun numarası
- Hata veya uyarı mesajı
- Önem seviyesi
- Bulguyu üreten kaynak
- Kural ID bilgisi
- İlgili kod parçası

Bu sınıf sayesinde farklı analiz modüllerinden gelen bulgular ortak bir formatta tutulur.

---

## CodeMetric

`CodeMetric`, analiz sonucunda hesaplanan kod metriklerini temsil eden veri sınıfıdır.

Örnek metrikler:

- Toplam satır sayısı
- Fonksiyon sayısı
- Struct sayısı
- Yorum satırı sayısı
- Boş satır sayısı

Bu sınıf, raporlama ve sonuç gösterimi aşamalarında metrik bilgilerinin düzenli şekilde kullanılmasını sağlar.

---

## SeverityClassifier

`SeverityClassifier`, analiz sonucunda oluşan bulgulara önem seviyesi atayan kontrol sınıfıdır.

Bu sınıf sayesinde sistem genelinde hata seviyeleri tutarlı şekilde belirlenir.

Örnek sınıflandırma:

- Syntax hataları → `CRITICAL`
- Kod kalitesi problemleri → `WARNING`
- Stil önerileri → `INFO`

Ayrıca önem seviyelerine karşılık gelen renk kodları ve etiketler de bu sınıf üzerinden yönetilebilir.

---

## SourceFile

`SourceFile`, kullanıcının sisteme yüklediği C kaynak kod dosyasını temsil eden veri sınıfıdır.

Sakladığı bilgiler:

- Dosya yolu
- Dosya adı
- Dosya içeriği
- Dosya boyutu
- Yüklenme zamanı

Bu sınıf, analiz sürecinde kaynak dosya bilgilerine merkezi şekilde erişilmesini sağlar.

---

## ValidationResult

`ValidationResult`, dosya doğrulama işleminin sonucunu temsil eden veri sınıfıdır.

Tuttuğu bilgiler:

- Dosyanın geçerli olup olmadığı
- Geçersiz dosya durumunda hata mesajı

Bu sınıf sayesinde doğrulama işlemi sonucunda oluşan hata mesajları kullanıcıya anlaşılır şekilde iletilebilir.

---

## FileValidator

`FileValidator`, yüklenen dosyanın analiz için uygun olup olmadığını kontrol eden sınıftır.

Yaptığı temel kontroller:

- Dosya uzantısı `.c` mi?
- Dosya boş mu?
- Dosya okunabilir mi?
- Dosya binary formatta mı?

Dosya geçerli değilse analiz süreci başlatılmaz ve kullanıcıya uygun hata mesajı gösterilir.

---

## FileLoader

`FileLoader`, diskten dosya okuma işlemini gerçekleştiren kontrol sınıfıdır.

Görevleri:

1. Dosya yolunu alır.
2. Dosya içeriğini okur.
3. `SourceFile` nesnesi oluşturur.
4. `FileValidator` ile doğrulama yapar.
5. Dosya geçerliyse analiz sürecine aktarılmasını sağlar.

Bu sınıf, dosya alma işleminin düzenli ve kontrollü şekilde yapılmasını sağlar.

---

## Preprocessor

`Preprocessor`, kaynak kod üzerinde ön işleme yapan sınıftır.

Temel görevi, yorum satırlarını analiz dışı bırakmaktır.

Desteklenen yorum türleri:

- Tek satırlı yorumlar: `// yorum`
- Çok satırlı yorumlar: `/* yorum */`

Önemli nokta:

String literal içerisindeki yorum benzeri ifadeler silinmemelidir.

Örnek:

```c
char *text = "// bu yorum değildir";
```

Bu ifade yorum olarak algılanmamalıdır.

`Preprocessor`, yorumları temizlerken satır numarası bütünlüğünü korumaya dikkat eder. Böylece analiz sonucunda verilen hata satırları kaynak kod ile uyumlu kalır.

---

## Application

`Application`, uygulamanın temel yaşam döngüsünü yöneten kontrol sınıfıdır.

Görevleri:

- Sistemi başlatmak
- Gerekli analiz bileşenlerini oluşturmak
- Analiz sürecini başlatacak temel yapıları hazırlamak
- Uygulama kapanırken kaynakları serbest bırakmak

Bu sınıf, uygulamanın başlangıç ve kapanış süreçlerinin düzenli şekilde yönetilmesini sağlar.

---

## Modülün Analiz Sürecindeki Yeri

Infrastructure modülü, analiz sürecinin ilk aşamasında görev alır.

Genel akış:

```txt
Dosya Seçimi
     ↓
FileLoader
     ↓
FileValidator
     ↓
SourceFile
     ↓
Preprocessor
     ↓
Lexer / Parser / Analyzer modülleri
```

Bu akış sayesinde analiz işlemine yalnızca geçerli, okunabilir ve işlenebilir C kaynak kodları dahil edilir.

---

## Örnek Kullanım

```cpp
FileLoader loader;
SourceFile file = loader.load("example.c");

Preprocessor preprocessor;
std::string cleanCode = preprocessor.process(file.getContent());
```

Bu örnekte dosya okunur, doğrulanır ve yorumlardan temizlenmiş kaynak kod sonraki analiz modüllerine aktarılmaya hazır hale getirilir.

---

## Amaç

Infrastructure modülünün amacı, sistemin geri kalan analiz bileşenlerine temiz, doğrulanmış ve güvenilir veri sağlamaktır.

Bu modül doğru çalışmadığında lexer, parser, syntax analyzer, rule engine ve raporlama modülleri hatalı veya eksik veriyle çalışabilir. Bu nedenle infrastructure katmanı projenin temel yapı taşlarından biridir.
