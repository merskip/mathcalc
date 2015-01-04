#pragma once

#include <exception>
#include <map>
#include <string>

class Exception : private std::exception {
public:
    static const std::map<int, std::string> errorMessages;

private:
    const std::string message;
    const int errorCode = 0;

public:
    Exception(std::string message);
    Exception(std::string message, const int errorCode);
    Exception(const int errorCode);

    std::string getMessage() const;
    int getErrorCode() const;

private:
    virtual const char *what() const noexcept;
};