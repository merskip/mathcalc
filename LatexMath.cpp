#include <assert.h>
#include <iostream>
#include "LatexMath.hpp"
#include "ParserRPN.hpp"

#define DEBUG

std::string LatexMath::fromRPN(const std::vector<std::string> &rpn) {
    std::vector<std::pair<std::string, Operator::OpType>> latexStack;

    for (auto iter = rpn.begin(); iter != rpn.end(); iter++) {
        std::string token = *iter;

#ifdef DEBUG
        static int i = 0;
        std::cout << std::endl << " ------------ " << std::endl;
        int j = 0;
        for (auto t : rpn) {
            if (i == j) std::cout << "_" << t << "_";
            else std::cout << " " << t << " ";
            j++;
        }
        i++;
        std::cout << std::endl;
#endif

        if (ParserRPN::isRationalNumber(token)) {
            RationalNumber number = ParserRPN::toRationalNumber(token);

            std::string number_l = fromNumber(number);

            if (!number.isInteger() && std::next(iter, 1) != rpn.end()) {
                std::string nextToken = *std::next(iter, 1);
                if (nextToken == "^")
                    number_l = fromNumber(number, true);
            }

            if (number.isInteger())
                latexStack.push_back({number_l, {Operator::None}});
            else
                latexStack.push_back({number_l, {Operator::Dividing}});
        }
        else if (token == "~") {
            auto latexRight = latexStack.back();
            latexStack.pop_back();

            OperatorEntity opEntity;
            opEntity.opType = Operator::Negative;
            opEntity.right = {latexRight.first, latexRight.second, false};
            opEntity = appendBracketsToOperatorEntity(opEntity);

            auto latexEntity = generateLatexEntity(opEntity);
            latexStack.push_back(latexEntity);
        }
        else if (ParserRPN::isOperator(token)) {
            auto latexRight = latexStack.back();
            latexStack.pop_back();
            auto latexLeft = latexStack.back();
            latexStack.pop_back();

            OperatorEntity opEntity;
            opEntity.opType = Operator::getOpType(token);
            opEntity.left = {latexLeft.first, latexLeft.second, false};
            opEntity.right = {latexRight.first, latexRight.second, false};
            opEntity = appendBracketsToOperatorEntity(opEntity);

            auto latexEntity = generateLatexEntity(opEntity);
            latexStack.push_back(latexEntity);
        }

#ifdef DEBUG
        int k = 0;
        for (auto l : latexStack) {
            std::cout << k++ << ": \"" << l.first << "\"" << " ∧ " << Operator::opTypeToString(l.second) << std::endl;
        }
#endif
    }

    return latexStack.back().first;
}

LatexMath::OperatorEntity &LatexMath::appendBracketsToOperatorEntity(OperatorEntity &opEntity) {
    switch (opEntity.opType) {
        case Operator::Negative:
            if (opEntity.right.opType & Operator::OP_MASK_ADD_SUB)
                opEntity.right.useBrackets = true;
            if (opEntity.right.opType == Operator::Multiplying)
                opEntity.right.useBrackets = true;
            break;

        case Operator::Adding:
            if (opEntity.right.opType == Operator::Subtracting)
                opEntity.right.useBrackets = true;
            break;

        case Operator::Subtracting:
            if (opEntity.right.opType & Operator::OP_MASK_ADD_SUB)
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

    if (opEntity.opType != Operator::Dividing) {
        if (opEntity.right.latex[0] == '-')
            opEntity.right.useBrackets = true;
    }
    if (opEntity.opType == Operator::Exponentiation) {
        if (opEntity.left.latex[0] == '-')
            opEntity.left.useBrackets = true;
    }

    return opEntity;
}

std::pair<std::string, Operator::OpType> LatexMath::generateLatexEntity(const OperatorEntity &opEntity) {
    std::string latex = "";

    switch (opEntity.opType) {
        case Operator::Negative:
            latex += "-";
            latex += opEntity.right.useBrackets
                    ? "\\left(" + opEntity.right.latex + "\\right)"
                    : opEntity.right.latex;
            break;

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
