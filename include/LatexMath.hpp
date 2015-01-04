#pragma once

#include <string>
#include <vector>
#include <stack>
#include "RationalNumber.hpp"
#include "Operator.hpp"
#include "ParserRPN.hpp"

class LatexMath {
public:
    struct OperatorEntity {
        Operator::OpType opType;

        struct {
            std::string latex;
            Operator::OpType opType;
            bool useBrackets;
        } left, right;
    };

private:
    LatexMath() {}

public:
    static std::string fromRPN(const std::vector<Token> &rpn);

    static std::string fromNumber(const RationalNumber &number, bool slanted = false);

private:
    static OperatorEntity &appendBracketsToOperatorEntity(OperatorEntity &opEntity);
    static std::pair<std::string, Operator::OpType> generateLatexEntity(const OperatorEntity &opEntity);

    static std::string getNormalFractionsForExponent(std::string exponentLatex);

    static void replace_all(std::string &subject, const std::string &search, const std::string &replace);
};