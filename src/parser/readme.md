# CodeSentinel — Parser, ASTNode, ASTNodeType & InfiniteLoopRule

> Bu doküman; **ASTNodeType** enum'unu, **ASTNode** veri yapısını, **Parser** sınıfını ve **InfiniteLoopRule (R006)** kural sınıfını açıklamaktadır.

---

## İçindekiler

1. [Genel Bakış](#1-genel-bakış)
2. [ASTNodeType (enum)](#2-astnodetype-enum)
3. [ASTNode (entity)](#3-astnode-entity)
4. [Parser (control)](#4-parser-control)
5. [InfiniteLoopRule — R006 (entity)](#5-infinitelooprule--r006-entity)
6. [Dosya Yapısı](#6-dosya-yapısı)
7. [Derleme](#7-derleme)
8. [Kullanım Örneği](#8-kullanım-örneği)

---

## 1. Genel Bakış

CodeSentinel'in analiz akışında bu dört bileşen şu şekilde konumlanır:

```
Kaynak Kod
    │
    ▼
 [Preprocessor]  →  Yorumsuz kod
    │
    ▼
 [Lexer]         →  Token[]
    │
    ▼
 [Parser]        →  ASTNode ağacı (ASTNodeType ile tiplendirilmiş)
    │
    ▼
 [InfiniteLoopRule + Diğer Kurallar]  →  Diagnostic[]
```

| Bileşen | Rol | Girdi | Çıktı |
|---------|-----|-------|-------|
| **ASTNodeType** | AST düğüm türlerini tanımlayan enum | — | — |
| **ASTNode** | AST ağacındaki tek bir düğümü temsil eden veri yapısı | — | — |
| **Parser** | Token dizisini alıp AST ağacı oluşturan sözdizimsel analizci | `vector<Token>` | `ASTNode*` (kök) |
| **InfiniteLoopRule** | AST üzerinde sonsuz döngü kalıplarını tespit eden kural | `ASTNode*` | `vector<Diagnostic>` |

---

## 2. ASTNodeType (enum)

**Dosya:** `include/ASTNodeType.h`

AST düğümlerinin türünü tanımlar. Parser düğüm oluştururken tip atar; analiz motorları (SyntaxAnalyzer, IRule kuralları, MetricCalculator) düğüm tipine göre farklı kontroller yapar.

| Değer | Anlam | Örnek C Kodu |
|-------|-------|--------------|
| `PROGRAM` | Kök düğüm — tüm programı kapsar | Dosyanın tamamı |
| `FUNCTION_DEF` | Fonksiyon tanımı | `int main() { ... }` |
| `VARIABLE_DECL` | Değişken tanımı | `int x = 5;` |
| `IF_STATEMENT` | Koşul ifadesi | `if (x > 0) { ... }` |
| `WHILE_LOOP` | While döngüsü | `while (i < n) { ... }` |
| `FOR_LOOP` | For döngüsü | `for (i=0; i<n; i++) { ... }` |
| `RETURN_STMT` | Return ifadesi | `return 0;` |
| `ASSIGNMENT` | Değer ataması | `x = 10;` |
| `EXPRESSION` | Genel ifade (literal, değişken referansı vb.) | `x + y * 2` |
| `BINARY_OP` | İkili operatör | `a + b`, `x == y` |
| `FUNCTION_CALL` | Fonksiyon çağrısı | `printf("hello")`, `malloc(...)` |
| `STRUCT_DEF` | Struct tanımı | `struct Point { int x; int y; };` |
| `BLOCK` | Kod bloğu (fonksiyon/if/döngü gövdesi) | `{ ... }` |

### Yardımcı Fonksiyon

```cpp
inline std::string astNodeTypeToString(ASTNodeType type);
```

Debug ve loglama amacıyla `ASTNodeType` değerini okunabilir bir string'e çevirir (örn: `ASTNodeType::FUNCTION_DEF` → `"FUNCTION_DEF"`).

---

## 3. ASTNode (entity)

**Dosyalar:** `include/ASTNode.h` — `src/ASTNode.cpp`

Soyut Sözdizim Ağacı'ndaki (AST) **tek bir düğümü** temsil eder. Özyinelemeli `children` yapısı sayesinde kaynak kodun tam hiyerarşik modelini tutar. **Parser** tarafından üretilir; **SyntaxAnalyzer**, tüm **IRule** kuralları ve **MetricCalculator** tarafından gezilerek kullanılır.

### Özellikler

| İsim | Tip | Erişim | Açıklama |
|------|-----|--------|----------|
| `type` | `ASTNodeType` | private | Düğüm türü (FUNCTION_DEF, VARIABLE_DECL vb.) |
| `value` | `string` | private | Düğümün değeri — fonksiyon adı, değişken adı, operatör veya literal |
| `line` | `int` | private | Kaynak koddaki satır numarası (1'den başlar) |
| `children` | `vector<ASTNode*>` | private | Alt düğüm listesi (özyinelemeli ağaç yapısı) |
| `parent` | `ASTNode*` | private | Üst düğüm işaretçisi (kök düğümde `nullptr`) |

### Metotlar

| Metot | İmza | Açıklama |
|-------|------|----------|
| **Constructor** | `ASTNode(ASTNodeType type, const string& value = "", int line = 0)` | Düğüm oluşturur. `parent` başlangıçta `nullptr`'dır; `addChild` çağrıldığında otomatik atanır. |
| **Destructor** | `~ASTNode()` | Tüm child düğümleri özyinelemeli olarak siler. Kök silindiğinde tüm ağaç temizlenir (bellek sızıntısı önlenir). |
| `addChild` | `void addChild(ASTNode* child)` | Alt düğüm ekler ve child'ın `parent` işaretçisini `this` yapar. |
| `getChildren` | `const vector<ASTNode*>& getChildren() const` | Alt düğüm listesini const referans olarak döndürür (kopyalama maliyeti yok). |
| `getType` | `ASTNodeType getType() const` | Düğüm türünü döndürür. |
| `getValue` | `const string& getValue() const` | Düğümün değerini döndürür. |
| `getLine` | `int getLine() const` | Kaynak koddaki satır numarasını döndürür. |
| `getParent` | `ASTNode* getParent() const` | Üst düğümü döndürür (kök'te `nullptr`). |
| `toString` | `string toString(int indent = 0) const` | Ağacı girintili formatta string olarak döndürür (debug/test amaçlı). |

### Örnek AST Çıktısı

Aşağıdaki C kodu:

```c
int main() {
    int x = 5;
    if (x > 0) {
        return 1;
    }
    return 0;
}
```

Şu AST yapısına dönüşür:

```
PROGRAM
  FUNCTION_DEF (value: "main", line: 1)
    BLOCK (line: 1)
      VARIABLE_DECL (value: "x", line: 2)
        EXPRESSION (value: "5", line: 2)
      IF_STATEMENT (line: 3)
        BINARY_OP (value: ">", line: 3)
          EXPRESSION (value: "x", line: 3)
          EXPRESSION (value: "0", line: 3)
        BLOCK (line: 3)
          RETURN_STMT (line: 4)
            EXPRESSION (value: "1", line: 4)
      RETURN_STMT (line: 6)
        EXPRESSION (value: "0", line: 6)
```

### Kurallar ASTNode'u Nasıl Kullanır?

Kurallar ağacı özyinelemeli gezerek ilgili düğümleri arar:

```cpp
void collectVars(ASTNode* node, vector<string>& vars) {
    if (node->getType() == ASTNodeType::VARIABLE_DECL) {
        vars.push_back(node->getValue());
    }
    for (ASTNode* child : node->getChildren()) {
        collectVars(child, vars);
    }
}
```

---

## 4. Parser (control)

**Dosyalar:** `include/Parser.h` — `src/Parser.cpp`

Token dizisini alarak **Recursive Descent (Özyinelemeli İniş)** yöntemiyle AST oluşturur. Ciddi syntax hatalarında bile `synchronize()` mekanizmasıyla kurtarma yapar ve **kısmi AST** üretir.

**Bağımlılıklar:** `Token`, `TokenType`, `ASTNode`, `ASTNodeType`, `Diagnostic`, `DiagnosticSeverity`

### Özellikler

| İsim | Tip | Erişim | Açıklama |
|------|-----|--------|----------|
| `tokens` | `vector<Token>` | private | Lexer'dan gelen token listesi |
| `pos` | `int` | private | Şu anki token pozisyonu |
| `root` | `ASTNode*` | private | AST kök düğümü |
| `diagnostics` | `vector<Diagnostic>` | private | Ayrıştırma sırasında bulunan hatalar |

### Public Metotlar

| Metot | İmza | Açıklama |
|-------|------|----------|
| `parse` | `ASTNode* parse(const vector<Token>& tokenList)` | **Ana metot.** PROGRAM kök düğümü oluşturur, `parseProgram()` ile doldurur ve kökü döndürür. |
| `getDiagnostics` | `const vector<Diagnostic>& getDiagnostics() const` | Ayrıştırma sırasında bulunan hataları döndürür. |

### Yardımcı (Private) Metotlar

| Metot | İmza | Açıklama |
|-------|------|----------|
| `current` | `Token current() const` | Şu anki token'ı döndürür (pozisyon değişmez). |
| `advance` | `Token advance()` | Şu anki token'ı döndürür ve pozisyonu 1 ilerletir. |
| `expect` | `Token expect(TokenType type)` | Beklenen tipteki token'ı döndürür; eşleşmezse `diagnostics`'e hata ekler. |
| `match` | `bool match(TokenType type)` | Eşleşirse `true` + ilerle, eşleşmezse `false` (hata üretmez). |
| `matchValue` | `bool matchValue(const string& val)` | Değere göre eşleştirme yapar. |
| `isAtEnd` | `bool isAtEnd() const` | Token listesinin sonuna gelinip gelinmediğini kontrol eder. |
| `synchronize` | `void synchronize()` | Hata kurtarma: bir sonraki `;` veya `}` karakterine kadar atlar. |
| `reportError` | `void reportError(const string& message)` | Hata mesajını `diagnostics` listesine `CRITICAL` seviyesinde ekler. |
| `isTypeName` | `bool isTypeName(const string& val) const` | Verilen string'in C veri tipi olup olmadığını kontrol eder (`int`, `char`, `float`, `double`, `void`, `long`, `short`, `unsigned`, `signed`, `struct`). |

### Ayrıştırma (Recursive Descent) Fonksiyonları

Her C yapısı için ayrı bir ayrıştırma fonksiyonu vardır:

| C Yapısı | Fonksiyon | Ürettiği ASTNode |
|----------|-----------|------------------|
| Tüm dosya (üst düzey) | `parseProgram()` | PROGRAM (kök) |
| `int f(params) { body }` | `parseFunctionDef(typeName)` | FUNCTION_DEF |
| `{ ... }` | `parseBlock()` | BLOCK |
| Tek bir ifade (if/while/for/return/tanım/atama) | `parseStatement()` | İlgili düğüm |
| `int x = expr;` veya `int *p;` | `parseVarDecl(typeName, isPointer)` | VARIABLE_DECL |
| `if (koşul) { } else { }` | `parseIfStatement()` | IF_STATEMENT |
| `while (koşul) { }` | `parseWhileLoop()` | WHILE_LOOP |
| `for (init; koşul; update) { }` | `parseForLoop()` | FOR_LOOP |
| `return expr;` | `parseReturnStmt()` | RETURN_STMT |
| `struct İsim { üyeler };` | `parseStructDef()` | STRUCT_DEF |

### İfade Ayrıştırma (Operatör Önceliği — Düşükten Yükseğe)

| Öncelik | Fonksiyon | Operatörler |
|---------|-----------|-------------|
| En düşük | `parseExpression()` | Giriş noktası |
| ↓ | `parseAssignmentExpr()` | `=` (sağdan sola birleştirme) |
| ↓ | `parseComparison()` | `==`, `!=`, `<`, `>`, `<=`, `>=` |
| ↓ | `parseAddition()` | `+`, `-` |
| ↓ | `parseMultiplication()` | `*`, `/`, `%` |
| ↓ | `parseUnary()` | `!`, `-`, `*`, `&`, `++` (tekli, özyinelemeli) |
| En yüksek | `parsePrimary()` | Literal, identifier, fonksiyon çağrısı, `sizeof(...)`, `(expr)`, dizi erişimi, postfix `++`/`--` |

### Hata Kurtarma Mekanizması

Parser hata bulduğunda **durmaz**. Hatayı `diagnostics` listesine ekler ve `synchronize()` ile bir sonraki `;` veya `}` karakterine kadar token'ları atlayarak devam eder. Böylece **tek seferde birden fazla hata** raporlanabilir.

---

## 5. InfiniteLoopRule — R006 (entity)

**Dosyalar:** `include/InfiniteLoopRule.h` — `src/InfiniteLoopRule.cpp`

`IRule` arayüzünü implement eder. AST üzerinde potansiyel **sonsuz döngü** yapılarını tespit eder.

**Bağımlılıklar:** `IRule`, `ASTNode`, `Diagnostic`, `DiagnosticSeverity`

### Tespit Edilen Durumlar

| Kalıp | Durum | Açıklama |
|-------|-------|----------|
| `while(1)` | Sabit koşul | Koşul her zaman `true` → döngü hiç bitmez |
| `while(true)` | Sabit koşul | Aynı durum |
| `while(42)` | Sabit koşul | Sıfır olmayan tam sayı → `true` kabul edilir |
| `for(;;)` | Koşul boş | init, koşul, güncelleme hepsi boş |
| `for(int i=0; ; i++)` | Koşul boş | init ve güncelleme var ama koşul yok |

**Seviye:** `WARNING`
**Mesaj formatı:** `"Satır X: Potansiyel sonsuz döngü tespit edildi. ..."`

### Özellikler

| İsim | Tip | Erişim | Açıklama |
|------|-----|--------|----------|
| `ruleId` | `string` | private | `"R006"` |

### IRule Arayüzü İmplementasyonları (Public)

| Metot | Dönüş | Açıklama |
|-------|-------|----------|
| `getId()` | `string` | `"R006"` döndürür |
| `getName()` | `string` | `"Sonsuz Döngü Tespiti"` döndürür |
| `getDefaultSeverity()` | `DiagnosticSeverity` | `WARNING` döndürür |
| `check(ASTNode* ast)` | `vector<Diagnostic>` | AST'nin tamamını gezerek sonsuz döngü ihlallerini toplar ve döndürür |

### Yardımcı (Private) Metotlar

| Metot | İmza | Açıklama |
|-------|------|----------|
| `traverse` | `void traverse(ASTNode* node, vector<Diagnostic>& results) const` | AST'yi derinlik öncelikli (DFS) gezer. `WHILE_LOOP` ve `FOR_LOOP` düğümlerinde kontrolleri çalıştırır, sonra alt düğümlere iner. |
| `isWhileConditionConstant` | `bool isWhileConditionConstant(ASTNode* whileNode) const` | `WHILE_LOOP` düğümünün ilk child'ı (koşul) sabit bir truthy değer mi kontrol eder. |
| `isForConditionEmpty` | `bool isForConditionEmpty(ASTNode* forNode) const` | `FOR_LOOP` düğümünde koşulun boş olup olmadığını belirler. BLOCK öncesi non-BLOCK child sayısına göre karar verir. |
| `isConstantTruthy` | `bool isConstantTruthy(ASTNode* expr) const` | Bir ifadenin "her zaman true" sabit değer olup olmadığını kontrol eder. |

### isConstantTruthy — Karar Mantığı

| Durum | Sonuç | Açıklama |
|-------|-------|----------|
| `EXPRESSION`, value = `"1"` | ✅ `true` | `while(1)` → sonsuz |
| `EXPRESSION`, value = `"true"` veya `"TRUE"` | ✅ `true` | `while(true)` → sonsuz |
| `EXPRESSION`, value = sıfır olmayan tam sayı (ör: `"42"`) | ✅ `true` | `while(42)` → sonsuz |
| `EXPRESSION`, value = `"0"` | ❌ `false` | `while(0)` → döngü hiç çalışmaz |
| `BINARY_OP` | ❌ `false` | `while(x > 0)` → dinamik, analiz edilmez |
| `FUNCTION_CALL` | ❌ `false` | `while(func())` → dinamik |
| `EXPRESSION`, value = değişken adı | ❌ `false` | `while(x)` → dinamik |

### isForConditionEmpty — Karar Mantığı

Parser'ın `FOR_LOOP` düğümüne eklediği child'lar: `[init]`, `[koşul]`, `[güncelleme]`, `BLOCK`.

| Non-BLOCK Child Sayısı | Karar | Açıklama |
|-------------------------|-------|----------|
| 0 | ✅ `true` | `for(;;)` — hiçbir parça yok |
| 1 (VARIABLE_DECL veya ASSIGNMENT) | ✅ `true` | Sadece init var → koşul yok |
| 1 (diğer) | ❌ `false` | Belirsiz → yanlış pozitif önlenir |
| 3 | `isConstantTruthy(2. non-BLOCK child)` | init + koşul + güncelleme mevcut → koşulun sabit olup olmadığı kontrol edilir |
| Diğer | ❌ `false` | Genel durum → false |

---

