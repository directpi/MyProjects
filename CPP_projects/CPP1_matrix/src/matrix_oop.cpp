#include "matrix_oop.h"
/* : <attribute_name>(value) syntax helps to automatically fill attributes with
values */

Matrix::Matrix() : rows(0), cols(0), matrix_ptr(nullptr) {}

Matrix::Matrix(int m_rows, int n_cols) : rows(m_rows), cols(n_cols) {
  if (rows <= 0 || cols <= 0) {
    throw std::invalid_argument("Incorrect matrix size");
  }
  matrix_ptr = new double[rows * cols]{0};
  // if (!matrix_ptr) {
  //   throw std::invalid_argument("Incorrect memory allocation");
  // }
}

Matrix::Matrix(const Matrix& another)
    : rows(another.rows), cols(another.cols) {
  Copy(another);
}

Matrix::Matrix(Matrix&& another)
    : rows(another.rows), cols(another.cols), matrix_ptr(another.matrix_ptr) {
  Move(another);
}

Matrix::~Matrix() {
  if (matrix_ptr) {
    // free
    delete[] matrix_ptr;
    matrix_ptr = nullptr;
    rows = 0;
    cols = 0;
  }
}
bool Matrix::EqMatrix(const Matrix& another) {
  bool result_code = true;
  if (this->rows == another.rows && this->cols == another.cols) {
    for (int i = 0; i < this->rows * this->cols && result_code; i++) {
      if (fabs(this->matrix_ptr[i] - another.matrix_ptr[i]) > DELTA_EPS) {
        result_code = false;
      }
    }
  } else {
    throw std::invalid_argument("Different matrix sizes");
  }
  return result_code;
}
void Matrix::SumMatrix(const Matrix& another) {
  if (this->rows != another.rows || this->cols != another.cols) {
    throw std::invalid_argument("Different matrix sizes");
  }
  for (auto i = 0; i < this->rows * this->cols; i++) {
    if (this->matrix_ptr[i] + another.matrix_ptr[i] < DOUBLE_MAX &&
        this->matrix_ptr[i] + another.matrix_ptr[i] != INFINITY) {
      this->matrix_ptr[i] = this->matrix_ptr[i] + another.matrix_ptr[i];
    } else {
      throw std::out_of_range("Summ is out of range");
    }
  }
}
void Matrix::SubMatrix(const Matrix& another) {
  // exception throwing example
  if (this->rows != another.rows || this->cols != another.cols) {
    throw std::invalid_argument("Different matrix sizes");
  }
  Matrix temp_matrix(this->rows, this->cols);
  for (auto i = 0; i < this->rows; i++) {
    for (auto j = 0; j < this->cols; j++) {
      (*this)[i][j] = (*this)[i][j] - another[i][j];
    }
  }
}
void Matrix::MulNumber(const double num) {
  for (int i = 0; i < rows * cols; i++) {
    if (matrix_ptr[i] * num < DOUBLE_MAX && matrix_ptr[i] * num != INFINITY) {
      matrix_ptr[i] = matrix_ptr[i] * num;
    } else {
      throw std::out_of_range("Multiplication is out of range");
    }
  }
}
void Matrix::MulMatrix(const Matrix& another) {
  if (this->rows != another.cols || this->cols != another.rows) {
    throw std::invalid_argument("Incorrect matrix sizes for multiplication");
  } else {
    Matrix temp_matrix(this->rows, another.cols);
    for (int i = 0; i < this->rows; i++) {
      for (auto j = 0; j < another.cols; j++) {
        for (int k = 0; k < this->cols; k++) {
          if ((*this)[i][k] * another[k][j] < DOUBLE_MAX &&
              (*this)[i][k] * another[k][j] != INFINITY) {
            temp_matrix[i][j] += (*this)[i][k] * another[k][j];
          } else {
            throw std::out_of_range("Multiplication is out of range");
          }
        }
      }
    }
    this->rows = temp_matrix.get_rows();
    this->cols = temp_matrix.get_cols();
    *this = temp_matrix;
  }
}
Matrix Matrix::Transpose() {
  Matrix temp_matrix(this->cols, this->rows);
  for (auto i = 0; i < this->rows; i++) {
    for (auto j = 0; j < this->cols; j++) {
      temp_matrix[j][i] = (*this)[i][j];
    }
  }
  return temp_matrix;
}
Matrix Matrix::CalcComplements() {
  Matrix result{this->rows, this->cols};
  if (this->rows != this->cols) {
    throw std::invalid_argument("Incorrect input, matrix is not square");
  } else {
    if (this->rows > 1) {
      for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
          Matrix matr_minor = minor(i, j);
          double determ = matr_minor.Determinant();
          result[i][j] = determ * ((i + j) & 1 ? -1 : 1);
          matr_minor.~Matrix();
        }
      }
    }
  }
  return result;
}
Matrix Matrix::minor(int row, int col) {
  Matrix minor(this->rows - 1, this->cols - 1);
  for (int i = 0; i < this->rows; i++) {
    for (int j = 0; j < this->cols; j++) {
      if (i != row && j != col) {
        int k = i - (i > row);
        int l = j - (j > col);
        minor[k][l] = (*this)[i][j];
      }
    }
  }
  return minor;
}
double Matrix::Determinant() {
  double result = 0;
  if (this->rows != this->cols) {
    throw std::invalid_argument("Incorrect input, matrix is not square");
  } else {
    if (rows == 1) {
      result = (*this)[0][0];
    } else if (rows == 2) {
      long double det = (*this)[0][0] * (*this)[1][1];
      det -= (*this)[0][1] * (*this)[1][0];
      if (det == INFINITY || fabs(det) > DOUBLE_MAX) {
        throw std::out_of_range("Determinant value is out of range");
      } else {
        result = (double)det;
      }
    } else {
      long double temp = 0;
      for (int j = 0; j < cols; j++) {
        Matrix matr_minor = minor(0, j);
        // double det = 0;
        double det = matr_minor.Determinant();
        if (det != 0) {
          temp += (*this)[0][j] * det * ((j & 1) ? -1 : 1);
        }
        // matr_minor.~Matrix();
      }
      result += (double)temp;
    }
  }
  return result;
}
Matrix Matrix::InverseMatrix() {
  if (rows != cols) {
    throw std::invalid_argument("Matrix not square");
  }
  double det = this->Determinant();
  if (det == 0) {
    throw std::invalid_argument("Zero determinant");
  }
  Matrix result(this->cols, this->rows);
  if (rows == 1) {
    result[0][0] = 1 / (*this)[0][0];
  }
  if (rows > 1) {
    Matrix temp_matr = this->CalcComplements();
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        temp_matr[i][j] = temp_matr[i][j] / det;
      }
    }
    result = temp_matr.Transpose();
  }
  return result;
}
Matrix Matrix::operator+(const Matrix& another) {
  Matrix res{*this};
  res.SumMatrix(another);
  return res;
}
Matrix& Matrix::operator+=(const Matrix& another) {
  SumMatrix(another);
  return (*this);
}
Matrix Matrix::operator-(const Matrix& another) {
  Matrix res(*this);
  res.SubMatrix(another);
  return res;
}
Matrix& Matrix::operator-=(const Matrix& another) {
  SubMatrix(another);
  return (*this);
}
Matrix Matrix::operator*(const Matrix& another) {
  Matrix res(*this);
  res.MulMatrix(another);
  return res;
}
Matrix Matrix::operator*(const double num) {
  Matrix res(*this);
  res.MulNumber(num);
  return res;
}
Matrix& Matrix::operator*=(const Matrix& another) {
  MulMatrix(another);
  return (*this);
}
Matrix Matrix::operator*=(const double num) {
  MulNumber(num);
  return (*this);
}
Matrix Matrix::operator=(const Matrix& another) noexcept {
  if (this != &another) {
    delete[] matrix_ptr;
    Copy(another);
  }
  return *this;
}
Matrix Matrix::operator=(Matrix&& another) noexcept {
  if (this != &another) {
    delete[] matrix_ptr;
    Move(another);
  }
  return *this;
}
bool Matrix::operator==(const Matrix& another) {
  return EqMatrix(another);
}

