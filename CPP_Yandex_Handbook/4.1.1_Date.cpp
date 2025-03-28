#include <chrono>
#include <iostream>

class Date {
 public:
  Date(int day, int month, int year) {
    if (isValidDate(day, month, year)) {
      this->day = day;
      this->month = month;
      this->year = year;
    } else {
      this->day = 01;
      this->month = 01;
      this->year = 1970;
    }
  }
  int GetDay() const { return day; };
  int GetMonth() const { return month; };
  int GetYear() const { return year; };
  Date operator+(int days) const { return addDays(days); };
  Date operator-(int days) const { return addDays(-days); };
  int operator-(const Date& other) const { return countDaysBetween(other); };

 private:
  int day;
  int month;
  int year;
  bool isValidDate(int d, int m, int y) const {
    if (y < 1970 || y > 2099 || m < 1 || m > 12 || d < 1) return false;
    int daysInMonth[] = {
        31, (isLeapYear(y) ? 29 : 28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return d <= daysInMonth[m - 1];
  }
  bool isLeapYear(int year) const {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
  }
  Date addDays(int days) const {
    std::tm tm = {};
    tm.tm_sec = 0;
    tm.tm_min = 0;
    tm.tm_hour = 0;
    tm.tm_mday = day + days;
    tm.tm_mon = month - 1;
    tm.tm_year = year - 1900;
    tm.tm_isdst = -1;
    std::mktime(&tm);
    return Date(tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
  }
  int countDaysBetween(const Date& other) const {
    std::tm tm1 = {};
    tm1.tm_sec = 0;
    tm1.tm_min = 0;
    tm1.tm_hour = 0;
    tm1.tm_mday = day;
    tm1.tm_mon = month - 1;
    tm1.tm_year = year - 1900;
    tm1.tm_isdst = -1;

    std::tm tm2 = {};
    tm2.tm_sec = 0;
    tm2.tm_min = 0;
    tm2.tm_hour = 0;
    tm2.tm_mday = other.day;
    tm2.tm_mon = other.month - 1;
    tm2.tm_year = other.year - 1900;
    tm2.tm_isdst = -1;
    auto time1 = std::mktime(&tm1);
    auto time2 = std::mktime(&tm2);
    constexpr int secondsPerDay = 60 * 60 * 24;
    return static_cast<int>(std::difftime(time1, time2) / secondsPerDay);
  }
};