#pragma once

#include <string>
#include "RationalNumber.hpp"

class Operator {
public:
    enum OpType {
        None = 0x00,            // b0000
        Adding = 0x02,          // b0010
        Subtracting = 0x03,     // b0011
        Multiplying = 0x04,     // b0100
        Dividing = 0x05,        // b0101
        Exponentiation = 0x08,  // b1000
    };

    const static int OP_MASK_ADD_SUB = 0x02;       // b0010
    const static int OP_MASK_MULTI_DIVISON = 0x04; // b0100
    const static int OP_MASK_POW = 0x08;           // b1000
    const static int OP_MASK_PRIORITY = 0x0E;      // b1110

private:
    RationalNumber leftParam;
    RationalNumber rightParam;
    OpType opType;

public:
    Operator();
    Operator(RationalNumber leftParam, RationalNumber rightParam, Operator::OpType opType);

    RationalNumber getResult();


    static bool isSingleLatexExpression(const std::string expression);
    static RationalNumber getRationalNumberLatexMath(const std::string &latexMath);

    static OpType getOpType(const std::string &s);
    static std::string opTypeToString(const OpType &opType);
};

