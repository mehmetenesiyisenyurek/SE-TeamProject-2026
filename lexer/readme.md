# Lexer Module Documentation

## Genel Bakış

Bu klasör CodeSentinel projesinin lexical analysis (sözcüksel analiz) katmanını ve temel static analysis kurallarını içermektedir.

Bu modülün temel amacı C/C++ kaynak kodlarını karakter düzeyinde incelemek, kodu anlamlı token parçalarına ayırmak ve daha sonra gerçekleştirilecek syntax analiz ile statik kod analiz işlemleri için gerekli veri yapısını oluşturmaktır.

Ek olarak bu klasörde bazı temel kod kalite kurallarının (Rule System) implementasyonları bulunmaktadır.

Bu modül sistem içerisinde aşağıdaki akışta çalışır:

```text
Source Code
      ↓
Lexer
      ↓
Token List
      ↓
Parser
      ↓
AST
      ↓
Rule Analysis
      ↓
Diagnostics
```

---

# Klasör İçeriği

```text
lexer/

    TokenType.h
    Token.h
    Token.cpp

    Lexer.h
    Lexer.cpp

    MagicNumberRule.h
    MagicNumberRule.cpp

    FunctionLengthRule.h
    FunctionLengthRule.cpp

    GotoUsageRule.h
    GotoUsageRule.cpp
```

---

# Dosyaların Ayrıntılı Açıklamaları

---

## TokenType.h

Bu dosya sistemde kullanılan token türlerini tanımlayan enum yapısını içerir.

Amaç:

Kaynak koddan elde edilen her parçanın hangi kategoriye ait olduğunu belirlemektir.

Tanımlanan token türleri:

| Token Türü | Açıklama |
|---|---:|
| KEYWORD | C/C++ anahtar kelimeleri |
| IDENTIFIER | Değişken veya fonksiyon isimleri |
| INT_LITERAL | Tamsayı sabitleri |
| FLOAT_LITERAL | Ondalıklı sayılar |
| STRING_LITERAL | String ifadeleri |
| CHAR_LITERAL | Karakter sabitleri |
| OPERATOR | İşleçler |
| PUNCTUATION | Noktalama karakterleri |
| PREPROCESSOR | Preprocessor komutları |
| UNKNOWN | Tanımlanamayan ifadeler |

Örnek:

```c
int age=20;
```

Üretilen tokenler:

```text
KEYWORD
IDENTIFIER
OPERATOR
INT_LITERAL
PUNCTUATION
```

---

## Token.h

Bu dosya Token sınıfını tanımlar.

Token sınıfı kaynak koddan elde edilen tek bir sözcüğü temsil eder.

Sınıf içerisindeki değişkenler:

```cpp
TokenType type
std::string value
int line
int column
```

Açıklamalar:

### type

Token türünü belirtir.

Örnek:

```text
KEYWORD
IDENTIFIER
```

### value

Token'ın gerçek değeridir.

Örnek:

```text
main
100
if
```

### line

Kod içerisindeki satır numarasıdır.

### column

Kod içerisindeki sütun numarasıdır.

---

## Token.cpp

Bu dosya Token sınıfının implementasyonlarını içerir.

İçerdiği işlemler:

### Constructor

Token oluşturur.

Örnek:

```cpp
Token(
    TokenType::KEYWORD,
    "int",
    5,
    8
);
```

### Getter Fonksiyonları

- getType()

- getValue()

- getLine()

- getColumn()

Bu fonksiyonlar token bilgilerine erişmek için kullanılır.

---

## Lexer.h

Bu dosya Lexer sınıfının bildirimlerini içerir.

Lexer sınıfı kaynak kodu karakter karakter inceleyen temel analiz sınıfıdır.

Temel görevleri:

- Kaynak kodu okumak
- Token üretmek
- Satır ve sütun takibi yapmak
- Geçersiz karakterleri belirlemek

Önemli fonksiyonlar:

### tokenize()

Kaynak kodu token listesine dönüştürür.

```cpp
std::vector<Token> tokenize(
    const std::string& code
);
```

---

### advance()

Bir sonraki karaktere geçer.

---

### peek()

