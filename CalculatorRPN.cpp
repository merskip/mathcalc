#include <stack>
#include <iostream>
#include <tgmath.h>
#include <regex>
#include "CalculatorRPN.hpp"
#include "ParserRPN.hpp"
#include "Exception.hpp"
#include "LatexMath.hpp"

CalculatorRPN::Result CalculatorRPN::compute(CalculatorRPN::Input &input) {
    std::stack<RationalNumber> stack;

    for (std::string token : input.rpn) {
        if (ParserRPN::isRationalNumber(token)) {
            RationalNumber number = ParserRPN::toRationalNumber(token);
            stack.push(number);
        }
        else if (token == "~") {
            RationalNumber rightParam = stack.top();
            stack.pop();
            stack.push(rightParam * -1);
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
    std::string inputLatex = LatexMath::fromRPN(input.rpn);

    return {input, inputLatex, expressionResult};
}