#include <assert.h>
#include <regex>
#include <iostream>
#include "Operator.hpp"

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

Operator::OpType Operator::getOpType(const Token &token) {
    switch (token.type) {
        case TokenType::Negative: return OpType::Negative;
        case TokenType::Positive: return OpType::Positive;
        case TokenType::Adding: return OpType::Adding;
        case TokenType::Subtracting: return OpType::Subtracting;
        case TokenType::Multiplying: return OpType::Multiplying;
        case TokenType::Dividing: return OpType::Dividing;
        case TokenType::Exponentiation: return OpType::Exponentiation;
        default: assert(false);
    }
}

std::string Operator::opTypeToString(const Operator::OpType &opType) {
    switch (opType) {
        case None: return "∅";
        case Negative: return "-`";
        case Positive: return "+`";
        case Adding: return "+";
        case Subtracting: return "-";
        case Multiplying: return "*";
        case Dividing: return "/";
        case Exponentiation: return "^";
        default: assert(false);
    }
}
