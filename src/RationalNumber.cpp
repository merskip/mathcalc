#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <math.h>
#include "RationalNumber.hpp"
#include "Exception.hpp"
#include "LatexMath.hpp"

RationalNumber::RationalNumber()
        : numerator(0),
          denominator(0) {
}

RationalNumber::RationalNumber(int number)
        : numerator(number),
          denominator(1) {
}

RationalNumber::RationalNumber(int numerator, int denominator)
        : numerator(numerator),
          denominator(denominator) {

    if (denominator == 0)
        throw std::invalid_argument("Denominator mustn't be zero");
}


RationalNumber RationalNumber::add(const RationalNumber &component) const {
    RationalNumber result;
    result.numerator = (numerator * component.denominator) + (denominator * component.numerator);
    result.denominator = denominator * component.denominator;
    return result;
}

RationalNumber RationalNumber::subtract(const RationalNumber &subtrahend) const {
    RationalNumber result;
    result.numerator = (numerator * subtrahend.denominator) - (denominator * subtrahend.numerator);
    result.denominator = denominator * subtrahend.denominator;
    return result;
}

RationalNumber RationalNumber::multiply(const RationalNumber &factor) const {
    RationalNumber result;
    result.numerator = numerator * factor.numerator;
    result.denominator = denominator * factor.denominator;
    return result;
}

RationalNumber RationalNumber::divide(const RationalNumber &divisor) const {
    RationalNumber inverseDivisor = divisor.getInverseNumber();
    RationalNumber result = multiply(inverseDivisor);
    return result;
}

RationalNumber RationalNumber::pow(const RationalNumber &exponent) const {
    RationalNumber _base = *this;
    RationalNumber _exponent = exponent;

    _exponent.simplify();
    if (!_exponent.isInteger())
        throw Exception("Exponentiation only for integer of exponent", 0x5);

    int exponentInteger = _exponent.getNumerator();

    if (exponentInteger < 0)
        _base = _base.getInverseNumber();

    int numerator = (int) ::pow(_base.getNumerator(), ::abs(exponentInteger));
    int denominator = (int) ::pow(_base.getDenominator(), ::abs(exponentInteger));

    return RationalNumber(numerator, denominator);
}

int RationalNumber::getNumerator() const {
    return numerator;
}

int RationalNumber::getDenominator() const {
    return denominator;
}

bool RationalNumber::isIndeterminate() const {
    return numerator == 0 && denominator == 0;
}

std::string RationalNumber::toPlainText() const {
    std::string num = std::to_string(numerator);
    std::string den = std::to_string(denominator);
    return isInteger() ? num : num + "/" + den;
}

std::string RationalNumber::toDecimal() const {
    double d_num = numerator;
    double d_den = denominator;
    double result = d_num / d_den;

    std::stringstream ss;
    ss << std::setprecision(std::numeric_limits<double>::digits10 + 1);
    ss << result;
    if (result * d_den != d_num)
        ss << "...";

    return ss.str();
}

std::string RationalNumber::toLatexMath() const {
    return LatexMath::fromNumber(*this);
}


double RationalNumber::toDouble() const {
    double d_num = numerator;
    double d_den = denominator;
    return d_num / d_den;
}

RationalNumber RationalNumber::getInverseNumber() const {
    RationalNumber inverseNumber;
    inverseNumber.numerator = denominator;
    inverseNumber.denominator = numerator;
    return inverseNumber;
}

void RationalNumber::simplify() {
    RationalNumber simplified = simplify(*this);
    numerator = simplified.numerator;
    denominator = simplified.denominator;
}

RationalNumber RationalNumber::simplify(RationalNumber number) {
    return simplify(number.numerator, number.denominator);
}

RationalNumber RationalNumber::simplify(int numerator, int denominator) {
    int gcd = getGCD(abs(numerator), abs(denominator));

    RationalNumber number;
    if (gcd > 1)
        number = RationalNumber(numerator / gcd, denominator / gcd);
    else
        number = RationalNumber(numerator, denominator);

    if (number.denominator < 0) {
        number.numerator *= -1;
        number.denominator *= -1;
    }
    return number;
}

int RationalNumber::getGCD(int a, int b) {
    if (b == 0)
        return a;
    return getGCD(b, a % b);
}

bool RationalNumber::isInteger() const {
    return denominator == 1;
}
