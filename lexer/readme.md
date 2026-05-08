# Lexer Module

Bu klasör CodeSentinel projesinin sözcüksel analiz (lexical analysis) katmanını içerir.

## İçerik

### TokenType
Token kategorilerini tanımlar.

### Token
Tek bir lexical token nesnesini temsil eder.

### Lexer
Kaynak kodu karakter karakter tarar ve token listesi üretir.

## Desteklenen Token Türleri

- KEYWORD
- IDENTIFIER
- INT_LITERAL
- FLOAT_LITERAL
- STRING_LITERAL
- CHAR_LITERAL
- OPERATOR
- PUNCTUATION
- PREPROCESSOR
- UNKNOWN

## Özellikler

- Satır/sütun takibi
- Hex literal desteği
- Float literal desteği
- String escape desteği
- Çok karakterli operatör desteği
- Preprocessor directive desteği
- UNKNOWN token üretimi

## Örnek

Giriş:

```c
int x = 42;
```
Çıkış:

```c
KEYWORD(int)
IDENTIFIER(x)
OPERATOR(=)
INT_LITERAL(42)
PUNCTUATION(;)
```