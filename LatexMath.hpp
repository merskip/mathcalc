#pragma once

#include <string>
#include <vector>
#include <stack>
#include "RationalNumber.hpp"
#include "Operator.hpp"

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
    static std::string fromRPN(const std::vector<std::string> &rpn);

    static std::string fromNumber(const RationalNumber &number);

private:
    static OperatorEntity &appendBracketsToOperatorEntity(OperatorEntity &opEntity);
    static std::pair<std::string, Operator::OpType> generateLatexEntity(const OperatorEntity &opEntity);
};