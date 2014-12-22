#include <stdexcept>
#include "Exception.hpp"

const std::map<int, std::string> Exception::errorMessages = {
        {0x0, "Unknown error"},
        {0x1, "Problem with parentheses"},
        {0x2, "Operator expected"},
        {0x3, "Expected more arguments"},
        {0x4, "Unknown token"}
};

Exception::Exception(std::string message)
        : Exception(message, 0) {
}

Exception::Exception(const int errorCode)
        : Exception(errorMessages.at(errorCode), errorCode) {
}

Exception::Exception(std::string message, const int errorCode)
        : message(message),
          errorCode(errorCode) {
}

std::string Exception::getMessage() const {
    return message;
}

int Exception::getErrorCode()const {
    return errorCode;
}

const char *Exception::what() const noexcept {
    return message.c_str();
}
