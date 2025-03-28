#include <iostream>
#include <numeric>

class Rational {
 public:
  Rational(int numerator = 0, int denominator = 1) {
    if (denominator == 0) {
      throw std::invalid_argument("Знаменатель не может быть равным нулю");
    } else {
      int gcd = std::gcd(numerator, denominator);
      num = numerator / gcd;
      den = denominator / gcd;
    }
    if (den < 0) {
      num = -num;
      den = -den;
    }
  }
  int Numerator() const { return num; }
  int Denominator() const { return den; }

  // Операторы сложения, вычитания, умножения и деления
  Rational operator+(const Rational& other) const {
    return Rational(num * other.den + other.num * den, den * other.den);
  }
  Rational operator-(const Rational& other) const {
    return Rational(num * other.den - other.num * den, den * other.den);
  }
  Rational operator*(const Rational& other) const {
    return Rational(num * other.num, den * other.den);
  }
  Rational operator/(const Rational& other) const {
    if (other.num == 0) {
      throw std::invalid_argument("На ноль делить нельзя");
    }
    return Rational(num * other.den, den * other.num);
  }

  Rational& operator+=(const Rational& other) {
    *this = *this + other;
    return *this;
  }
  Rational& operator-=(const Rational& other) {
    *this = *this - other;
    return *this;
  }
  Rational& operator*=(const Rational& other) {
    *this = *this * other;
    return *this;
  }
  Rational& operator/=(const Rational& other) {
    *this = *this / other;
    return *this;
  }

  // Операторы сравнения
  bool operator==(const Rational& other) const {
    return num == other.num && den == other.den;
  }
  bool operator!=(const Rational& other) const { return !(*this == other); }

  // Унарные операторы + и -
  Rational operator+() const { return *this; }
  Rational operator-() const { return Rational(-num, den); }

  // Перегрузка операторов для работы с int
  Rational operator+(int other) const { return *this + Rational(other); }
  Rational operator-(int other) const { return *this - Rational(other); }
  Rational operator*(int other) const { return *this * Rational(other); }
  Rational operator/(int other) const { return *this / Rational(other); }

  Rational operator+=(int other) {
    *this += Rational(other);
    return *this;
  }
  Rational operator-=(int other) {
    *this -= Rational(other);
    return *this;
  }
  Rational operator*=(int other) {
    *this *= Rational(other);
    return *this;
  }
  Rational operator/=(int other) {
    *this /= Rational(other);
    return *this;
  }

 private:
  int num;  // числитель
  int den;  // знаменатель
};
  // Перегрузка операторов для работы с int как левым операндом

  Rational operator+(int lhs, const Rational& rhs) {
    return Rational (lhs)+rhs;
  }
  Rational operator-(int lhs, const Rational& rhs) {
    return Rational (lhs)-rhs;
  }
  Rational operator*(int lhs, const Rational& rhs) {
    return Rational (lhs)*rhs;
  }
  Rational operator/(int lhs, const Rational& rhs) {
    return Rational (lhs)/rhs;
  }
