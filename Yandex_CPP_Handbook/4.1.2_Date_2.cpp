#include <iostream>

class Date {
 public:
  Date(int day, int month, int year) {
    if (!isValidDate(day, month, year)) {
      daysSinceEpoch = 0;  // 1 января 1970 года
    } else {
      daysSinceEpoch = dateToDays(day, month, year);
    }
  }
  int GetDay() const {
    int day, month, year;
    daysToDate(daysSinceEpoch, day, month, year);
    return day;
  };
  int GetMonth() const {
    int day, month, year;
    daysToDate(daysSinceEpoch, day, month, year);
    return month;
  };
  int GetYear() const {
    int day, month, year;
    daysToDate(daysSinceEpoch, day, month, year);
    return year;
  };
  Date operator+(int days) const {
    return Date::fromDaysSinceEpoch(daysSinceEpoch + days);
  };
  Date operator-(int days) const {
    return Date::fromDaysSinceEpoch(daysSinceEpoch - days);
  };
  int operator-(const Date& other) const {
    return daysSinceEpoch - other.daysSinceEpoch;
  }

 private:
  int daysSinceEpoch;  // количество дней с 1 января 1970 года
  static bool isValidDate(int d, int m, int y) {
    if (y < 1970 || y > 2099 || m < 1 || m > 12 || d < 1) return false;
    int daysInMonth[] = {
        31, (isLeapYear(y) ? 29 : 28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return d <= daysInMonth[m - 1];
  }
  static bool isLeapYear(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
  }
  static int dateToDays(int day, int month, int year) {
    int days = 0;
    for (int y = 1970; y < year; y++) {
      days += isLeapYear(y) ? 366 : 365;
    }
    int daysInMonth[] = {
        31, (isLeapYear(year) ? 29 : 28), 31, 30, 31, 30, 31, 31, 30, 31, 30,
        31};
    for (int m = 1; m < month; ++m) {
      days += daysInMonth[m - 1];
    }
    days += day - 1;
    return days;
  }

  static void daysToDate(int days, int& day, int& month, int& year) {
    year = 1970;
    while (true) {
      int daysInYear = isLeapYear(year) ? 366 : 365;
      if (days < daysInYear) break;
      days -= daysInYear;
      ++year;
    }
    int daysInMonth[] = {
        31, (isLeapYear(year) ? 29 : 28), 31, 30, 31, 30, 31, 31, 30, 31, 30,
        31};
    month = 1;
    while (days >= daysInMonth[month - 1]) {
      days -= daysInMonth[month - 1];
      ++month;
    }
    day = days + 1;
  }
  static Date fromDaysSinceEpoch(int days) {
    int day, month, year;
    daysToDate(days, day, month, year);
    return Date(day, month, year);
  }
};