#ifndef TEMPLATEMATRIX_TPP
#define TEMPLATEMATRIX_TPP

#include <algorithm>
#include <cassert>
#include <limits>

template <typename T>
TemplateMatrix<T>::TemplateMatrix(const size_t rows, const size_t cols)
    : rows_(rows), cols_(cols), matrix(nullptr) {
  if (rows != 0 && cols != 0) {
    if (rows > std::numeric_limits<size_t>::max() / cols) {
      throw std::bad_alloc();
    }
    matrix = std::make_unique<T[]>(rows * cols);
  }
}

template <typename T>
void TemplateMatrix<T>::initStartValue(T value) {
  if (matrix && rows_ > 0 && cols_ > 0) {
    if (rows_ > std::numeric_limits<size_t>::max() / cols_) {
      throw std::bad_alloc();
    }
    size_t total_size = rows_ * cols_;
    if (total_size > static_cast<size_t>(std::numeric_limits<std::ptrdiff_t>::max())) {
      throw std::length_error("Matrix size exceeds maximum allowed size");
    }

    if (total_size < std::numeric_limits<size_t>::max() / sizeof(T)) {
      std::fill(matrix.get(), matrix.get() + total_size, value);
    } else {
      throw std::bad_alloc();
    }
  }
}

template <typename T>
size_t TemplateMatrix<T>::getRows() const {
  return rows_;
}

template <typename T>
size_t TemplateMatrix<T>::getCols() const {
  return cols_;
}

template <typename T>
T& TemplateMatrix<T>::operator()(const size_t row, const size_t col) {
  assert(row < rows_ && col < cols_ && "Matrix index out of bounds");
  return matrix[row * cols_ + col];
}

template <typename T>
const T& TemplateMatrix<T>::operator()(const size_t row,
                                       const size_t col) const {
  assert(row < rows_ && col < cols_ && "Matrix index out of bounds");
  return matrix[row * cols_ + col];
}

template <typename T>
void TemplateMatrix<T>::copyLineMatrix(const size_t srcLine,
                                       const size_t destLine) {
  assert(srcLine < rows_ && destLine < rows_ && "Matrix index out of bounds");
  for (size_t i = 0; i < cols_; i++) {
    matrix[destLine * cols_ + i] = matrix[srcLine * cols_ + i];
  }
}
template <typename T>
std::vector<std::vector<T>> TemplateMatrix<T>::toVector() const {
  std::vector<std::vector<T>> result;
  result.reserve(rows_);

  for (size_t i = 0; i < rows_; ++i) {
    std::vector<T> row_vector;
    row_vector.reserve(cols_);
    for (size_t j = 0; j < cols_; ++j) {
      row_vector.push_back((*this)(i, j));
    }
    result.push_back(std::move(row_vector));
  }

  return result;
}

#endif  // TEMPLATEMATRIX_TPP
