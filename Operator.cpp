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

std::string Operator::toLatexMath(const std::string &leftExp, const std::string &rightExp, const OpType &opType) {
    std::string exp = "";

    bool singleLeft = isSingleLatexExpression(leftExp);
    bool singleRight = isSingleLatexExpression(rightExp);
    RationalNumber rightParam;

    switch (opType) {
        case Adding:
        case Subtracting:
            exp += singleLeft ? leftExp : "\\left(" + leftExp + "\\right)";
            exp += " " + opTypeToString(opType) + " ";
            exp += singleRight ? rightExp : "\\left(" + rightExp + "\\right)";
            break;

        case Multiplying:
            exp += singleLeft ? leftExp : "\\left(" + leftExp + "\\right)";
            exp += " \\times ";
            exp += singleRight ? rightExp : "\\left(" + rightExp + "\\right)";
            break;

        case Dividing:
            if (singleLeft && singleRight)
                exp = "\\frac{" + leftExp + "}{" + rightExp + "}";
            else
                exp = "\\dfrac{" + leftExp + "}{" + rightExp + "}";
            break;

        case Exponentiation:
            rightParam = getRationalNumberLatexMath(rightExp);
            if (!rightParam.isIndeterminate() && !rightParam.isInteger()) {

                int num = rightParam.getNumerator();
                int den = rightParam.getDenominator();

                if (num != 1) {
                    exp += ParserRPN::isNumber(leftExp) ? leftExp : "\\left(" + leftExp + "\\right)";
                    exp += "^{" + rightExp + "}";
                } else {
                    int &root = den;
                    if (root != 2) {
                        exp += "\\sqrt[" + std::to_string(root) + "]";
                        exp += "{" + leftExp + "}";
                    } else {
                        exp += "\\sqrt[]";
                        exp += "{" + leftExp + "}";
                    }
                }
            } else {
                exp += ParserRPN::isNumber(leftExp) ? leftExp : "\\left(" + leftExp + "\\right)";
                exp += "^{" + rightExp + "}";
            }
            break;

        default:
            assert(false);
    }

    return exp;
}

bool Operator::isSingleLatexExpression(const std::string expression) {
    if (ParserRPN::isNumber(expression))
        return true;

    std::smatch match;
    std::regex regex("^\\S+(?:\\{\\S+?\\})+$");
    return std::regex_search(expression, match, regex);
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
        case Adding: return "+";
        case Subtracting: return "-";
        case Multiplying: return "*";
        case Dividing: return "/";
        case Exponentiation: return "^";
        default: assert(false);
    }
}
