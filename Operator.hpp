#pragma once

#include <string>
#include "RationalNumber.hpp"
#include "ParserRPN.hpp"

class Operator {
public:
    enum OpType {
        None            = 0x00, // 0000 0000
        Negative        = 0x10, // 0001 0000
        Positive        = 0x11, // 0001 0001
        Adding          = 0x20, // 0010 0000
        Subtracting     = 0x21, // 0010 0001
        Multiplying     = 0x40, // 0100 0000
        Dividing        = 0x41, // 0100 0001
        Exponentiation  = 0x80  // 1000 0000
    };

    const static int OP_MASK_NEG_POS       = 0x10; // 0001 0000
    const static int OP_MASK_ADD_SUB       = 0x20; // 0010 0000
    const static int OP_MASK_MULTI_DIVISON = 0x40; // 0100 0000
    const static int OP_MASK_POW           = 0x80; // 1000 0000
    const static int OP_MASK_PRIORITY      = 0xF0; // 1111 0000

private:
    RationalNumber leftParam;
    RationalNumber rightParam;
    OpType opType;

public:
    Operator();
    Operator(RationalNumber leftParam, RationalNumber rightParam, Operator::OpType opType);

    RationalNumber getResult();

    static OpType getOpType(const Token &token);
    static std::string opTypeToString(const OpType &opType);
};

