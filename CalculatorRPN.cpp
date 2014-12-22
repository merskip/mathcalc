#include <stack>
#include <iostream>
#include <tgmath.h>
#include <regex>
#include "CalculatorRPN.hpp"
#include "ParserRPN.hpp"
#include "Operator.hpp"
#include "Exception.hpp"

CalculatorRPN::Result CalculatorRPN::compute(CalculatorRPN::Input &input) {
    std::stack<std::string> latexStack;
    std::stack<RationalNumber> stack;

    for (std::string token : input.rpn) {
        if (ParserRPN::isRationalNumber(token)) {
            RationalNumber number = ParserRPN::toRationalNumber(token);
            stack.push(number);
            latexStack.push(number.toLatexMath());
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

            std::string l_right = latexStack.top();
            latexStack.pop();
            std::string l_left = latexStack.top();
            latexStack.pop();

            std::string exp = op.toLatexMath(l_left, l_right, opType);
            latexStack.push(exp);
        }
        else {
            throw Exception("Unknown token: " + token, 0x4);
        }
    }

    if (stack.size() > 1)
        throw Exception("Operator expected", 0x2);

    RationalNumber expressionResult = stack.top();
    std::string inputLatex = latexStack.top();

    return {input, inputLatex, expressionResult};
}
