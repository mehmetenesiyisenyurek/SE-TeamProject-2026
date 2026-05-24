# Pointer Analiz Modülü

## Genel Amaç

Bu klasör, Code Sentinel projesinin pointer ve bellek güvenliği analizlerinden sorumlu bölümüdür.

Pointer modülü; C projelerinde sık karşılaşılan bellek yönetimi hatalarını statik analiz yöntemiyle tespit etmek amacıyla geliştirilmiştir.

Sistem, kaynak kodu çalıştırmadan analiz ederek potansiyel riskleri belirler ve kullanıcıya hata raporları üretir.

Bu modül özellikle;

- Bellek sızıntıları,
- Hatalı free işlemleri,
- Geçersiz pointer kullanımları,
- NULL kontrol eksikleri,
- Bellek güvenliği problemleri

üzerinde yoğunlaşmaktadır.

---

# Klasör Yapısı

```text
pointer/
│
├── NullDereferenceRule.h
├── NullDereferenceRule.cpp
│
├── UseAfterFreeRule.h
├── UseAfterFreeRule.cpp
│
├── MemoryLeakRule.h
├── MemoryLeakRule.cpp
│
├── DoubleFreeRule.h
├── DoubleFreeRule.cpp
│
├── DanglingPointerRule.h
├── DanglingPointerRule.cpp
│
├── MetricCalculator.h
└── MetricCalculator.cpp
```

---

# Çalışma Mantığı

Pointer analiz sistemi AST (Abstract Syntax Tree) yapısı üzerinden çalışmaktadır.

Kod analiz süreci aşağıdaki gibidir:

```text
Kaynak Kod
   ↓
Lexer
   ↓
Parser
   ↓
AST Oluşturma
   ↓
Pointer Kuralları
   ↓
Diagnostic Üretimi
```

Her kural, AST üzerinde dolaşarak belirli pointer kullanım senaryolarını kontrol eder.

Hatalı bir kullanım tespit edildiğinde sistem `Diagnostic` nesnesi üretir ve kullanıcıya hata bilgisi sunar.

---

# Kullanılan Kurallar

## R007 - Null Dereference Rule

Dosyalar:

```text
NullDereferenceRule.h
NullDereferenceRule.cpp
```

Bu kural;

- `malloc`
- `calloc`
- `realloc`

sonrasında NULL kontrolü yapılıp yapılmadığını analiz eder.

### Amaç

Bellek ayrımı başarısız olduğunda pointer NULL dönebilir. NULL kontrolü yapılmadan pointer kullanılması programın çökmesine neden olabilir.

### Problemli Kullanım

```c
int* ptr = malloc(sizeof(int));
*ptr = 5;
```

### Güvenli Kullanım

```c
int* ptr = malloc(sizeof(int));

if(ptr == NULL){
    return;
}

*ptr = 5;
```

---

## R008 - Use After Free Rule

Dosyalar:

```text
UseAfterFreeRule.h
UseAfterFreeRule.cpp
```

Bu kural, `free()` işleminden sonra pointer’ın tekrar kullanılıp kullanılmadığını kontrol eder.

### Amaç

Serbest bırakılmış belleğe tekrar erişmek tanımsız davranış oluşturur ve ciddi güvenlik problemlerine neden olabilir.

### Problemli Kullanım

```c
free(ptr);
printf("%d", *ptr);
```

Bu durumda pointer artık geçersiz olmasına rağmen tekrar kullanılmaktadır.

---

## R009 - Memory Leak Rule

Dosyalar:

```text
MemoryLeakRule.h
MemoryLeakRule.cpp
```

Bu kural, ayrılan belleğin uygun şekilde `free()` edilip edilmediğini kontrol eder.

### Amaç

Serbest bırakılmayan bellek alanları zamanla bellek tüketimini artırır ve uygulamanın performansını düşürür.

### Problemli Kullanım

```c
int* ptr = malloc(sizeof(int) * 10);
return;
```

Bellek ayrılmış fakat serbest bırakılmamıştır.

### Güvenli Kullanım

```c
int* ptr = malloc(sizeof(int) * 10);

if(ptr == NULL){
    return;
}

free(ptr);
```

---

## R010 - Double Free Rule

Dosyalar:

```text
DoubleFreeRule.h
DoubleFreeRule.cpp
```

Bu kural aynı pointer’ın birden fazla kez `free()` edilmesini kontrol eder.

### Amaç

Aynı belleğin tekrar serbest bırakılması program çökmesine veya heap bozulmasına neden olabilir.

### Problemli Kullanım

```c
free(ptr);
free(ptr);
```

Bu kullanım `double free` hatası oluşturur.

---

## R011 - Dangling Pointer Rule

Dosyalar:

```text
DanglingPointerRule.h
DanglingPointerRule.cpp
```

Bu kural, `free()` işleminden sonra pointer’ın NULL yapılıp yapılmadığını kontrol eder.

### Amaç

Serbest bırakılan pointer hâlâ eski adresi göstermeye devam ederse dangling pointer oluşur.

Bu durum yanlış bellek erişimlerine neden olabilir.

### Problemli Kullanım

```c
free(ptr);
```

### Daha Güvenli Kullanım

```c
free(ptr);
ptr = NULL;
```

---

# MetricCalculator

Dosyalar:

```text
MetricCalculator.h
MetricCalculator.cpp
```

Bu sınıf kod metriklerini hesaplamak için kullanılmaktadır.

Hesaplanan bazı metrikler:

- Toplam satır sayısı
- Kod satırı sayısı
- Boş satır sayısı
- Yorum satırı sayısı
- Fonksiyon sayısı
- Struct sayısı
- Değişken sayısı
- Ortalama fonksiyon uzunluğu

---

# Diagnostic Sistemi

Pointer analiz kuralları hata bulduğunda kullanıcıya belirli hata kodları ile çıktı üretir.

| Kod | Açıklama |
|---|---|
| R007 | NULL kontrolü yapılmamış |
| R008 | Use after free |
| R009 | Memory leak |
| R010 | Double free |
| R011 | Dangling pointer |

Örnek çıktı:

```text
R008 -> free işleminden sonra pointer tekrar kullaniliyor: ptr | Satir: 12
```

---

# Kullanılan Yapılar

Pointer modülünde aşağıdaki yapılar aktif olarak kullanılmaktadır:

- AST tabanlı analiz
- Recursive node traversal
- Token kontrolü
- Function-level analiz
- Pointer takip mekanizması
- Diagnostic üretimi
- Rule-based static analysis

---

# Genel Değerlendirme

Pointer modülü, Code Sentinel projesinin en kritik bileşenlerinden biridir.

Bu sistem sayesinde;

- Bellek güvenliği problemleri,
- Riskli pointer kullanımları,
- Potansiyel çökme senaryoları,
- Güvensiz bellek işlemleri

erken aşamada tespit edilebilmektedir.

Modüler yapı sayesinde yeni pointer kuralları sisteme kolayca eklenebilmektedir.

---

# Geliştirilebilecek Noktalar ve Gelecek Hedefleri

- Pointer alias analizi
- Scope takibi geliştirmeleri
- Control flow analysis
- Daha gelişmiş AST kontrolleri
- False positive azaltımı
- Smart pointer desteği
- Daha kapsamlı realloc analizleri