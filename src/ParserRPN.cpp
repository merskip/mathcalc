#include <iostream>
#include <map>
#include <list>
#include <stack>
#include "ParserRPN.hpp"
#include "Exception.hpp"

ParserRPN::ParserRPN(std::vector<Token> const &tokens)
        : tokens(tokens) {
}

std::vector<Token> ParserRPN::parseRPN(const std::vector<Token> &tokens) {
    ParserRPN rpn(tokens);
    return rpn.parseRPN();
}

std::vector<Token> ParserRPN::parseRPN() {
    std::vector<Token> outStack;
    std::stack<Token> opStack;

    for (Token token : tokens) {
        if (isOperator(token)) {
            Token o1 = token;

            if (!opStack.empty()) {
                Token o2 = opStack.top();

                while (isOperator(o2) && ((isAssociative(o1, TokenBiding::Left)
                        && comparePrecedence(o1, o2) == 0) || (comparePrecedence(o1, o2) < 0))) {

                    opStack.pop();
                    outStack.push_back(o2);

                    if (opStack.empty())
                        break;

                    o2 = opStack.top();
                }
            }

            opStack.push(o1);
        }
        else if (token.text == "(") {
            opStack.push(token);
        }
        else if (token.text == ")") {
            Token opTop = opStack.top();

            while (opTop.text != "(") {
                outStack.push_back(opTop);
                opStack.pop();

                if (opStack.empty())
                    break;
                opTop = opStack.top();
            }

            if (!opStack.empty())
                opStack.pop();

            if (opTop.text != "(")
                throw Exception("Not found opening parenthesis", 0x1);
        }
        else {
            outStack.push_back(token);
        }
    }

    while (!opStack.empty()) {
        const Token opToken = opStack.top();

        if (isParenthesis(opToken.text))
            throw Exception("Not expected to parenthesis", 0x1);

        outStack.push_back(opToken);
        opStack.pop();
    }

    return outStack;
}

std::vector<Token> ParserRPN::getExpressionTokens(const std::string &expression) {
    std::list<Token> tokens;
    std::string str = "";

    for (int i = 0; i < (int) expression.length(); ++i) {
        const std::string token(1, expression[i]);

        if (isOperator(token) || isParenthesis(token)) {
            if (!str.empty()) {
                tokens.push_back(getToken(str));
            }
            str = "";
            tokens.push_back(getToken(token));
        }  else {
            if (!token.empty() && token != " " && token != "\n") {
                str.append(token);
            }  else {
                if (str != "") {
                    tokens.push_back(getToken(str));
                    str = "";
                }
            }
        }
    }

    if (!str.empty())
        tokens.push_back(getToken(str));

    std::list<Token>::iterator iter = tokens.begin();
    std::stack<std::string> opStack;
    std::string lastOperator;
    while (iter != tokens.end()) {
        Token &token = *iter;

        if (token.text == "(") {
            opStack.push(lastOperator);
            lastOperator = "";
        }
        else if (token.text == ")") {
            lastOperator = opStack.top();
            opStack.pop();
        }

        if (token.text == "/" && lastOperator != "/" && lastOperator != "^") {
            auto left = std::prev(iter, 1);
            auto right = std::next(iter, 1);

            Token leftToken = *left;
            Token rightToken = *right;

            if (leftToken.type == Number && rightToken.type == Number) {
                iter = tokens.erase(left, ++right);
                std::string newTokenText = leftToken.text + "/" + rightToken.text;
                iter = tokens.insert(iter, {newTokenText, Number});

                lastOperator = "/";
                iter++;
                continue;
            }
        }

        if (isNegative(tokens, iter)) {
            Token &rightToken = *std::next(iter, 1);
            auto nextOp = std::next(iter, 2);
            if (isNumber(rightToken.text) && (nextOp == tokens.end() || (*nextOp).text != "^")) {
                rightToken = {"-" + rightToken.text, Number};
                iter = tokens.erase(iter);
                continue;
            }
            else {
                token = {"-", Negative};
            };
        }

        if (isOperator(token.text))
            lastOperator = token.text;

        iter++;
    }

//    for (auto t : tokens)
//        std::cout << ""

    return std::vector<Token>(tokens.begin(), tokens.end());
}


Token ParserRPN::getToken(const std::string &token) {
    return {token, getTokenType(token)};
}

TokenType ParserRPN::getTokenType(const std::string &token) {
    if (token == "(")       return LeftBracket;
    else if (token == ")")  return RightBracket;
    else if (token == "+")  return Adding;
    else if (token == "-")  return Subtracting;
    else if (token == "*")  return Multiplying;
    else if (token == "/")  return Dividing;
    else if (token == "^")  return Exponentiation;
    else if (isNumber(token)) return Number;
    else                    return Unknown;
}


bool ParserRPN::isParenthesis(const Token &token) {
    return token.type & TOKEN_MASK_BRACKET;
}

bool ParserRPN::isParenthesis(const std::string &token) {
    return token == "(" || token == ")";
}

bool ParserRPN::isNegative(const Token &token) {
    return token.type == TokenType::Negative;
}

bool ParserRPN::isNegative(std::list<Token> &tokens, std::list<Token>::iterator &iter) {
    if ((*iter).text != "-")
        return false;

    auto tokenLeft = std::prev(iter, 1);
    std::string left = (*tokenLeft).text;

    if (iter == tokens.begin())
        return true;

    return left == "(" || isOperator(left);
}

bool ParserRPN::isOperator(const Token &token) {
    return token.type & TOKEN_MASK_IS_OPERATOR;
}

bool ParserRPN::isOperator(const std::string &token) {
    return token == "+"
            || token == "-"
            || token == "*"
            || token == "/"
            || token == "~"
            || token == "^";
}

bool ParserRPN::isAssociative(const Token &token, TokenBiding biding) {
    return (token.type & TOKEN_MASK_BINDING) == biding;
}

bool ParserRPN::isRationalNumber(const std::string &token) {
    if (token == "/")
        return false;
    if (isNumber(token))
        return true;

    size_t symbolIndex = token.find('/');
    if (symbolIndex == std::string::npos)
        return false;

    std::string numerator = token.substr(0, symbolIndex);
    std::string denominator = token.substr(symbolIndex + 1);
    return isNumber(numerator) && isNumber(denominator);
}

RationalNumber ParserRPN::toRationalNumber(const std::string &token) {
    if (isNumber(token))
        return RationalNumber(std::stoi(token));

    size_t symbolIndex = token.find('/');
    int numerator = std::stoi(token.substr(0, symbolIndex));
    int denominator = std::stoi(token.substr(symbolIndex + 1));
    return RationalNumber(numerator, denominator);
}

bool ParserRPN::isNumber(const Token &token) {
    return (token.type & TOKEN_MASK_CLASS) == 0x0000;
}

bool ParserRPN::isNumber(const std::string &token) {
    char* p;
    strtol(token.c_str(), &p, 10);
    return *p == 0;
}

int ParserRPN::comparePrecedence(const Token &token1, const Token &token2) {
    const int token1priority = (token1.type & TOKEN_MASK_PRIORITY) >> 4;
    const int token2priority = (token2.type & TOKEN_MASK_PRIORITY) >> 4;
    return token1priority - token2priority;
}