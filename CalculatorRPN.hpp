#pragma once

#include <string>
#include <vector>
#include "RationalNumber.hpp"

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
};