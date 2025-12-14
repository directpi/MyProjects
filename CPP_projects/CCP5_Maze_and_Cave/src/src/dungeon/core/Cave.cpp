#include "../../../include/dungeon/core/Cave.h"

#include <stdexcept>

#include "../../../include/api/API.h"
#include "../../../include/common/Utils.h"

Cave::Cave(const int rows, const int cols)
    : limitBirth(),
      limitDeath(),
      rows(rows),
      cols(cols),
      chanceBirth(),
      delay() {}

void Cave::setMatrix(const std::vector<std::vector<int>>& matrix_) {
  if (matrix_.empty() || matrix_[0].empty()) {
    throw std::invalid_argument("Matrix is empty");
  }
  // Проверка, что все строки имеют одинаковый размер
  const size_t expected_cols = matrix_[0].size();
  for (const auto& row : matrix_) {
    if (row.size() != expected_cols) {
      throw std::invalid_argument("All rows must have the same size");
    }
  }

  this->matrix = matrix_;

  // Обновляем размеры
  this->rows = matrix_.size();
  this->cols = matrix_.empty() ? 0 : matrix_[0].size();
}

void Cave::setDelay(const int value) { delay = value; }

int Cave::getDelay() const { return delay; }

DungeonInfo Cave::getInfo() {
  CaveInfo info;
  info.matrix = matrix;

  return info;
}

std::vector<std::vector<int>> Cave::generateCave() {
  matrix.resize(rows, std::vector<int>(cols, 0));

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      matrix[i][j] = (generateRandomInt(1, 100) <= chanceBirth) ? 1 : 0;
    }
  }

  return matrix;
}

void Cave::setChanceBirth(int const value) { chanceBirth = value; }

bool Cave::generateCell() {
  bool isGenerate = false;
  std::vector<std::vector<int>> newMatrix =
      matrix;  // копируем текущее состояние

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      int liveNeighbors = 0;

      // Проходим по всем 8 соседям
      for (int di = -1; di <= 1; ++di) {
        for (int dj = -1; dj <= 1; ++dj) {
          if (di == 0 && dj == 0) continue;  // пропускаем саму клетку

          const int ni = i + di;
          const int nj = j + dj;

          // Если сосед вне границ — считаем его живым
          if (ni < 0 || ni >= rows || nj < 0 || nj >= cols) {
            liveNeighbors++;
          } else {
            liveNeighbors += matrix[ni][nj];
          }
        }
      }

      if (matrix[i][j]) {
        if (liveNeighbors < limitDeath) {
          newMatrix[i][j] = 0;
          isGenerate = true;
        }
      } else {
        if (liveNeighbors > limitBirth) {
          newMatrix[i][j] = 1;
          isGenerate = true;
        }
      }
    }
  }

  matrix = std::move(newMatrix);

  return isGenerate;
}

void Cave::setLimitBirth(const int limit) { limitBirth = limit; }

void Cave::setLimitDeath(const int limit) { limitDeath = limit; }
