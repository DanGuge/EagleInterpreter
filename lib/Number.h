/*
 * Number - Arbitrary-Precision Float Arithmetic Library
 * Copyright (C) 2015 Gianfranco Mariotti
 */

#pragma once

#include <cmath>
#include <deque>
#include <iostream>
#include <sstream>

#include "object.h"

namespace eagle {

class Number;
using BigFloatPtr = std::shared_ptr<Number>;

class Number : public Object {
private:
    char sign;
    std::deque<char> number;
    int decimals;

    // Error Checking
    bool error;
    std::string error_code;

    // Transformations int<-->char single digit
    inline static int CharToInt(const char& val) {
        return (val - '0');
    };
    inline static char IntToChar(const int& val) {
        return (val + '0');
    };

    // Comparator without sign, utilized by Comparators and Operations
    static int CompareNum(const Number& left, const Number& right);

    // Operations without sign and decimals, utilized by Operations
    static Number Sum(const Number& left, const Number& right);
    static Number Subtract(const Number& left, const Number& right);
    static Number Multiply(const Number& left, const Number& right);
    static Number Pow(const Number& left, const Number& right);

    // Default decimal
    static const int DEFAULT_DECIMAL = 20;

public:
    // Constructors
    Number() {
        sign = '\0';
        decimals = 0;
        error = 1;
        error_code = "Unset";
    };
    Number(const char* strNum) {
        *this = strNum;
    };
    Number(std::string strNum) {
        *this = strNum;
    };
    Number(int Num) {
        *this = Num;
    };
    Number(double Num) {
        *this = Num;
    };

    // Assignment operators
    Number& operator=(const char* strNum);
    Number& operator=(std::string strNum);
    Number& operator=(int Num);
    Number& operator=(double Num);

    // Operations
    friend Number operator+(const Number& left_, const Number& right_);
    friend Number operator+(const Number& left, const int& int_right);
    friend Number operator+(const Number& left, const double& double_right);

    friend Number operator-(const Number& left_, const Number& right_);
    friend Number operator-(const Number& left, const int& int_right);
    friend Number operator-(const Number& left, const double& double_right);

    friend Number operator*(const Number& left, const Number& right);
    friend Number operator*(const Number& left, const int& int_right);
    friend Number operator*(const Number& left, const double& double_right);

    friend Number operator/(const Number& left, const Number& right);
    friend Number operator/(const Number& left, const int& int_right);
    friend Number operator/(const Number& left, const double& double_right);
    static Number PrecDiv(const Number& left, const Number& right, int div_precision);
    static Number PrecDiv(const Number& left, const int& int_right, int div_precision);
    static Number PrecDiv(const Number& left, const double& double_right, int div_precision);

    friend Number operator%(const Number& left, const Number& right);
    friend Number operator%(const Number& left, const int& int_right);

    static Number Power(const Number& left, const Number& right, int div_precision = 0);
    static Number Power(const Number& left, const int& int_right, int div_precision = 0);
    static Number Power(const Number& left, const double& double_right, int div_precision = 0);

    friend std::ostream& operator<<(std::ostream& out, Number& b);

    Number& operator++(int i) {
        *this = *this + 1;
        return *this;
    };
    Number& operator++() {
        *this = *this + 1;
        return *this;
    };
    Number& operator--(int i) {
        *this = *this - 1;
        return *this;
    };
    Number& operator--() {
        *this = *this - 1;
        return *this;
    };

    // Comparators
    bool operator==(const Number& right) const;
    bool operator==(const int& int_right) const;
    bool operator==(const double& double_right) const;

    bool operator!=(const Number& right) const;
    bool operator!=(const int& int_right) const;
    bool operator!=(const double& double_right) const;

    bool operator>(const Number& right) const;
    bool operator>(const int& int_right) const;
    bool operator>(const double& double_right) const;

    bool operator>=(const Number& right) const;
    bool operator>=(const int& int_right) const;
    bool operator>=(const double& double_right) const;

    bool operator<(const Number& right) const;
    bool operator<(const int& int_right) const;
    bool operator<(const double& double_right) const;

    bool operator<=(const Number& right) const;
    bool operator<=(const int& int_right) const;
    bool operator<=(const double& double_right) const;

    // Stream Operators
    friend std::ostream& operator<<(std::ostream& out, const Number& right);
    friend std::istream& operator>>(std::istream& in, Number& right);

    // Check Methods
    bool isInteger() const {
        return Decimals() == 0;
    }

    // Transformation Methods
    double ToDouble() const;
    float ToFloat() const;
    int ToInt() const;
    std::string ToString() const;
    void SetPrecision(int prec);  // Approximate number or Increase number decimals

    void LeadTrim();   // Remove number leading zeros, utilized by Operations without sign
    void TrailTrim();  // Remove number non significant trailing zeros

    // Error Checking
    inline bool HasError() const {
        return error;
    };
    inline std::string GetError() const {
        if (error)
            return error_code;
        else
            return "No Error";
    };

    // Miscellaneous Methods
    inline int Decimals() const {
        return decimals;
    };
    inline int Ints() const {
        return number.size() - decimals;
    };
    inline int MemorySize() const {
        return sizeof(*this) + number.size() * sizeof(char);
    };
    std::string Exp() const;

    // object methods
    std::string toString() override;

    bool equals(ObjectPtr other) override;

    size_t hashcode() override;

    bool isTruthy() override;
};
}  // namespace eagle