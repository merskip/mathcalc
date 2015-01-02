#pragma once

/*
 * bity  0-2: Numer typu w jego klasie
 * bit     3: Wiązanie (0 - lewo, 1 - prawo)
 * bity  4-7: Priorytet dla operatorów
 * bity 8-15: Klasa tokenu
 */
enum TokenType : int {
    Unknown        = 0x8000, // 1000 0000 0000 0000
    Number         = 0x0000, // 0000 0000 0000 0000
    Positive       = 0x0148, // 0000 0001 0100 1000
    Negative       = 0x0149, // 0000 0001 0100 1001
    Adding         = 0x0210, // 0000 0010 0001 0000
    Subtracting    = 0x0211, // 0000 0010 0001 0001
    Multiplying    = 0x0420, // 0000 0100 0010 0000
    Dividing       = 0x0421, // 0000 0100 0010 0001
    Exponentiation = 0x0848, // 0000 1000 0100 1000
    LeftBracket    = 0x1000, // 0001 0000 0000 0000
    RightBracket   = 0x1001  // 0001 0000 0000 0001
};

enum TokenBiding : int {
    Left  = 0x0000, // 0000 0000 0000 0000
    Right = 0x0008, // 0000 0000 0000 1000
};

const static int TOKEN_MASK_NEG_POS       = 0x0100; // 0000 0001 0000 0000
const static int TOKEN_MASK_ADD_SUB       = 0x0200; // 0000 0010 0000 0000
const static int TOKEN_MASK_MULTI_DIVISON = 0x0400; // 0000 0100 0000 0000
const static int TOKEN_MASK_POW           = 0x0800; // 0000 1000 0000 0000
const static int TOKEN_MASK_BRACKET       = 0x1000; // 0001 0000 0000 0000
const static int TOKEN_MASK_BINDING       = 0x0008; // 0000 0000 0000 1000
const static int TOKEN_MASK_PRIORITY      = 0x00F0; // 0000 0000 1111 0000
const static int TOKEN_MASK_IS_OPERATOR   = 0x0F00; // 0000 1111 0000 0000
const static int TOKEN_MASK_CLASS         = 0xFF00; // 1111 1111 0000 0000

struct Token {
    std::string text;
    TokenType type;
};

inline std::string toString(const Token &token) {
    switch (token.type) {
        case Unknown: return "?";
        case Number: return token.text;
        case Negative: return "-`";
        case Positive: return "+`";
        case Adding: return "+";
        case Subtracting: return "-";
        case Multiplying: return "*";
        case Dividing: return "/";
        case Exponentiation: return "^";
        case LeftBracket: return "(";
        case RightBracket: return ")";
    }
}