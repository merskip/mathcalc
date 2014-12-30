#include <assert.h>
#include <regex>
#include <iostream>
#include "Operator.hpp"
#include "ParserRPN.hpp"

Operator::Operator() {
}

Operator::Operator(RationalNumber leftParam, RationalNumber rightParam, Operator::OpType opType)
        : leftParam(leftParam),
          rightParam(rightParam),
          opType(opType) {
}

RationalNumber Operator::getResult() {
    switch (opType) {
        case Adding: return leftParam + rightParam;
        case Subtracting: return leftParam - rightParam;
        case Dividing: return leftParam / rightParam;
        case Multiplying: return leftParam * rightParam;
        case Exponentiation: return RationalNumber::pow(leftParam, rightParam);
        default: assert(false);
    }
}

RationalNumber Operator::getRationalNumberLatexMath(const std::string &latexMath) {
    if (ParserRPN::isNumber(latexMath))
        return ParserRPN::toRationalNumber(latexMath);

    std::smatch match;
    std::regex regex("^\\\\frac\\{(\\d+)\\}\\{(\\d+)\\}$");
    if (!std::regex_match(latexMath, match, regex))
        return RationalNumber();

    int num = std::stoi(match[1]);
    int den = std::stoi(match[2]);
    return RationalNumber(num, den);
}

Operator::OpType Operator::getOpType(const std::string &s) {
    if (s == "+") return OpType::Adding;
    else if (s == "-") return OpType::Subtracting;
    else if (s == "*") return OpType::Multiplying;
    else if (s == "/") return OpType::Dividing;
    else if (s == "^") return OpType::Exponentiation;
    else assert(false);
}


std::string Operator::opTypeToString(const Operator::OpType &opType) {
    switch (opType) {
        case None: return "∅";
        case Adding: return "+";
        case Subtracting: return "-";
        case Multiplying: return "*";
        case Dividing: return "/";
        case Exponentiation: return "^";
        default: assert(false);
    }
}
