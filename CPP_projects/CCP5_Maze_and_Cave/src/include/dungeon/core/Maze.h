#ifndef MAZE_H
#define MAZE_H

#include <vector>

#include "../../common/Point.h"
#include "../../common/TemplateMatrix.h"
#include "Dungeon.h"

#define EMPTY_CELL 0
#define WALL 1

using Matrix = TemplateMatrix<int>;

class Maze final : public Dungeon {
 private:
  Matrix rightWalls;
  Matrix downWalls;
  size_t rows_;
  size_t cols_;

  void mergingCells(size_t column, std::vector<int>& cellsNumbers) const;
  void initCellToUniqValue(std::vector<int>& cellsNumbers) const;
  void generateRightWalls(size_t line, std::vector<int>& cellsNumbers);
  void generateDownWalls(size_t line, const std::vector<int>& cellsNumbers);
  [[nodiscard]] int countTypeCellsWithoutDownWalls(
      int cellType, size_t rowIndex,
      const std::vector<int>& cellsNumbers) const;
  void generateNormalRow(size_t line, std::vector<int>& cellsNumbers);
  [[nodiscard]] int countCellsByType(
      int cellType, const std::vector<int>& cellsNumbers) const;
  [[nodiscard]] int countDownWallByType(
      int cellType, size_t rowIndex,
      const std::vector<int>& cellsNumbers) const;
  void newCellsNumberRow(size_t line, std::vector<int>& cellsNumbers);
  void generateFinalRow(size_t line, std::vector<int>& cellsNumbers);

 public:
  Maze(size_t rows, size_t cols);
  void generateMaze();
  Matrix& getRightWalls() { return rightWalls; };
  Matrix& getDownWalls() { return downWalls; };
  const Matrix& getRightWalls() const { return rightWalls; }
  const Matrix& getDownWalls() const { return downWalls; }
  bool hasWallBetween(Point p1, Point p2) const;

  size_t getRows() const { return rows_; }
  size_t getCols() const { return cols_; }

  DungeonInfo getInfo() override;
};

#endif