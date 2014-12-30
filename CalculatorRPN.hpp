#pragma once

#include <string>
#include <vector>
#include "RationalNumber.hpp"
#include "Operator.hpp"

class CalculatorRPN {
public:
    struct Input {
        std::string rawInput;
        std::vector<std::string> tokens;
        std::vector<std::string> rpn;
    };

    struct Result {
        Input input;
        std::string input_latex;
        RationalNumber result;
    };

    static Result compute(Input &input);

    static std::string generateLatexMath(std::vector<std::string> rpn);
    static std::string toLatexMath(const std::string &leftExp, const std::string &rightExp,
            const Operator::OpType &opType, bool useLeftBrackets, bool useRightBrackets);
};