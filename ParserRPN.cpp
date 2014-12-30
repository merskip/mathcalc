#include <iostream>
#include <map>
#include <list>
#include <stack>
#include "ParserRPN.hpp"
#include "Exception.hpp"

ParserRPN::OperationTypeMap ParserRPN::operationTypeMap = {
        {"+", {2, OpDirection::Left}},
        {"-", {2, OpDirection::Left}},
        {"*", {3, OpDirection::Left}},
        {"/", {3, OpDirection::Left}},
        {"^", {4, OpDirection::Right}}
};

ParserRPN::ParserRPN(std::vector<std::string> const &tokens)
        : tokens(tokens) {
}

std::vector<std::string> ParserRPN::parseRPN(const std::vector<std::string> &tokens) {
    ParserRPN rpn(tokens);
    return rpn.parseRPN();
}

std::vector<std::string> ParserRPN::parseRPN() {
    std::vector<std::string> outStack;
    std::stack<std::string> opStack;

    for (const std::string token : tokens) {
        if (isOperator(token)) {
            const std::string o1 = token;

            if (!opStack.empty()) {
                std::string o2 = opStack.top();

                while (isOperator(o2) && ((isAssociative(o1, OpDirection::Left)
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
        else if (token == "(") {
            opStack.push(token);
        }
        else if (token == ")") {
            std::string opTop = opStack.top();

            while (opTop != "(") {
                outStack.push_back(opTop);
                opStack.pop();

                if (opStack.empty())
                    break;
                opTop = opStack.top();
            }

            if (!opStack.empty())
                opStack.pop();

            if (opTop != "(")
                throw Exception("Not found opening parenthesis", 0x1);
        }
        else {
            outStack.push_back(token);
        }
    }

    while (!opStack.empty()) {
        const std::string opToken = opStack.top();

        if (isParenthesis(opToken))
            throw Exception("Not expected to parenthesis", 0x1);

        outStack.push_back(opToken);
        opStack.pop();
    }

    return outStack;
}

std::vector<std::string> ParserRPN::getExpressionTokens(const std::string &expression) {
    std::list<std::string> tokens;
    std::string str = "";

    for (int i = 0; i < (int) expression.length(); ++i) {
        const std::string token(1, expression[i]);

        if (isOperator(token) || isParenthesis(token)) {
            if (!str.empty()) {
                tokens.push_back(str);
            }
            str = "";
            tokens.push_back(token);
        }  else {
            if (!token.empty() && token != " " && token != "\n") {
                str.append(token);
            }  else {
                if (str != "") {
                    tokens.push_back(str);
                    str = "";
                }
            }
        }
    }

    if (!str.empty())
        tokens.push_back(str);

    std::list<std::string>::iterator iter = tokens.begin();
    while (iter != tokens.end()) {
        std::string token = *iter;

        if (token == "/") {
            std::list<std::string>::iterator left = std::prev(iter, 1);
            std::list<std::string>::iterator right = std::next(iter, 1);

            std::string leftToken = *left;
            std::string rightToken = *right;

            if (isNumber(leftToken) && isNumber(rightToken)) {
                iter = tokens.erase(left, ++right);
                iter = tokens.insert(iter, leftToken + "/" + rightToken);
            }
        }

        iter++;
    }

    return std::vector<std::string>(tokens.begin(), tokens.end());
}

bool ParserRPN::isParenthesis(const std::string &token) {
    return token == "(" || token == ")";
}

bool ParserRPN::isOperator(const std::string &token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "^";
}

bool ParserRPN::isAssociative(const std::string &token, OpDirection director) {
    const std::pair<int, OpDirection> op = operationTypeMap.find(token)->second;
    return op.second == director;
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

bool ParserRPN::isNumber(const std::string &token) {
    char* p;
    strtol(token.c_str(), &p, 10);
    return *p == 0;
}

int ParserRPN::comparePrecedence(const std::string &token1, const std::string &token2) {
    const std::pair<int, int> op1 = operationTypeMap.find(token1)->second;
    const std::pair<int, int> op2 = operationTypeMap.find(token2)->second;

    return op1.first - op2.first;
}