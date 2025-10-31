#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <cmath>
#include <cstring>
#include <iostream>

#define DOUBLE_MAX 1.79769313486231570814527423731704357e+308L
#define DOUBLE_MIN 2.22507385850720138309023271733240406e-308L
#define DELTA_EPS 1e-7

class Matrix {
 private:
  // Attributes
  int rows, cols;      // Rows and columns
  double* matrix_ptr;  // Pointer to the memory where the matrix is allocated

 public:
  // Default constructor
  Matrix();
  // Parametrized constructor with number of rows and columns
  Matrix(int rows, int cols);
  // Copy constructor
  Matrix(const Matrix& another);
  // Move constructor
  Matrix(Matrix&& another);
  // Destructor
  ~Matrix();

  // Checks matrices for equality with each other
  bool EqMatrix(const Matrix& another);
  // Adds the second matrix to the current one
  void SumMatrix(const Matrix& another);
  // Subtracts another matrix from the current one
  void SubMatrix(const Matrix& another);
  // Multiplies the current matrix by a number
  void MulNumber(const double num);
  // Multiplies the current matrix by the second matrix
  void MulMatrix(const Matrix& another);
  // Creates a new transposed matrix from the current one and returns it
  Matrix Transpose();
  // Calculates the algebraic addition matrix of the current one and returns it
  Matrix CalcComplements();
  // Calculates and returns the determinant of the current matrix
  double Determinant();
  // Calculates and returns the inverse matrix
  Matrix InverseMatrix();
  // Addition of two matrices
  Matrix operator+(const Matrix& another);
  // Subtraction of one matrix from another
  Matrix operator-(const Matrix& another);
  // Matrix multiplication
  Matrix operator*(const Matrix& another);
  // Multiplication matrix by a number
  Matrix operator*(const double);
  // Checks for matrices equality (EqMatrix)
  bool operator==(const Matrix& another);
  // Assignment of values from one matrix to another one
  Matrix operator=(const Matrix& another) noexcept;
  // Move of values from one matrix to another one
  Matrix operator=(Matrix&& another) noexcept;
  // Addition assignment (SumMatrix)
  Matrix& operator+=(const Matrix& another);
  // Difference assignment (SubMatrix)
  Matrix& operator-=(const Matrix& another);
  // Multiplication assignment (MulMatrix/MulNumber)
  Matrix& operator*=(const Matrix& another);
  // Multiplication assignment matrix by a number
  Matrix operator*=(const double);
  // Indexation by matrix elements (row, column)
  double& operator()(int rows, int cols) &;
  // Indexation by matrix elements (row, column)
  double& operator()(int rows, int cols) const&;
  double* operator[](int row) const;

  friend Matrix operator*(double, Matrix&);
  friend Matrix operator*(Matrix&, double);
  Matrix minor(int row, int col);
  // secure public method of access (accessor)
  int get_rows() const;
  // secure public method of access (accessor)
  int get_cols() const;
  // secure public method of modification (mutator)
  void set_rows(const int new_rows);
  // secure public method of modification (mutator)
  void set_cols(const int new_cols);
  void Copy(const Matrix& another);
  void Move(Matrix& another);
  // void Matrix::CommonSumSubMatrix(const Matrix& another);
};
// Multiplication a number by matrix (overload)
Matrix operator*(double, Matrix&);
// Multiplication matrix by a number (overload)
Matrix operator*(Matrix&, double);
// Init matrix values
void init_matrix(Matrix&, double num, double shift);
// void print_matrix(Matrix&);
#endif
