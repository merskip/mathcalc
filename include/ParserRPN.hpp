#pragma once

#include <string>
#include <vector>
#include <list>
#include <map>
#include "RationalNumber.hpp"
#include "Token.hpp"
#include "Operator.hpp"

class ParserRPN {
private:
    const std::vector<Token> tokens;

public:
    ParserRPN(const std::vector<Token> &tokens);

    static std::vector<Token> parseRPN(const std::vector<Token> &tokens);
    std::vector<Token> parseRPN();

    static std::vector<Token> getExpressionTokens(const std::string &expression);

    static Token getToken(const std::string &token);
    static TokenType getTokenType(const std::string &token);

    static bool isParenthesis(const Token &token);
    static bool isParenthesis(const std::string &token);

    static bool isNegative(const Token &token);
    static bool isNegative(std::list<Token> &tokens, std::list<Token>::iterator &iter);

    static bool isOperator(const Token &token);
    static bool isOperator(const std::string &token);

    static bool isAssociative(const Token &token, TokenBiding biding);

    static bool isRationalNumber(const std::string &token);
    static RationalNumber toRationalNumber(const std::string &token);

    static bool isNumber(const Token &token);
    static bool isNumber(const std::string &token);

    static int comparePrecedence(const Token &token1, const Token &token2);
};