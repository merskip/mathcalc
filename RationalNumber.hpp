#pragma once

class RationalNumber {
private:

    int numerator;
    int denominator;

public:

    RationalNumber();
    RationalNumber(int number);
    RationalNumber(int numerator, int denominator);

    RationalNumber operator+(const int &component);
    RationalNumber operator+(const RationalNumber &component);
    RationalNumber &operator+=(const int &component);
    RationalNumber &operator+=(const RationalNumber &component);

    RationalNumber operator-(const int &subtrahend);
    RationalNumber operator-(const RationalNumber &subtrahend);
    RationalNumber &operator-=(const int &subtrahend);
    RationalNumber &operator-=(const RationalNumber &subtrahend);

    RationalNumber operator/(const int &divisor);
    RationalNumber operator/(const RationalNumber &divisor);
    RationalNumber &operator/=(const int &divisor);
    RationalNumber &operator/=(const RationalNumber &divisor);

    RationalNumber operator*(const int &factor);
    RationalNumber operator*(const RationalNumber &factor);
    RationalNumber &operator*=(const int &factor);
    RationalNumber &operator*=(const RationalNumber &factor);

    static RationalNumber pow(const RationalNumber &base, const RationalNumber &exponent);

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