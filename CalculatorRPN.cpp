#include <stack>
#include <iostream>
#include <tgmath.h>
#include <regex>
#include <assert.h>
#include "CalculatorRPN.hpp"
#include "ParserRPN.hpp"
#include "Exception.hpp"

CalculatorRPN::Result CalculatorRPN::compute(CalculatorRPN::Input &input) {
    std::stack<RationalNumber> stack;

    for (std::string token : input.rpn) {
        if (ParserRPN::isRationalNumber(token)) {
            RationalNumber number = ParserRPN::toRationalNumber(token);
            stack.push(number);
        }
        else if (ParserRPN::isOperator(token)) {
            if (stack.size() < 2)
                throw Exception("Expected more arguments", 0x3);

            RationalNumber rightParam = stack.top();
            stack.pop();
            RationalNumber leftParam = stack.top();
            stack.pop();

            Operator::OpType opType = Operator::getOpType(token);
            Operator op(leftParam, rightParam, opType);
            RationalNumber opResult = op.getResult();
            stack.push(opResult);
        }
        else {
            throw Exception("Unknown token: " + token, 0x4);
        }
    }

    if (stack.size() > 1)
        throw Exception("Operator expected", 0x2);

    RationalNumber expressionResult = stack.top();
    std::string inputLatex = generateLatexMath(input.rpn);

    return {input, inputLatex, expressionResult};
}

std::string CalculatorRPN::generateLatexMath(std::vector<std::string> rpn) {
    std::vector<std::pair<std::string, Operator::OpType>> latexStack;

    for (std::string token : rpn) {
        if (ParserRPN::isRationalNumber(token)) {
            RationalNumber number = ParserRPN::toRationalNumber(token);
            latexStack.push_back({number.toLatexMath(), {Operator::None}});
        }
        else if (ParserRPN::isOperator(token)) {
            std::string latexRight = latexStack.back().first;
            Operator::OpType opRight = latexStack.back().second;
            latexStack.pop_back();

            std::string latexLeft = latexStack.back().first;
            Operator::OpType opLeft = latexStack.back().second;
            latexStack.pop_back();

            bool useLeftBrackets = false;
            bool useRightBrackets = false;

            Operator::OpType opType = Operator::getOpType(token);
            std::string exp;

            switch (opType) {
                case Operator::Adding:
                    if (opRight == Operator::Subtracting)
                        useRightBrackets = true;

                    exp = toLatexMath(latexLeft, latexRight, opType, useLeftBrackets, useRightBrackets);
                    latexStack.push_back({exp, opType});
                    break;

                case Operator::Subtracting:
                    if (opRight == Operator::Adding)
                        useRightBrackets = true;

                    exp = toLatexMath(latexLeft, latexRight, opType, useLeftBrackets, useRightBrackets);
                    latexStack.push_back({exp, opType});
                    break;

                case Operator::Multiplying:
                    if (opLeft == Operator::Adding || opLeft == Operator::Subtracting)
                        useLeftBrackets = true;
                    if (opRight == Operator::Adding || opRight == Operator::Subtracting)
                        useRightBrackets = true;

                    exp = toLatexMath(latexLeft, latexRight, opType, useLeftBrackets, useRightBrackets);
                    latexStack.push_back({exp, opType});
                    break;

                case Operator::Dividing:
                    exp = toLatexMath(latexLeft, latexRight, opType, useLeftBrackets, useRightBrackets);
                    latexStack.push_back({exp, opType});
                    break;

                default:
                    assert(false);
            }
        }
    }

    return latexStack.back().first;
}

std::string CalculatorRPN::toLatexMath(const std::string &leftExp, const std::string &rightExp,
        const Operator::OpType &opType, bool useLeftBrackets, bool useRightBrackets) {
    std::string exp = "";

    RationalNumber rightParam;

    switch (opType) {
        case Operator::OpType::Adding:
        case Operator::OpType::Subtracting:
            exp += !useLeftBrackets ? leftExp : "\\left(" + leftExp + "\\right)";
            exp += " " + Operator::opTypeToString(opType) + " ";
            exp += !useRightBrackets ? rightExp : "\\left(" + rightExp + "\\right)";
            break;

        case Operator::OpType::Multiplying:
            exp += !useLeftBrackets ? leftExp : "\\left(" + leftExp + "\\right)";
            exp += " \\times ";
            exp += !useRightBrackets ? rightExp : "\\left(" + rightExp + "\\right)";
            break;

        case Operator::OpType::Dividing:
            exp = "\\dfrac{" + leftExp + "}{" + rightExp + "}";
            break;

        case Operator::OpType::Exponentiation:
            rightParam = Operator::getRationalNumberLatexMath(rightExp);
            if (!rightParam.isIndeterminate() && !rightParam.isInteger()) {

                int num = rightParam.getNumerator();
                int den = rightParam.getDenominator();

                if (num != 1) {
                    exp += ParserRPN::isNumber(leftExp) ? leftExp : "\\left(" + leftExp + "\\right)";
                    exp += "^{" + rightExp + "}";
                } else {
                    int &root = den;
                    if (root != 2) {
                        exp += "\\sqrt[" + std::to_string((unsigned int) root) + "]";
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