Matrix operator*(double num, Matrix& A) {
  Matrix res(A);
  res.MulNumber(num);
  return res;
}
Matrix operator*(Matrix& A, double num) {
  Matrix res(A);
  res.MulNumber(num);
  return res;
}
double& Matrix::operator()(int row, int col) & {
  if (row >= rows || col >= cols || row < 0 || cols < 0) {
    throw std::invalid_argument(
        "Incorrect input for (), index is out of range");
  }
  return const_cast<double&>(matrix_ptr[row * cols + col]);
}
double& Matrix::operator()(int row, int col) const& {
  if (row >= rows || col >= cols || row < 0 || cols < 0) {
    throw std::invalid_argument(
        "Incorrect input for () const&, index is out of range");
  }
  return matrix_ptr[row * cols + col];
}
double* Matrix::operator[](int row) const {
  if (row >= rows || row < 0) {
    throw std::invalid_argument(
        "Incorrect input for [], index is out of range");
  }
  return row * cols + matrix_ptr;
}

int Matrix::get_rows() const { return rows; }
int Matrix::get_cols() const { return cols; }
void Matrix::set_rows(const int new_rows) {
  if (new_rows < 0) {
    throw std::invalid_argument("Incorrect input row");
  } else {
    Matrix temp_matrix(new_rows, cols);
    for (int i = 0; i < (rows < new_rows ? rows : new_rows); i++) {
      for (int j = 0; j < cols; j++) {
        temp_matrix[i][j] = (*this)[i][j];
      }
    }
    rows = new_rows;
    *this = temp_matrix;
  }
}
void Matrix::set_cols(const int new_cols) {
  if (new_cols < 0) {
    throw std::invalid_argument("Incorrect input column");
  } else {
    Matrix temp_matrix(rows, new_cols);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < (cols < new_cols ? cols : new_cols); j++) {
        if (j < cols) {
          temp_matrix[i][j] = (*this)[i][j];
        }
      }
    }
    cols = new_cols;
    *this = temp_matrix;
  }
}

// void print_matrix(Matrix& A) {
//   std::cout << "\n";
//   for (int i = 0; i < A.get_rows(); i++) {
//     for (int j = 0; j < A.get_cols(); j++) {
//       std::cout << A(i, j) << "\t";
//       if ((j + 1) % A.get_cols() == 0 && i != 0) {
//       }
//     }
//     std::cout << "\n";
//   }
// }
void init_matrix(Matrix& A, double num, double shift) {
  for (int i = 0; i < A.get_rows(); i++) {
    for (int j = 0; j < A.get_cols(); j++) {
      num += shift;
      A(i, j) = num;
    }
  }
}
void Matrix::Copy(const Matrix& another) {
  rows = another.rows;
  cols = another.cols;
  matrix_ptr = new double[rows * cols]{0};
  std::memcpy(matrix_ptr, another.matrix_ptr, rows * cols * sizeof(double));
}
void Matrix::Move(Matrix& another) {
  rows = another.rows;
  cols = another.cols;
  matrix_ptr = new double[rows * cols]{0};
  std::memmove(matrix_ptr, another.matrix_ptr, rows * cols * sizeof(double));
  another.rows = 0;
  another.cols = 0;
}