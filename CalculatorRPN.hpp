#pragma once

#include <string>
#include <vector>
#include "RationalNumber.hpp"
#include "Operator.hpp"
#include "ParserRPN.hpp"

class CalculatorRPN {
public:
    struct Input {
        std::string rawInput;
        std::vector<Token> tokens;
        std::vector<Token> rpn;
    };

    struct Result {
        Input input;
        std::string input_latex;
        RationalNumber result;
    };

    static Result compute(Input &input);
};