#include "../../../include/dungeon/core/Maze.h"

#include "../../../include/common/Utils.h"

Maze::Maze(const size_t rows, const size_t cols)
    : rightWalls(rows, cols), downWalls(rows, cols), rows_(rows), cols_(cols) {}

void Maze::generateMaze() {
  std::vector<int> cellsNumbers(cols_, EMPTY_CELL);
  for (size_t i = 0; i < rows_ - 1; i++) {
    generateNormalRow(i, cellsNumbers);
    newCellsNumberRow(i, cellsNumbers);
  }
  generateFinalRow(rows_ - 1, cellsNumbers);
}

void Maze::generateFinalRow(const size_t line, std::vector<int> &cellsNumbers) {
  initCellToUniqValue(cellsNumbers);

  for (size_t i = 0; i < cols_ - 1; i++) {
    if (cellsNumbers[i] != cellsNumbers[i + 1]) {
      rightWalls(line, i) = EMPTY_CELL;
      mergingCells(i, cellsNumbers);
    } else {
      rightWalls(line, i) = WALL;
    }
  }
  rightWalls(line, cols_ - 1) = WALL;

  for (size_t i = 0; i < cols_; i++) {
    downWalls(line, i) = WALL;
  }
}

void Maze::generateNormalRow(const size_t line,
                             std::vector<int> &cellsNumbers) {
  initCellToUniqValue(cellsNumbers);
  generateRightWalls(line, cellsNumbers);
  generateDownWalls(line, cellsNumbers);
}

void Maze::newCellsNumberRow(const size_t line,
                             std::vector<int> &cellsNumbers) {
  for (size_t i = 0; i < cols_; i++) {
    if (downWalls(line, i) == WALL) {
      cellsNumbers[i] = getCounter();
    }
  }
}

void Maze::initCellToUniqValue(std::vector<int> &cellsNumbers) const {
  for (size_t i = 0; i < cols_; i++) {
    if (cellsNumbers[i] == EMPTY_CELL) {
      cellsNumbers[i] = getCounter();
    }
  }
}

void Maze::generateRightWalls(const size_t line,
                              std::vector<int> &cellsNumbers) {
  for (size_t i = 0; i < cols_ - 1; i++) {
    if (cellsNumbers[i] == cellsNumbers[i + 1] || randomTrueFalse()) {
      rightWalls(line, i) = WALL;
    } else {
      rightWalls(line, i) = EMPTY_CELL;
      mergingCells(i, cellsNumbers);
    }
  }
  rightWalls(line, cols_ - 1) = WALL;
}

void Maze::mergingCells(const size_t column,
                        std::vector<int> &cellsNumbers) const {
  const int oldSet = cellsNumbers[column + 1];
  const int newSet = cellsNumbers[column];
  for (size_t j = 0; j < cols_; ++j) {
    if (cellsNumbers[j] == oldSet) {
      cellsNumbers[j] = newSet;
    }
  }
}

void Maze::generateDownWalls(const size_t line,
                             const std::vector<int> &cellsNumbers) {
  for (size_t i = 0; i < cols_; i++) {
    if (randomTrueFalse() && countTypeCellsWithoutDownWalls(
                                 cellsNumbers[i], line, cellsNumbers) > 1) {
      downWalls(line, i) = WALL;
    } else {
      downWalls(line, i) = EMPTY_CELL;
    }
  }
}

int Maze::countTypeCellsWithoutDownWalls(
    const int cellType, const size_t rowIndex,
    const std::vector<int> &cellsNumbers) const {
  return countCellsByType(cellType, cellsNumbers) -
         countDownWallByType(cellType, rowIndex, cellsNumbers);
}

int Maze::countCellsByType(const int cellType,
                           const std::vector<int> &cellsNumbers) const {
  int count = 0;
  for (size_t i = 0; i < cols_; i++) {
    if (cellsNumbers[i] == cellType) {
      count++;
    }
  }
  return count;
}

int Maze::countDownWallByType(const int cellType, const size_t rowIndex,
                              const std::vector<int> &cellsNumbers) const {
  int count = 0;
  for (size_t i = 0; i < cols_; i++) {
    if (cellsNumbers[i] == cellType && downWalls(rowIndex, i) == WALL) {
      count++;
    }
  }
  return count;
}

bool Maze::hasWallBetween(Point p1, Point p2) const {
  bool result = false;
  if (std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y) == 1) {
    if (p2.x > p1.x) {
      result = rightWalls(p1.y, p1.x) == WALL;
    } else if (p2.x < p1.x) {
      result = rightWalls(p2.y, p2.x) == WALL;
    } else if (p2.y > p1.y) {
      result = downWalls(p1.y, p1.x) == WALL;
    } else if (p2.y < p1.y) {
      result = downWalls(p2.y, p2.x) == WALL;
    }
  }

  return result;
}

DungeonInfo Maze::getInfo() {
  MazeInfo info;
  info.rightWall = rightWalls.toVector();
  info.downWall = downWalls.toVector();
  return info;
}