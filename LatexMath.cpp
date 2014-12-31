#include <assert.h>
#include <iostream>
#include "LatexMath.hpp"
#include "ParserRPN.hpp"

std::string LatexMath::fromRPN(const std::vector<std::string> &rpn) {
    std::stack<std::pair<std::string, Operator::OpType>> latexStack;

    unsigned int i = 0;
    for (std::string token : rpn) {
        if (ParserRPN::isRationalNumber(token)) {
            RationalNumber number = ParserRPN::toRationalNumber(token);

            std::string number_l = fromNumber(number);

            if (!number.isInteger() && i + 1 < rpn.size()) {
                std::string nextToken = rpn.at(i + 1);
                if (nextToken == "^")
                    number_l = fromNumber(number, true);
            }
            i++;

            if (number.isInteger())
                latexStack.push({number_l, {Operator::None}});
            else
                latexStack.push({number_l, {Operator::Dividing}});
        }
        else if (ParserRPN::isOperator(token)) {
            auto latexRight = latexStack.top();
            latexStack.pop();
            auto latexLeft = latexStack.top();
            latexStack.pop();

            OperatorEntity opEntity;
            opEntity.opType = Operator::getOpType(token);
            opEntity.left = {latexLeft.first, latexLeft.second, false};
            opEntity.right = {latexRight.first, latexRight.second, false};
            opEntity = appendBracketsToOperatorEntity(opEntity);

            auto latexEntity = generateLatexEntity(opEntity);
            latexStack.push(latexEntity);
        }
    }

    return latexStack.top().first;
}

LatexMath::OperatorEntity &LatexMath::appendBracketsToOperatorEntity(OperatorEntity &opEntity) {
    switch (opEntity.opType) {
        case Operator::Adding:
            if (opEntity.right.opType == Operator::Subtracting)
                opEntity.right.useBrackets = true;
            break;

        case Operator::Subtracting:
            if (opEntity.right.opType == Operator::Adding)
                opEntity.right.useBrackets = true;
            break;

        case Operator::Multiplying:
            if (opEntity.left.opType & Operator::OP_MASK_ADD_SUB)
                opEntity.left.useBrackets = true;

            if (opEntity.right.opType & Operator::OP_MASK_ADD_SUB)
                opEntity.right.useBrackets = true;
            break;

        case Operator::Dividing:
            break;

        case Operator::Exponentiation:
            if (opEntity.left.opType != Operator::None)
                opEntity.left.useBrackets = true;
            break;

        default:
            assert(false);
    }
    return opEntity;
}

std::pair<std::string, Operator::OpType> LatexMath::generateLatexEntity(const OperatorEntity &opEntity) {
    std::string latex = "";

    switch (opEntity.opType) {
        case Operator::Adding:
        case Operator::Subtracting:
            latex += opEntity.left.useBrackets
                    ? "\\left(" + opEntity.left.latex + "\\right)"
                    : opEntity.left.latex;

            latex += " " + Operator::opTypeToString(opEntity.opType) + " ";

            latex += opEntity.right.useBrackets
                    ? "\\left(" + opEntity.right.latex + "\\right)"
                    : opEntity.right.latex;
            break;

        case Operator::Multiplying:
            latex += opEntity.left.useBrackets
                    ? "\\left(" + opEntity.left.latex + "\\right)"
                    : opEntity.left.latex;

            latex += " \\times ";

            latex += opEntity.right.useBrackets
                    ? "\\left(" + opEntity.right.latex + "\\right)"
                    : opEntity.right.latex;
            break;

        case Operator::Dividing:
            latex = "\\dfrac{" + opEntity.left.latex + "}{" + opEntity.right.latex + "}";
            break;

        case Operator::Exponentiation:
            latex += opEntity.left.useBrackets
                    ? "\\left(" + opEntity.left.latex + "\\right)"
                    : opEntity.left.latex;

            latex += "^{" + getNormalFractionsForExponent(opEntity.right.latex) + "}";
            break;

        default:
            assert(false);
    }

    return {latex, opEntity.opType};
}

std::string LatexMath::fromNumber(const RationalNumber &number, bool slanted) {
    int num = number.getNumerator();
    int den = number.getDenominator();

    if (number.isInteger())
        return std::to_string(num);

    std::string num_s = std::to_string(abs(num));
    std::string den_s = std::to_string(abs(den));

    if (!slanted) {
        return num >= 0
            ? "\\dfrac{" + num_s + "}{" + den_s + "}"
            : "-\\dfrac{" + num_s + "}{" + den_s + "}";
    } else {
        return num >= 0
            ? num_s + "/" + den_s
            : "-" + num_s + "/" + den_s;
    }
}

std::string LatexMath::getNormalFractionsForExponent(std::string exponentLatex) {
    replace_all(exponentLatex, "\\dfrac", "\\frac");
    replace_all(exponentLatex, "\\left(", "(");
    replace_all(exponentLatex, "\\right)", ")");
    return exponentLatex;
}


void LatexMath::replace_all(std::string &subject, const std::string &search, const std::string &replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}