Mevcut karakteri okumayı sağlar.

---

### skipWhitespace()

Boşluk karakterlerini atlar.

---

### readKeywordOrIdentifier()

Keyword veya identifier okur.

---

### readNumericLiteral()

Sayıları okur.

Desteklenen:

- Integer
- Float
- Hexadecimal

---

### readStringLiteral()

String ifadelerini işler.

Örnek:

```c
"Hello World"
```

---

### readCharLiteral()

Karakter ifadelerini işler.

Örnek:

```c
'A'
```

---

### readOperator()

Operatörleri işler.

Desteklenen örnekler:

```c
+
-
*
/
==
!=
>=
<=
++
--
&&
||
```

---

## Lexer.cpp

Bu dosya Lexer sınıfının gerçek implementasyonlarını içerir.

Gerçekleştirilen işlemler:

### Karakter Bazlı Tarama

Lexer kaynak kodu soldan sağa tarar.

Örnek:

Kod:

```c
int x=5;
```

İşlem:

```text
i → n → t
↓
KEYWORD(int)

x
↓
IDENTIFIER(x)

=
↓
OPERATOR

5
↓
INT_LITERAL

;
↓
PUNCTUATION
```

---

### Satır ve Sütun Takibi

Lexer hata raporları için satır bilgisi tutar.

Örnek:

```c
1: int x;
2: x=10;
```

Hata:

```text
Unknown character at line 2 column 5
```

---

### Çok Karakterli Operatör Analizi

Lexer aşağıdaki operatörleri algılayabilir:

```c
==
!=
<=
>=
++
--
+=
-=
*=
/=
&&
||
->
```

---

### Preprocessor Desteği

Örnek:

```c
#include<stdio.h>
#define PI 3.14
```

Üretilen token:

```text
PREPROCESSOR
```

---

# Static Analysis Rule Sistemi

Bu klasör ayrıca temel static code analysis kurallarını içerir.

---

## MagicNumberRule

Dosyalar:

```text
MagicNumberRule.h
MagicNumberRule.cpp
```

Amaç:

Kod içerisinde açıklanmamış sabit sayı kullanımını tespit etmek.

Örnek:

Kötü kullanım:

```c
salary=5678;
```

Önerilen:

```c
const int DEFAULT_SALARY=5678;
salary=DEFAULT_SALARY;
```

Algoritma:

1. AST dolaşılır
2. Sayısal literal bulunur
3. Ignore listesi kontrol edilir
4. Diagnostic oluşturulur

Ignore edilen değerler:

```text
0
1
-1
```

---

## FunctionLengthRule

Dosyalar:

```text
FunctionLengthRule.h
FunctionLengthRule.cpp
```

Amaç:

Aşırı uzun fonksiyonları tespit etmek.

Varsayılan limit:

```cpp
50 satır
```

Algoritma:

1. AST taranır
2. Function node bulunur
3. Satır uzunluğu hesaplanır
4. Limit aşılırsa uyarı oluşturulur

---

## GotoUsageRule

Dosyalar:

```text
GotoUsageRule.h
GotoUsageRule.cpp
```

Amaç:

Kod içerisinde goto kullanımını tespit etmek.

Örnek:

```c
goto errorLabel;
```

Neden önerilmez:

- Kod okunabilirliğini azaltır
- Karmaşıklığı artırır
- Bakımı zorlaştırır

Algoritma:

1. AST dolaşılır
2. GOTO_STATEMENT node aranır
3. Diagnostic üretilir

---

# Kullanılan Yazılım Mühendisliği Yaklaşımları

Bu modül geliştirilirken:

- Object Oriented Programming
- SOLID Principles
- Separation of Concerns
- Modular Design
- Reusable Components
- Recursive Tree Traversal
- Extensible Architecture

yaklaşımları kullanılmıştır.

---

# Gelecekte Planlanan Geliştirmeler

- Comment token desteği
- Multi-line string desteği
- Daha gelişmiş operator analizi
- Rule sayısının artırılması
- Performans optimizasyonları
- Multi-thread analiz desteği
- Daha gelişmiş hata raporlama sistemi
- Pointer analiz kuralları
- Memory leak detection