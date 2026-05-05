
// Created by Mehmet Enes İyişenyürek on 5.05.2026.


#ifndef ASTNODETYPE_H
#define ASTNODETYPE_H
// Yazılan kodun sınıfını belirler.
enum class ASTNodeType {
    PROGRAM,         // Kök düğüm
    FUNCTION_DEF,    // Fonksiyon tanımı
    VARIABLE_DECL,   // Değişken tanımı
    IF_STATEMENT,    // Koşul ifadesi
    WHILE_LOOP,      // While döngüsü
    FOR_LOOP,        // For döngüsü
    RETURN_STMT,     // Return ifadesi
    ASSIGNMENT,      // Değer ataması
    EXPRESSION,      // Genel ifade
    BINARY_OP,       // İkili operatör
    FUNCTION_CALL,   // Fonksiyon çağrısı
    STRUCT_DEF,      // Struct tanımı
    BLOCK            // Kod bloğu : if, for, while vb.
};

#endif //ASTNODETYPE_H
