
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
// Hata ayıklama ve loglama için eklendi.
inline std::string astNodeTypeToString(ASTNodeType type) {
    switch (type) {
    case ASTNodeType::PROGRAM:       return "PROGRAM";
    case ASTNodeType::FUNCTION_DEF:  return "FUNCTION_DEF";
    case ASTNodeType::VARIABLE_DECL: return "VARIABLE_DECL";
    case ASTNodeType::IF_STATEMENT:  return "IF_STATEMENT";
    case ASTNodeType::WHILE_LOOP:    return "WHILE_LOOP";
    case ASTNodeType::FOR_LOOP:      return "FOR_LOOP";
    case ASTNodeType::RETURN_STMT:   return "RETURN_STMT";
    case ASTNodeType::ASSIGNMENT:    return "ASSIGNMENT";
    case ASTNodeType::EXPRESSION:    return "EXPRESSION";
    case ASTNodeType::BINARY_OP:     return "BINARY_OP";
    case ASTNodeType::FUNCTION_CALL: return "FUNCTION_CALL";
    case ASTNodeType::STRUCT_DEF:    return "STRUCT_DEF";
    case ASTNodeType::BLOCK:         return "BLOCK";
    default:                         return "UNKNOWN";
    }
}

#endif //ASTNODETYPE_H
