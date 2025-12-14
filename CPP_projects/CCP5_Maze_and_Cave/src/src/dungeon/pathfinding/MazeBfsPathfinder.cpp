#include "../../../include/dungeon/pathfinding/MazeBfsPathfinder.h"

#include <algorithm>
#include <queue>
#include <stdexcept>

#include "../../../include/common/TemplateMatrix.h"

using BoolMatrix = TemplateMatrix<bool>;
using PointMatrix = TemplateMatrix<Point>;

MazeBfsPathfinder::MazeBfsPathfinder(const Dungeon& dungeon)
    : maze(dynamic_cast<const Maze*>(&dungeon)) {}

std::optional<std::vector<std::pair<int, int>>> MazeBfsPathfinder::findPath(
    std::pair<int, int> start_, std::pair<int, int> end_) {
  if (!maze) {
    throw std::invalid_argument("Dungeon is not a Maze");
  }
  Point start(start_.first, start_.second);
  Point end(end_.first, end_.second);
  const size_t rows = maze->getRows();
  const size_t cols = maze->getCols();

  if (start.y < 0 || start.y >= static_cast<int>(rows) || end.x < 0 ||
      end.x >= static_cast<int>(cols)) {
    return std::nullopt;
  }

  std::queue<Point> q;
  BoolMatrix visited(rows, cols);
  visited.initStartValue(false);
  PointMatrix parent(rows, cols);
  parent.initStartValue(Point(-1, -1));

  q.push(start);
  visited(start.y, start.x) = true;

  int dy[] = {-1, 1, 0, 0};
  int dx[] = {0, 0, -1, 1};

  bool pathFound = false;
  while (!q.empty()) {
    Point current = q.front();
    q.pop();

    if (current == end) {
      pathFound = true;
      break;
    }

    for (int i = 0; i < 4; ++i) {
      Point next = {current.y + dy[i], current.x + dx[i]};
      if (next.y >= 0 && next.y < static_cast<int>(rows) && next.x >= 0 &&
          next.x < static_cast<int>(cols) && !visited(next.y, next.x)) {
        if (!maze->hasWallBetween(current, next)) {
          visited(next.y, next.x) = true;
          parent(next.y, next.x) = current;
          q.push(next);
        }
      }
    }
  }

  if (!pathFound) {
    return std::nullopt;
  }

  std::vector<std::pair<int, int>> path;
  Point current = end;
  while (!(current.y == -1 && current.x == -1)) {
    path.push_back(current.toPair());
    if (current == start) break;
    current = parent(current.y, current.x);
  }
  std::reverse(path.begin(), path.end());

  return path;
}