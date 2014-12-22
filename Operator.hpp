#pragma once

#include <string>
#include "RationalNumber.hpp"

class Operator {
public:
    enum OpType {
        Adding,
        Subtracting,
        Multiplying,
        Dividing,
        Exponentiation
    };

private:
    RationalNumber leftParam;
    RationalNumber rightParam;
    OpType opType;

public:
    Operator();
    Operator(RationalNumber leftParam, RationalNumber rightParam, Operator::OpType opType);

    RationalNumber getResult();

    static std::string toLatexMath(const std::string &leftExp, const std::string &rightExp, const OpType &opType);
    static bool isSingleLatexExpression(const std::string expression);
    static RationalNumber getRationalNumberLatexMath(const std::string &latexMath);

    static OpType getOpType(const std::string &s);
    static std::string opTypeToString(const OpType &opType);
};

