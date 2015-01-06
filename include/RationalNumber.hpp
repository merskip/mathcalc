#pragma once

class RationalNumber {
private:

    int numerator;
    int denominator;

public:

    RationalNumber();
    RationalNumber(int number);
    RationalNumber(int numerator, int denominator);

    RationalNumber add(const RationalNumber &component) const;
    RationalNumber subtract(const RationalNumber &subtrahend) const;
    RationalNumber multiply(const RationalNumber &factor) const;
    RationalNumber divide(const RationalNumber &divisor) const;
    RationalNumber pow(const RationalNumber &exponent) const;

    int getNumerator() const;
    int getDenominator() const;

    bool isIndeterminate() const;

    std::string toPlainText() const;
    std::string toDecimal() const;
    std::string toLatexMath() const;
    double toDouble() const;

    RationalNumber getInverseNumber() const;

    void simplify();
    static RationalNumber simplify(RationalNumber number);
    static RationalNumber simplify(int numerator, int denominator);

    bool isInteger() const;

private:
    static int getGCD(int a, int b);
};