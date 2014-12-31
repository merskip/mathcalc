#include <iostream>
#include <vector>
#include <stack>
#include <iterator>
#include <sstream>
#include <unistd.h>

#include "Exception.hpp"
#include "Version.hpp"
#include "RationalNumber.hpp"
#include "ParserRPN.hpp"
#include "CalculatorRPN.hpp"
#include "LatexMath.hpp"

#define APP_NAME "MathCalc"
#define PROMPT " >> "

CalculatorRPN::Result computeExpression(const std::string &rawInput);
std::string encodeJson(std::vector<std::pair<std::string, std::string>> data);

std::string toString(const std::vector<std::string> &stringVector);
std::string replaceString(std::string subject, const std::string &search,  const std::string &replace);

enum RunningMode {
    LOOP_EXPRESSION,
    ONE_EXPRESSION
};

int main(int argc, char *argv[]) {
    RunningMode runningMode = argc >= 2 ? ONE_EXPRESSION : LOOP_EXPRESSION;

    if (runningMode == LOOP_EXPRESSION) {
        std::cout << APP_NAME << " " << APP_VERSION << std::endl;
        std::cout << std::endl;
        std::cout << "Wprowadź wyrażenie do obliczenia:" << std::endl;
    } else {
        std::string rawInput = argv[1];
        try {
            CalculatorRPN::Result result = computeExpression(rawInput);

            char hostname[255];
            gethostname(hostname, 255);

            std::string json = encodeJson({
                    {"system", encodeJson({
                            {"version", APP_VERSION},
                            {"build_date", APP_BUILD_DATE},
                            {"hostname", std::string(hostname)}
                    })},
                    {"input", encodeJson({
                            {"raw", result.input.rawInput},
                            {"tokens", toString(result.input.tokens)},
                            {"rpn", toString(result.input.rpn)},
                            {"as_latex", result.input_latex}
                    })},
                    {"result", encodeJson({
                            {"plain_text", result.result.toPlainText()},
                            {"decimal", result.result.toDecimal()},
                            {"as_latex", LatexMath::fromNumber(result.result)}
                    })}
            });
            std::cout << json << std::endl;
        } catch (Exception &e) {
            std::string json = encodeJson({
                    {"error", e.getMessage()},
                    {"error_code", std::to_string(e.getErrorCode())}
            });
            std::cout << json;
        }
        return 0;
    }

    while (true) {
        std::string rawInput;
        std::cout << PROMPT;
        std::getline(std::cin, rawInput);

        if (rawInput == "quit" || rawInput == "q" || std::cin.eof()) {
            if (rawInput.empty())
                std::cout << std::endl;
            std::cout << "Do zobaczenia!" << std::endl;
            return 0;
        }
        if (rawInput.empty())
            continue;

        try {
            CalculatorRPN::Result result = computeExpression(rawInput);
            std::cout << "RPN: " << toString(result.input.rpn) << std::endl;
            std::cout << "Wynik: " << result.result.toPlainText() << std::endl;
        } catch (Exception &e) {
            std::cout << "Błąd: "
                    << "(" << std::hex << e.getErrorCode() << ") "
                    << e.getMessage() << std::endl;
        } catch(...) {
            std::cout << "Error: "
                    << "(-1) Unknown exception" << std::endl;
        }
    }
};

CalculatorRPN::Result computeExpression(const std::string &rawInput) {
    CalculatorRPN::Input calcInput;
    calcInput.rawInput = rawInput;
    calcInput.tokens = ParserRPN::getExpressionTokens(rawInput);
    calcInput.rpn = ParserRPN::parseRPN(calcInput.tokens);

    CalculatorRPN::Result result = CalculatorRPN::compute(calcInput);
    return result;
}

std::string encodeJson(std::vector<std::pair<std::string, std::string>> data) {
    static std::string ind = "   ";

    std::stringstream json;
    json << "{" << std::endl;
    int i = 0;
    for (const auto &item : data) {
        json << ind << "\"" << item.first << "\": ";

        if (item.second[0] == '{') {
            json << replaceString(item.second, "\n", "\n" + ind);
        }  else {
            json << "\"" << replaceString(item.second, "\\", "\\\\") << "\"";
        }

        if (i < data.size() - 1)
            json << ", " << std::endl;
        else
            json << std::endl;
        i++;
    }

    json << "}";
    return json.str();
}

std::string toString(const std::vector<std::string> &stringVector) {
    std::stringstream str;
    for (std::string item : stringVector)
        str << item << " ";
    return str.str();
}

std::string replaceString(std::string subject, const std::string& search,  const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}