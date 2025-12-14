#ifndef POINT_H
#define POINT_H
#include <utility>
struct Point {
  int y;
  int x;

  Point() : y(0), x(0) {}

  Point(int y_val, int x_val) : y(y_val), x(x_val) {}

  bool operator<(const Point& other) const {
    return (y != other.y) ? y < other.y : x < other.x;
  }
  bool operator==(const Point& other) const {
    return y == other.y && x == other.x;
  }
  std::pair<int, int> toPair() { return std::pair<int, int>(y, x); }
};

#endif
