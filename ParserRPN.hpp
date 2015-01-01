#pragma once

#include <string>
#include <vector>
#include <list>
#include <map>
#include "RationalNumber.hpp"

class ParserRPN {
public:
    enum OpDirection {
        Left,
        Right
    };

    typedef std::map<std::string, std::pair<int, OpDirection>> OperationTypeMap;

private:
    static OperationTypeMap operationTypeMap;

    const std::vector<std::string> tokens;

public:
    ParserRPN(const std::vector<std::string> &tokens);

    static std::vector<std::string> parseRPN(const std::vector<std::string> &tokens);
    std::vector<std::string> parseRPN();

    static std::vector<std::string> getExpressionTokens(const std::string &expression);

    static bool isParenthesis(const std::string &token);
    static bool isOperator(const std::string &token);
    static bool isNegative(std::list<std::string> &tokens, std::list<std::string>::iterator &iter);
    static bool isAssociative(const std::string &token, OpDirection direction);
    static bool isRationalNumber(const std::string &token);
    static RationalNumber toRationalNumber(const std::string &token);
    static bool isNumber(const std::string &token);

    static int comparePrecedence(const std::string &token1, const std::string &token2);
};