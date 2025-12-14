#ifndef TEMPLATEMATRIX_H
#define TEMPLATEMATRIX_H

#include <memory>
#include <stdexcept>
#include <vector>

#include "../dungeon/core/Dungeon.h"

template <typename T>
class TemplateMatrix {
 private:
  size_t rows_{};
  size_t cols_{};
  std::unique_ptr<T[]> matrix;

 public:
  TemplateMatrix(size_t rows, size_t cols);
  TemplateMatrix() = delete;
  TemplateMatrix(const TemplateMatrix&) = delete;
  void initStartValue(T value);
  TemplateMatrix& operator=(const TemplateMatrix&) = delete;
  [[nodiscard]] size_t getRows() const;
  [[nodiscard]] size_t getCols() const;
  T& operator()(size_t row, size_t col);
  const T& operator()(size_t row, size_t col) const;
  void copyLineMatrix(size_t srcLine, size_t destLine);
  std::vector<std::vector<T>> toVector() const;
};

#include "../../src/common/TemplateMatrix.tpp"

#endif  // TEMPLATEMATRIX_H
