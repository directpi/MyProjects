#include <gtest/gtest.h>

#include "../matrix_oop.h"

int main(int argc, char *argv[]) {
  // Передаем параметры командной строки в gtest
  testing::InitGoogleTest(&argc, argv);
  // Запускаем все тесты
  return RUN_ALL_TESTS();
}
/* Create exception */

TEST(TestConstructor, TestException_1) { EXPECT_ANY_THROW(Matrix(-10, 10)); }

TEST(TestConstructor, TestException_2) { EXPECT_ANY_THROW(Matrix(10, -10)); }

TEST(TestConstructor, TestException_3) { EXPECT_ANY_THROW(Matrix(-1, -1)); }

TEST(TestConstructor, TestException_4) {
  EXPECT_ANY_THROW(Matrix(-100, -100));
}

/* Success create */
TEST(TestConstructor, TestParameters_1) {
  Matrix m;
  EXPECT_EQ(m.get_cols(), 0);
  EXPECT_EQ(m.get_rows(), 0);
}
TEST(TestConstructor, TestParameters_2) {
  Matrix m(3, 4);
  EXPECT_EQ(m.get_rows(), 3);
  EXPECT_EQ(m.get_cols(), 4);
}

TEST(TestConstructor, TestParameters_3) {
  Matrix m(1000, 1234);
  EXPECT_EQ(m.get_rows(), 1000);
  EXPECT_EQ(m.get_cols(), 1234);
}

TEST(TestConstructor, TestCopy_1) {
  Matrix m(Matrix(2, 2));

  m(0, 0) = 7.0;
  m(0, 1) = 5.0;
  m(1, 0) = 3.0;
  m(1, 1) = 1.0;

  Matrix m_copy(m);

  EXPECT_EQ(m.get_cols(), 2);
  EXPECT_EQ(m.get_rows(), 2);

  EXPECT_EQ(m_copy.get_cols(), 2);
  EXPECT_EQ(m_copy.get_rows(), 2);

  EXPECT_EQ(m_copy(0, 0), m(0, 0));
  EXPECT_EQ(m_copy(0, 1), m(0, 1));
  EXPECT_EQ(m_copy(1, 0), m(1, 0));
  EXPECT_EQ(m_copy(1, 1), m(1, 1));
}

TEST(TestConstructor, TestCopy_2) {
  Matrix m(Matrix(3, 3));

  m(0, 0) = 1.0;
  m(0, 1) = 2.0;
  m(0, 2) = 3.0;
  m(1, 0) = 4.0;
  m(1, 1) = 5.0;
  m(1, 2) = 6.0;
  m(2, 0) = 7.0;
  m(2, 1) = 8.0;
  m(2, 2) = 9.0;

  Matrix m_copy(m);

  EXPECT_EQ(m.get_cols(), m_copy.get_cols());
  EXPECT_EQ(m.get_rows(), m_copy.get_rows());
  EXPECT_EQ(m(0, 0), m_copy(0, 0));
  EXPECT_EQ(m(0, 1), m_copy(0, 1));
  EXPECT_EQ(m(0, 2), m_copy(0, 2));
  EXPECT_EQ(m(1, 0), m_copy(1, 0));
  EXPECT_EQ(m(1, 1), m_copy(1, 1));
  EXPECT_EQ(m(1, 2), m_copy(1, 2));
  EXPECT_EQ(m(2, 0), m_copy(2, 0));
  EXPECT_EQ(m(2, 1), m_copy(2, 1));
  EXPECT_EQ(m(2, 2), m_copy(2, 2));
}

TEST(TestConstructor, TestMove_1) {
  Matrix m(6, 6);
  Matrix m_move(std::move(m));

  EXPECT_EQ(m.get_cols(), 0);
  EXPECT_EQ(m.get_rows(), 0);
  EXPECT_EQ(m_move.get_cols(), 6);
  EXPECT_EQ(m_move.get_rows(), 6);
}

TEST(TestConstructor, TestMove_2) {
  Matrix m_test{10, 10};
  Matrix m(std::move(m_test));
  // EXPECT_TRUE(m == m_test);
  EXPECT_EQ(m.get_cols(), 10);
  EXPECT_EQ(m.get_rows(), 10);
  EXPECT_EQ(m_test.get_rows(), 0);
  EXPECT_EQ(m_test.get_cols(), 0);
}
TEST(TestConstructor, TestMove_3) {
  Matrix m_test{1, 10};
  Matrix m(std::move(m_test));
  EXPECT_EQ(m.get_rows(), 1);
  EXPECT_EQ(m.get_cols(), 10);
  EXPECT_EQ(m_test.get_rows(), 0);
  EXPECT_EQ(m_test.get_cols(), 0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты доступа к приватным полям через accessors и mutators
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

TEST(TestConstructor, TestAccessorMutator_1) {
  Matrix m_test{4, 4};
  Matrix m{2, 2};
  m.set_rows(4);
  m.set_cols(4);
  m = m_test;
  EXPECT_EQ(m.get_cols(), 4);
  EXPECT_EQ(m.get_rows(), 4);
  EXPECT_EQ(m_test.get_rows(), 4);
  EXPECT_EQ(m_test.get_rows(), 4);
  EXPECT_TRUE(m == m_test);
}

TEST(TestConstructor, TestAccessorMutator_2) {
  Matrix matrix_test{5, 5};
  Matrix m{2, 2};
  m.set_rows(5);
  m.set_cols(5);
  m = matrix_test;
  EXPECT_EQ(m.get_cols(), 5);
  EXPECT_EQ(m.get_rows(), 5);
  EXPECT_EQ(matrix_test.get_rows(), 5);
  EXPECT_EQ(matrix_test.get_rows(), 5);
  EXPECT_TRUE(m == matrix_test);
}
TEST(TestConstructor, TestAccessorMutator_3) {
  Matrix matrix_test(5, 5);
  init_matrix(matrix_test, 0, 1);
  Matrix m(7, 7);
  m.set_rows(5);
  m.set_cols(5);
  m = matrix_test;
  EXPECT_EQ(m.get_cols(), 5);
  EXPECT_EQ(m.get_rows(), 5);
  EXPECT_TRUE(m == matrix_test);
}
TEST(TestConstructor, TestAccessorMutator_4) {
  Matrix matrix_test{1, 1};
  Matrix m{2, 2};
  m.set_rows(1);
  m.set_cols(1);
  m = matrix_test;
  EXPECT_EQ(m.get_cols(), 1);
  EXPECT_EQ(m.get_rows(), 1);
  EXPECT_EQ(matrix_test.get_rows(), 1);
  EXPECT_EQ(matrix_test.get_rows(), 1);
  EXPECT_TRUE(m == matrix_test);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты SumMatrix
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestMatrixSum, TestSum_1) {
  Matrix m2{3, 3};
  init_matrix(m2, 0, 1);
  Matrix m{3, 3};
  init_matrix(m, 0, 1);
  m.SumMatrix(m2);
  EXPECT_EQ(m[0][0], 2);
  EXPECT_EQ(m[0][1], 4);
  EXPECT_EQ(m[0][2], 6);
  EXPECT_EQ(m[1][0], 8);
  EXPECT_EQ(m[1][1], 10);
  EXPECT_EQ(m[1][2], 12);
  EXPECT_EQ(m[2][0], 14);
  EXPECT_EQ(m[2][1], 16);
  EXPECT_EQ(m[2][2], 18);
}
TEST(TestMatrixSum, TestSum_2) {
  Matrix m2{1, 1};
  init_matrix(m2, 0, 1);
  Matrix m{1, 1};
  init_matrix(m, 0, 1);
  m.SumMatrix(m2);
  EXPECT_EQ(m[0][0], 2);
}
TEST(TestMatrixSum, TestSum_3) {
  Matrix m2{2, 2};
  init_matrix(m2, 10, 0);
  Matrix m{2, 2};
  init_matrix(m, 10, 0);
  m.SumMatrix(m2);
  EXPECT_EQ(m[0][0], 20);
  EXPECT_EQ(m[0][1], 20);
  EXPECT_EQ(m[1][0], 20);
  EXPECT_EQ(m[1][1], 20);
}
TEST(TestMatrixSum, TestSum_4) {
  Matrix m2;
  Matrix m{2, 2};
  init_matrix(m, 10, 0);
  EXPECT_ANY_THROW(m.SumMatrix(m2));
}
TEST(TestMatrixSum, TestSum_5) {
  Matrix m2{2, 1};
  init_matrix(m2, 10, 0);
  Matrix m{2, 2};
  init_matrix(m, 10, 0);
  EXPECT_ANY_THROW(m.SumMatrix(m2));
}
TEST(TestMatrixSum, TestSum_6) {
  Matrix m2{2, 2};
  init_matrix(m2, -10, 0);
  Matrix m{2, 2};
  init_matrix(m, 10, 0);
  m.SumMatrix(m2);
  EXPECT_EQ(m[0][0], 0);
  EXPECT_EQ(m[0][1], 0);
  EXPECT_EQ(m[1][0], 0);
  EXPECT_EQ(m[1][1], 0);
}
TEST(TestMatrixSum, TestSum_7) {
  Matrix m2{3, 3};
  init_matrix(m2, 0, 1);
  Matrix m{3, 3};
  init_matrix(m, 0, 1);
  m.SumMatrix(m2);
  Matrix result{3, 3};
  result[0][0] = 2;
  result[0][1] = 4;
  result[0][2] = 6;
  result[1][0] = 8;
  result[1][1] = 10;
  result[1][2] = 12;
  result[2][0] = 14;
  result[2][1] = 16;
  result[2][2] = 18;
  EXPECT_EQ(result.EqMatrix(m), true);
}
TEST(TestMatrixSum, TestSum_8) {
  Matrix m2{3, 3};
  init_matrix(m2, 0, 1);
  Matrix m{3, 3};
  init_matrix(m, -9, 1);
  m.SumMatrix(m2);
  Matrix result{3, 3};
  result[0][0] = -7;
  result[0][1] = -5;
  result[0][2] = -3;
  result[1][0] = -1;
  result[1][1] = 1;
  result[1][2] = 3;
  result[2][0] = 5;
  result[2][1] = 7;
  result[2][2] = 9;
  EXPECT_EQ(result.EqMatrix(m), true);
}
TEST(TestMatrixSum, TestSum_9) {
  Matrix m2{3, 3};
  init_matrix(m2, DOUBLE_MAX, 0);
  Matrix m{3, 3};
  init_matrix(m, 9, 1);
  EXPECT_ANY_THROW(m.SumMatrix(m2));
}
TEST(TestMatrixSum, TestSum_10) {
  Matrix m2{3, 3};
  init_matrix(m2, INFINITY, 0);
  Matrix m{3, 3};
  init_matrix(m, INFINITY, 1);
  EXPECT_ANY_THROW(m.SumMatrix(m2));
}
TEST(TestMatrixSum, TestSum_11) {
  const Matrix m2{3, 3};
  Matrix m{3, 3};
  init_matrix(m, 0, 1);
  m.SumMatrix(m2);
  EXPECT_EQ(m[0][0], 1);
  EXPECT_EQ(m[0][1], 2);
  EXPECT_EQ(m[0][2], 3);
  EXPECT_EQ(m[1][0], 4);
  EXPECT_EQ(m[1][1], 5);
  EXPECT_EQ(m[1][2], 6);
  EXPECT_EQ(m[2][0], 7);
  EXPECT_EQ(m[2][1], 8);
  EXPECT_EQ(m[2][2], 9);
}
TEST(TestMatrixSum, TestSum_12) {
  const Matrix m2{1, 1};
  Matrix m{1, 1};
  init_matrix(m, 0, 1);
  m.SumMatrix(m2);
  EXPECT_EQ(m[0][0], 1);
}

TEST(TestMatrixSum, TestSum_13) {
  Matrix m2{3, 3};
  init_matrix(m2, INFINITY, 0);
  const Matrix m{3, 3};
  EXPECT_ANY_THROW(m2.SumMatrix(m));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты SubMatrix
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestMatrixSub, TestSub_1) {
  Matrix m2{3, 3};
  init_matrix(m2, 0, 1);
  Matrix m{3, 3};
  init_matrix(m, 0, 1);
  m.SubMatrix(m2);
  EXPECT_EQ(m[0][0], 0);
  EXPECT_EQ(m[0][1], 0);
  EXPECT_EQ(m[0][2], 0);
  EXPECT_EQ(m[1][0], 0);
  EXPECT_EQ(m[1][1], 0);
  EXPECT_EQ(m[1][2], 0);
  EXPECT_EQ(m[2][0], 0);
  EXPECT_EQ(m[2][1], 0);
  EXPECT_EQ(m[2][2], 0);
}
TEST(TestMatrixSub, TestSub_2) {
  Matrix m2{1, 1};
  init_matrix(m2, 0, 1);
  Matrix m{1, 1};
  init_matrix(m, 1, 1);
  m.SubMatrix(m2);
  EXPECT_EQ(m[0][0], 1);
}
TEST(TestMatrixSub, TestSub_3) {
  Matrix m2{2, 2};
  init_matrix(m2, 10, 0);
  Matrix m{2, 2};
  init_matrix(m, 20, 0);
  m.SubMatrix(m2);
  EXPECT_EQ(m[0][0], 10);
  EXPECT_EQ(m[0][1], 10);
  EXPECT_EQ(m[1][0], 10);
  EXPECT_EQ(m[1][1], 10);
}
TEST(TestMatrixSub, TestSub_4) {
  Matrix m2;
  Matrix m{2, 2};
  init_matrix(m, 10, 0);
  EXPECT_ANY_THROW(m.SubMatrix(m2));
}
TEST(TestMatrixSub, TestSub_5) {
  Matrix m2{2, 1};
  init_matrix(m2, 10, 0);
  Matrix m{2, 2};
  init_matrix(m, 10, 0);
  EXPECT_ANY_THROW(m.SubMatrix(m2));
}
TEST(TestMatrixSub, TestSub_6) {
  Matrix m2{3, 3};
  init_matrix(m2, 0, 1);
  Matrix m{3, 3};
  init_matrix(m, 1, 1);
  m.SubMatrix(m2);
  EXPECT_EQ(m[0][0], 1);
  EXPECT_EQ(m[0][1], 1);
  EXPECT_EQ(m[0][2], 1);
  EXPECT_EQ(m[1][0], 1);
  EXPECT_EQ(m[1][1], 1);
  EXPECT_EQ(m[1][2], 1);
  EXPECT_EQ(m[2][0], 1);
  EXPECT_EQ(m[2][1], 1);
  EXPECT_EQ(m[2][2], 1);
}
TEST(TestMatrixSub, TestSub_7) {
  Matrix m2{3, 3};
  init_matrix(m2, 1, 1);
  Matrix m{3, 3};
  init_matrix(m, -1, -1);
  m.SubMatrix(m2);
  EXPECT_EQ(m[0][0], -4);
  EXPECT_EQ(m[0][1], -6);
  EXPECT_EQ(m[0][2], -8);
  EXPECT_EQ(m[1][0], -10);
  EXPECT_EQ(m[1][1], -12);
  EXPECT_EQ(m[1][2], -14);
  EXPECT_EQ(m[2][0], -16);
  EXPECT_EQ(m[2][1], -18);
  EXPECT_EQ(m[2][2], -20);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты MulNumber и MulMatrix
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestMatrixMul, TestMulNum_1) {
  Matrix m{3, 3};
  init_matrix(m, 0, 1);
  m.MulNumber(10);
  EXPECT_EQ(m[0][0], 10);
  EXPECT_EQ(m[0][1], 20);
  EXPECT_EQ(m[0][2], 30);
  EXPECT_EQ(m[1][0], 40);
  EXPECT_EQ(m[1][1], 50);
  EXPECT_EQ(m[1][2], 60);
  EXPECT_EQ(m[2][0], 70);
  EXPECT_EQ(m[2][1], 80);
  EXPECT_EQ(m[2][2], 90);
}
TEST(TestMatrixMul, TestMulNum_2) {
  Matrix m{3, 3};
  init_matrix(m, 0, 1);
  m.MulNumber(0);
  EXPECT_EQ(m[0][0], 0);
  EXPECT_EQ(m[0][1], 0);
  EXPECT_EQ(m[0][2], 0);
  EXPECT_EQ(m[1][0], 0);
  EXPECT_EQ(m[1][1], 0);
  EXPECT_EQ(m[1][2], 0);
  EXPECT_EQ(m[2][0], 0);
  EXPECT_EQ(m[2][1], 0);
  EXPECT_EQ(m[2][2], 0);
}
TEST(TestMatrixMul, TestMulNum_3) {
  Matrix m{3, 3};
  init_matrix(m, 0, 1);
  m.MulNumber(-1);
  EXPECT_EQ(m[0][0], -1);
  EXPECT_EQ(m[0][1], -2);
  EXPECT_EQ(m[0][2], -3);
  EXPECT_EQ(m[1][0], -4);
  EXPECT_EQ(m[1][1], -5);
  EXPECT_EQ(m[1][2], -6);
  EXPECT_EQ(m[2][0], -7);
  EXPECT_EQ(m[2][1], -8);
  EXPECT_EQ(m[2][2], -9);
}
TEST(TestMatrixMul, TestMulNum_4) {
  Matrix m{3, 3};
  init_matrix(m, 0, -1);
  m.MulNumber(-1);
  EXPECT_EQ(m[0][0], 1);
  EXPECT_EQ(m[0][1], 2);
  EXPECT_EQ(m[0][2], 3);
  EXPECT_EQ(m[1][0], 4);
  EXPECT_EQ(m[1][1], 5);
  EXPECT_EQ(m[1][2], 6);
  EXPECT_EQ(m[2][0], 7);
  EXPECT_EQ(m[2][1], 8);
  EXPECT_EQ(m[2][2], 9);
}
TEST(TestMatrixMul, TestMulNum_5) {
  Matrix m{3, 3};
  init_matrix(m, DOUBLE_MAX, 0);
  EXPECT_ANY_THROW(m.MulNumber(10));
}
TEST(TestMatrixMul, TestMulNum_6) {
  Matrix m{3, 3};
  init_matrix(m, INFINITY, 0);
  EXPECT_ANY_THROW(m.MulNumber(100));
}
TEST(TestMatrixMul, TestMulMatrix_1) {
  Matrix m{3, 2};
  init_matrix(m, 0, 1);
  Matrix m2{2, 3};
  init_matrix(m2, 0, 1);
  m.MulMatrix(m2);
  EXPECT_EQ(m[0][0], 9);
  EXPECT_EQ(m[0][1], 12);
  EXPECT_EQ(m[0][2], 15);
  EXPECT_EQ(m[1][0], 19);
  EXPECT_EQ(m[1][1], 26);
  EXPECT_EQ(m[1][2], 33);
  EXPECT_EQ(m[2][0], 29);
  EXPECT_EQ(m[2][1], 40);
  EXPECT_EQ(m[2][2], 51);
}
TEST(TestMatrixMul, TestMulMatrix_2) {
  Matrix m{3, 3};
  Matrix m2{5, 5};
  EXPECT_ANY_THROW(m.MulMatrix(m2));
}
TEST(TestMatrixMul, TestMulMatrix_3) {
  Matrix m;
  Matrix m2{2, 2};
  EXPECT_ANY_THROW(m.MulMatrix(m2));
}

TEST(TestMatrixMul, TestMulMatrix_4) {
  Matrix m{3, 2};
  init_matrix(m, 1, 0);
  Matrix m2{2, 3};
  init_matrix(m2, 2, 0);
  m.MulMatrix(m2);
  EXPECT_EQ(m[0][0], 4);
  EXPECT_EQ(m[0][1], 4);
  EXPECT_EQ(m[0][2], 4);
  EXPECT_EQ(m[1][0], 4);
  EXPECT_EQ(m[1][1], 4);
  EXPECT_EQ(m[1][2], 4);
  EXPECT_EQ(m[2][0], 4);
  EXPECT_EQ(m[2][1], 4);
  EXPECT_EQ(m[2][2], 4);
}
TEST(TestMatrixMul, TestMulMatrix_5) {
  Matrix m{2, 2};
  init_matrix(m, DOUBLE_MAX, 0);
  Matrix m2{2, 2};
  init_matrix(m2, DOUBLE_MAX, 0);
  EXPECT_ANY_THROW(m.MulMatrix(m2));
}
TEST(TestMatrixMul, TestMulMatrix_6) {
  Matrix m{2, 2};
  init_matrix(m, INFINITY, 0);
  Matrix m2{2, 2};
  init_matrix(m2, INFINITY, 0);
  EXPECT_ANY_THROW(m.MulMatrix(m2));
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты EqMatrix
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestMatrixEq, TestEq_1) {
  Matrix m{3, 2};
  init_matrix(m, 1, 0);
  Matrix m2{2, 3};
  init_matrix(m2, 2, 0);
  EXPECT_ANY_THROW(m.EqMatrix(m2));
}
TEST(TestMatrixEq, TestEq_2) {
  Matrix m;
  Matrix m2{2, 3};
  init_matrix(m2, 2, 0);
  EXPECT_ANY_THROW(m.EqMatrix(m2));
}
TEST(TestMatrixEq, TestEq_3) {
  Matrix m{3, 3};
  init_matrix(m, 1, 0);
  Matrix m2{3, 3};
  init_matrix(m2, 1, 0);
  EXPECT_EQ(m.EqMatrix(m2), true);
}
TEST(TestMatrixEq, TestEq_4) {
  Matrix m{3, 3};
  init_matrix(m, 0, -1);
  Matrix m2{3, 3};
  init_matrix(m2, 0, -1);
  EXPECT_EQ(m.EqMatrix(m2), true);
}
TEST(TestMatrixEq, TestEq_5) {
  Matrix m;
  Matrix m2;
  EXPECT_EQ(m.EqMatrix(m2), true);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты Transpose
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestMatrixTranspose, TestTranspose_1) {
  Matrix m{3, 3};
  init_matrix(m, 0, 1);
  Matrix m2 = m.Transpose();
  EXPECT_EQ(m2[0][0], 1);
  EXPECT_EQ(m2[0][1], 4);
  EXPECT_EQ(m2[0][2], 7);
  EXPECT_EQ(m2[1][0], 2);
  EXPECT_EQ(m2[1][1], 5);
  EXPECT_EQ(m2[1][2], 8);
  EXPECT_EQ(m2[2][0], 3);
  EXPECT_EQ(m2[2][1], 6);
  EXPECT_EQ(m2[2][2], 9);
  EXPECT_EQ(m2.get_rows(), m.get_cols());
  EXPECT_EQ(m2.get_cols(), m.get_rows());
}
TEST(TestMatrixTranspose, TestTranspose_2) {
  Matrix m{2, 2};
  init_matrix(m, 0, -1);
  Matrix m2 = m.Transpose();
  EXPECT_EQ(m2[0][0], -1);
  EXPECT_EQ(m2[0][1], -3);
  EXPECT_EQ(m2[1][0], -2);
  EXPECT_EQ(m2[1][1], -4);
  EXPECT_EQ(m2.get_rows(), m.get_cols());
  EXPECT_EQ(m2.get_cols(), m.get_rows());
}
TEST(TestMatrixTranspose, TestTranspose_3) {
  Matrix m{3, 2};
  init_matrix(m, 0, 1);
  Matrix m2 = m.Transpose();
  EXPECT_EQ(m2[0][0], 1);
  EXPECT_EQ(m2[0][1], 3);
  EXPECT_EQ(m2[0][2], 5);
  EXPECT_EQ(m2[1][0], 2);
  EXPECT_EQ(m2[1][1], 4);
  EXPECT_EQ(m2[1][2], 6);
  EXPECT_EQ(m2.get_rows(), m.get_cols());
  EXPECT_EQ(m2.get_cols(), m.get_rows());
}
TEST(TestMatrixTranspose, TestTranspose_4) {
  Matrix m{1, 2};
  init_matrix(m, 0, 1);
  Matrix m2 = m.Transpose();
  EXPECT_EQ(m2[0][0], 1);
  EXPECT_EQ(m2[1][0], 2);
  EXPECT_EQ(m2.get_rows(), m.get_cols());
  EXPECT_EQ(m2.get_cols(), m.get_rows());
}
TEST(TestMatrixTranspose, TestTranspose_5) {
  Matrix m{1, 1};
  init_matrix(m, 0, 1);
  Matrix m2 = m.Transpose();
  EXPECT_EQ(m2[0][0], 1);
  EXPECT_EQ(m2.get_rows(), m.get_cols());
  EXPECT_EQ(m2.get_cols(), m.get_rows());
}
TEST(TestMatrixTranspose, TestTranspose_6) {
  Matrix m;
  EXPECT_ANY_THROW(m.Transpose());
}
TEST(TestMatrixTranspose, TestTranspose_7) {
  Matrix m{3, 2};
  Matrix result{2, 3};
  m[0][0] = 1;
  m[0][1] = 4;
  m[1][0] = 2;
  m[1][1] = 5;
  m[2][0] = 3;
  m[2][1] = 6;
  Matrix m2 = m.Transpose();
  result[0][0] = 1;
  result[0][1] = 2;
  result[0][2] = 3;
  result[1][0] = 4;
  result[1][1] = 5;
  result[1][2] = 6;
  EXPECT_EQ(result.EqMatrix(m2), true);
  EXPECT_EQ(m2.get_rows(), m.get_cols());
  EXPECT_EQ(m2.get_cols(), m.get_rows());
}
TEST(TestMatrixTranspose, TestTranspose_8) {
  Matrix m{1, 3};
  Matrix result{3, 1};
  m[0][0] = 1.9999999;
  m[0][1] = 2.5555555;
  m[0][2] = 3.7777777;
  Matrix m2 = m.Transpose();
  result[0][0] = 1.9999999;
  result[1][0] = 2.5555555;
  result[2][0] = 3.7777777;
  EXPECT_EQ(result.EqMatrix(m2), true);
  EXPECT_EQ(m2.get_rows(), m.get_cols());
  EXPECT_EQ(m2.get_cols(), m.get_rows());
}
TEST(TestMatrixTranspose, TestTranspose_9) {
  Matrix m{3, 2};
  Matrix result{2, 3};
  m[0][0] = -1;
  m[0][1] = -4;
  m[1][0] = -2;
  m[1][1] = -5;
  m[2][0] = -3;
  m[2][1] = -6;
  Matrix m2 = m.Transpose();
  result[0][0] = -1;
  result[0][1] = -2;
  result[0][2] = -3;
  result[1][0] = -4;
  result[1][1] = -5;
  result[1][2] = -6;
  EXPECT_EQ(result.EqMatrix(m2), true);
  EXPECT_EQ(m2.get_rows(), m.get_cols());
  EXPECT_EQ(m2.get_cols(), m.get_rows());
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты Determinant
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestMatrixDeterminant, TestDeterminant_1) {
  Matrix m{3, 3};
  init_matrix(m, 0, 1);
  EXPECT_EQ(m.Determinant(), 0);
}
TEST(TestMatrixDeterminant, TestDeterminant_2) {
  Matrix m{1, 1};
  init_matrix(m, 1, 0);
  EXPECT_EQ(m.Determinant(), 1);
}
TEST(TestMatrixDeterminant, TestDeterminant_3) {
  Matrix m{5, 5};
  m[0][0] = 1;
  m[0][1] = 2;
  m[0][2] = 0;
  m[0][3] = -2;
  m[0][4] = 6;
  m[1][0] = 4;
  m[1][1] = 5;
  m[1][2] = 0;
  m[1][3] = 8;
  m[1][4] = 5;
  m[2][0] = 0;
  m[2][1] = 0;
  m[2][2] = -7;
  m[2][3] = 0;
  m[2][4] = 4;
  m[3][0] = 5;
  m[3][1] = 7;
  m[3][2] = 16;
  m[3][3] = -6;
  m[3][4] = -5;
  m[4][0] = 0;
  m[4][1] = -2;
  m[4][2] = 4;
  m[4][3] = 11;
  m[4][4] = -4;
  EXPECT_EQ(m.Determinant(), -2712);
}
TEST(TestMatrixDeterminant, TestDeterminant_4) {
  Matrix m{3, 3};
  m[0][0] = 1;
  m[0][1] = 2;
  m[0][2] = 3;
  m[1][0] = 3;
  m[1][1] = 2;
  m[1][2] = 1;
  m[2][0] = 2;
  m[2][1] = 1;
  m[2][2] = 3;
  EXPECT_EQ(m.Determinant(), -12);
}
TEST(TestMatrixDeterminant, TestDeterminant_5) {
  Matrix m;
  EXPECT_EQ(m.Determinant(), 0);
}
TEST(TestMatrixDeterminant, TestDeterminant_6) {
  Matrix m{3, 3};
  m[0][0] = 1;
  m[0][1] = 2;
  m[0][2] = 3;
  m[1][0] = 0;
  m[1][1] = 4;
  m[1][2] = 2;
  m[2][0] = 5;
  m[2][1] = 2;
  m[2][2] = 1;
  EXPECT_EQ(m.Determinant(), -40);
}
TEST(TestMatrixDeterminant, TestDeterminant_7) {
  Matrix m{5, 5};
  m[0][0] = -1;
  m[0][1] = 2;
  m[0][2] = 3;
  m[0][3] = 0;
  m[0][4] = 5;
  m[1][0] = 3;
  m[1][1] = 5;
  m[1][2] = 6;
  m[1][3] = 9;
  m[1][4] = 2;
  m[2][0] = -7;
  m[2][1] = -4;
  m[2][2] = 2;
  m[2][3] = 15;
  m[2][4] = 32;
  m[3][0] = 11;
  m[3][1] = -10;
  m[3][2] = 0;
  m[3][3] = 2;
  m[3][4] = 7;
  m[4][0] = 1;
  m[4][1] = 14;
  m[4][2] = -15;
  m[4][3] = 1;
  m[4][4] = 2;
  EXPECT_EQ(m.Determinant(), 470696);
}
TEST(TestMatrixDeterminant, TestDeterminant_8) {
  Matrix m{3, 3};
  m[0][0] = -1;
  m[0][1] = -4;
  m[0][2] = -2;
  m[1][0] = -5;
  m[1][1] = -3;
  m[1][2] = -6;
  m[2][0] = -7;
  m[2][1] = 0;
  m[2][2] = -8;
  EXPECT_EQ(m.Determinant(), 10);
}
TEST(TestMatrixDeterminant, TestDeterminant_9) {
  Matrix m{1, 3};
  EXPECT_ANY_THROW(m.Determinant());
}
TEST(TestMatrixDeterminant, TestDeterminant_10) {
  Matrix m{10, 5};
  EXPECT_ANY_THROW(m.Determinant());
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты Minor
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestMatrixMinor, TestMinor_1) {
  Matrix m{3, 3};
  init_matrix(m, 0, 1);
  Matrix m2 = m.minor(0, 0);
  EXPECT_EQ(m2[0][0], 5);
  EXPECT_EQ(m2[0][1], 6);
  EXPECT_EQ(m2[1][0], 8);
  EXPECT_EQ(m2[1][1], 9);
}
TEST(TestMatrixMinor, TestMinor_2) {
  Matrix m{4, 4};
  init_matrix(m, 0, 1);
  Matrix m2 = m.minor(0, 0);
  EXPECT_EQ(m2[0][0], 6);
  EXPECT_EQ(m2[0][1], 7);
  EXPECT_EQ(m2[0][2], 8);
  EXPECT_EQ(m2[1][0], 10);
  EXPECT_EQ(m2[1][1], 11);
  EXPECT_EQ(m2[1][2], 12);
  EXPECT_EQ(m2[2][0], 14);
  EXPECT_EQ(m2[2][1], 15);
  EXPECT_EQ(m2[2][2], 16);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты CalcComplements
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestCalcComplements, TestCompl_1) {
  Matrix m1{3, 3};
  Matrix m2{3, 3};
  m1[0][0] = 1;
  m1[0][1] = 2;
  m1[0][2] = 3;
  m1[1][0] = 0;
  m1[1][1] = 4;
  m1[1][2] = 2;
  m1[2][0] = 5;
  m1[2][1] = 2;
  m1[2][2] = 1;
  m2[0][0] = 0;
  m2[0][1] = 10;
  m2[0][2] = -20;
  m2[1][0] = 4;
  m2[1][1] = -14;
  m2[1][2] = 8;
  m2[2][0] = -8;
  m2[2][1] = -2;
  m2[2][2] = 4;

  Matrix result = m1.CalcComplements();
  EXPECT_EQ(result.get_cols(), m2.get_cols());
  EXPECT_EQ(result.get_rows(), m2.get_rows());
  EXPECT_EQ(result.EqMatrix(m2), true);
}
TEST(TestCalcComplements, TestCompl_2) {
  Matrix m1{3, 3};
  Matrix m2{3, 3};
  m1[0][0] = 1;
  m1[0][1] = 2;
  m1[0][2] = 3;
  m1[1][0] = 4;
  m1[1][1] = 5;
  m1[1][2] = 6;
  m1[2][0] = 7;
  m1[2][1] = 8;
  m1[2][2] = 9;
  m2[0][0] = -3;
  m2[0][1] = 6;
  m2[0][2] = -3;
  m2[1][0] = 6;
  m2[1][1] = -12;
  m2[1][2] = 6;
  m2[2][0] = -3;
  m2[2][1] = 6;
  m2[2][2] = -3;

  Matrix result = m1.CalcComplements();
  EXPECT_EQ(result.get_cols(), m2.get_cols());
  EXPECT_EQ(result.get_rows(), m2.get_rows());
  EXPECT_EQ(result.EqMatrix(m2), true);
}
TEST(TestCalcComplements, TestCompl_3) {
  Matrix m1{2, 2};
  Matrix m2{2, 2};
  m1[0][0] = 1;
  m1[0][1] = 2;
  m1[1][0] = 3;
  m1[1][1] = 5;
  m2[0][0] = 5;
  m2[0][1] = -3;
  m2[1][0] = -2;
  m2[1][1] = 1;

  Matrix result = m1.CalcComplements();
  EXPECT_EQ(result.get_cols(), m2.get_cols());
  EXPECT_EQ(result.get_rows(), m2.get_rows());
  EXPECT_EQ(result.EqMatrix(m2), true);
}
TEST(TestCalcComplements, TestCompl_4) {
  Matrix m1{1, 1};
  Matrix m2{1, 1};
  m1[0][0] = 1;
  m2[0][0] = 0;

  Matrix result = m1.CalcComplements();
  EXPECT_EQ(result.get_cols(), m2.get_rows());
  EXPECT_EQ(result.get_rows(), m2.get_cols());
  EXPECT_EQ(result.EqMatrix(m2), true);
}
TEST(TestCalcComplements, TestCompl_6) {
  Matrix m1{3, 3};
  Matrix m2{3, 3};
  m1[0][0] = -1;
  m1[0][1] = -4;
  m1[0][2] = -2;
  m1[1][0] = -5;
  m1[1][1] = -3;
  m1[1][2] = -6;
  m1[2][0] = -7;
  m1[2][1] = 0;
  m1[2][2] = -8;
  m2[0][0] = 24;
  m2[0][1] = 2;
  m2[0][2] = -21;
  m2[1][0] = -32;
  m2[1][1] = -6;
  m2[1][2] = 28;
  m2[2][0] = 18;
  m2[2][1] = 4;
  m2[2][2] = -17;

  Matrix result = m1.CalcComplements();
  EXPECT_EQ(result.get_cols(), m2.get_rows());
  EXPECT_EQ(result.get_rows(), m2.get_cols());
  EXPECT_EQ(result.EqMatrix(m2), true);
}
TEST(TestCalcComplements, TestCompl_7) {
  Matrix m1{1, 3};
  Matrix m2{1, 3};
  EXPECT_ANY_THROW(m1.CalcComplements());
}

TEST(TestCalcComplements, TestCompl_8) {
  Matrix m1{3, 3};
  Matrix m2{3, 3};
  m1[0][0] = 5;
  m1[0][1] = 10;
  m1[0][2] = 15;
  m1[1][0] = 20;
  m1[1][1] = 25;
  m1[1][2] = 30;
  m1[2][0] = 35;
  m1[2][1] = 40;
  m1[2][2] = 45;

  m2[0][0] = -75;
  m2[0][1] = 150;
  m2[0][2] = -75;
  m2[1][0] = 150;
  m2[1][1] = -300;
  m2[1][2] = 150;
  m2[2][0] = -75;
  m2[2][1] = 150;
  m2[2][2] = -75;

  Matrix result = m1.CalcComplements();
  EXPECT_EQ(result.get_cols(), m2.get_rows());
  EXPECT_EQ(result.get_rows(), m2.get_cols());
  EXPECT_EQ(result.EqMatrix(m2), true);
}
TEST(TestCalcComplements, TestCompl_9) {
  Matrix m1{3, 3};
  Matrix m2{3, 3};
  init_matrix(m1, 0, 0);
  init_matrix(m2, 0, 0);
  Matrix result = m1.CalcComplements();
  EXPECT_EQ(result.get_cols(), m2.get_rows());
  EXPECT_EQ(result.get_rows(), m2.get_cols());
  EXPECT_EQ(result.EqMatrix(m2), true);
}
TEST(TestCalcComplements, TestCompl_10) {
  Matrix m1;
  EXPECT_ANY_THROW(m1.CalcComplements());
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты InverseMatrix
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestInverseMatrix, TestInverse_1) {
  Matrix m1{3, 3};
  init_matrix(m1, 1, 1);
  EXPECT_ANY_THROW(m1.InverseMatrix());
}
TEST(TestInverseMatrix, TestInverse_2) {
  Matrix m1{3, 3};
  m1[0][0] = 5;
  m1[0][1] = 5;
  m1[0][2] = 5;
  m1[1][0] = 1;
  m1[1][1] = 1.7976931348623157e+308;
  m1[1][2] = 0;
  m1[2][0] = 1;
  m1[2][1] = 0;
  m1[2][2] = 1.7976931348623157e+308;

  EXPECT_ANY_THROW(m1.InverseMatrix());
}
TEST(TestInverseMatrix, TestInverse_3) {
  Matrix m1{3, 3};
  Matrix m2{3, 3};
  m1[0][0] = 2;
  m1[0][1] = 5;
  m1[0][2] = 7;
  m1[1][0] = 6;
  m1[1][1] = 3;
  m1[1][2] = 4;
  m1[2][0] = 5;
  m1[2][1] = -2;
  m1[2][2] = -3;
  m2[0][0] = 1;
  m2[0][1] = -1;
  m2[0][2] = 1;
  m2[1][0] = -38;
  m2[1][1] = 41;
  m2[1][2] = -34;
  m2[2][0] = 27;
  m2[2][1] = -29;
  m2[2][2] = 24;
  Matrix result = m1.InverseMatrix();
  EXPECT_EQ(result.get_cols(), m2.get_rows());
  EXPECT_EQ(result.get_rows(), m2.get_cols());
  EXPECT_EQ(result.EqMatrix(m2), true);
}
TEST(TestInverseMatrix, TestInverse_4) {
  Matrix m1{3, 3};
  Matrix m2{3, 3};
  m1[0][0] = 1;
  m1[0][1] = 1;
  m1[0][2] = 8;
  m1[1][0] = 0;
  m1[1][1] = 2;
  m1[1][2] = 2;
  m1[2][0] = 5;
  m1[2][1] = 0;
  m1[2][2] = 0;

  m2[0][0] = 0;
  m2[0][1] = 0;
  m2[0][2] = 0.2;
  m2[1][0] = -0.1428571;
  m2[1][1] = 0.5714286;
  m2[1][2] = 0.0285714;
  m2[2][0] = 0.1428571;
  m2[2][1] = -0.0714286;
  m2[2][2] = -0.0285714;
  Matrix result = m1.InverseMatrix();
  EXPECT_EQ(result.get_cols(), m2.get_rows());
  EXPECT_EQ(result.get_rows(), m2.get_cols());
  EXPECT_EQ(result.EqMatrix(m2), true);
}
TEST(TestInverseMatrix, TestInverse_5) {
  Matrix m1{3, 3};
  Matrix m2{3, 3};
  m1[0][0] = 2;
  m1[0][1] = 0;
  m1[0][2] = -1;
  m1[1][0] = 1;
  m1[1][1] = 5;
  m1[1][2] = -4;
  m1[2][0] = -1;
  m1[2][1] = 1;
  m1[2][2] = 0;
  m2[0][0] = 2;
  m2[0][1] = -0.5;
  m2[0][2] = 2.5;
  m2[1][0] = 2;
  m2[1][1] = -0.5;
  m2[1][2] = 3.5;
  m2[2][0] = 3.0;
  m2[2][1] = -1.0;
  m2[2][2] = 5.0;
  Matrix result = m1.InverseMatrix();
  EXPECT_EQ(result.get_cols(), m2.get_rows());
  EXPECT_EQ(result.get_rows(), m2.get_cols());
  EXPECT_EQ(result.EqMatrix(m2), true);
}
TEST(TestInverseMatrix, TestInverse_6) {
  Matrix m1{1, 1};
  Matrix m2{1, 1};
  m1[0][0] = 5;
  m2[0][0] = 0.2;
  Matrix result = m1.InverseMatrix();
  EXPECT_EQ(result.get_cols(), m2.get_rows());
  EXPECT_EQ(result.get_rows(), m2.get_cols());
  EXPECT_EQ(result.EqMatrix(m2), true);
}
TEST(TestInverseMatrix, TestInverse_7) {
  Matrix m1{1, 3};
  init_matrix(m1, 1, 1);
  EXPECT_ANY_THROW(m1.InverseMatrix());
}
TEST(TestInverseMatrix, TestInverse_8) {
  Matrix m1{3, 3};
  Matrix m2{3, 3};
  m1[0][0] = 0;
  m1[0][1] = 7;
  m1[0][2] = 2;
  m1[1][0] = 5;
  m1[1][1] = 1;
  m1[1][2] = 0;
  m1[2][0] = 4;
  m1[2][1] = 4;
  m1[2][2] = 3;

  m2[0][0] = -0.04109589;
  m2[0][1] = 0.17808219;
  m2[0][2] = 0.02739726;
  m2[1][0] = 0.20547945;
  m2[1][1] = 0.10958904;
  m2[1][2] = -0.1369863;
  m2[2][0] = -0.21917808;
  m2[2][1] = -0.38356164;
  m2[2][2] = 0.479452054;
  Matrix result = m1.InverseMatrix();
  EXPECT_EQ(result.get_cols(), m2.get_rows());
  EXPECT_EQ(result.get_rows(), m2.get_cols());
  EXPECT_EQ(result.EqMatrix(m2), true);
}
TEST(TestInverseMatrix, TestInverse_9) {
  Matrix m1{3, 3};
  init_matrix(m1, 1, 1);
  EXPECT_ANY_THROW(m1.InverseMatrix());
}
TEST(TestInverseMatrix, TestInverse_10) {
  Matrix m1;
  EXPECT_ANY_THROW(m1.InverseMatrix());
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты Operator+
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestOperatorPlus, TestOpPlus_1) {
  Matrix m{3, 3};
  init_matrix(m, 0, 1);
  Matrix m2{3, 3};
  init_matrix(m2, 1, 0);
  Matrix m3(m + m2);
  Matrix result{3, 3};
  init_matrix(result, 1, 1);
  EXPECT_EQ(result.EqMatrix(m3), true);
}
TEST(TestOperatorPlus, TestOpPlus_2) {
  Matrix m;
  Matrix m2{3, 3};
  init_matrix(m2, 1, 0);
  EXPECT_ANY_THROW(m + m2);
}
TEST(TestOperatorPlus, TestOpPlus_3) {
  Matrix m{2, 2};
  init_matrix(m, 0, 5);
  Matrix m2{2, 2};
  init_matrix(m2, 0, 5);
  Matrix m3(m + m2);
  Matrix result{2, 2};
  init_matrix(result, 0, 10);
  EXPECT_EQ(result.EqMatrix(m3), true);
}
TEST(TestOperatorPlus, TestOpPlus_4) {
  Matrix m{3, 3};
  init_matrix(m, 1, 0);
  Matrix m2;
  EXPECT_ANY_THROW(m + m2);
}
TEST(TestOperatorPlus, TestOpPlus_5) {
  Matrix m{3, 3};
  init_matrix(m, 1, 0);
  Matrix m2{4, 4};
  init_matrix(m2, 1, 1);
  EXPECT_ANY_THROW(m + m2);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты Operator+=
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestOperatorPlusEq, TestOpPlusEq_1) {
  Matrix m{3, 3};
  init_matrix(m, 0, 1);
  Matrix m2{3, 3};
  init_matrix(m2, 1, 0);
  m += m2;
  Matrix result{3, 3};
  init_matrix(result, 1, 1);
  EXPECT_EQ(result.EqMatrix(m), true);
}
TEST(TestOperatorPlusEq, TestOpPlusEq_2) {
  Matrix m;
  Matrix m2{3, 3};
  init_matrix(m2, 1, 0);
  EXPECT_ANY_THROW(m += m2);
}
TEST(TestOperatorPlusEq, TestOpPlusEq_3) {
  Matrix m{3, 3};
  init_matrix(m, 1, 0);
  Matrix m2;
  EXPECT_ANY_THROW(m += m2);
}
TEST(TestOperatorPlusEq, TestOpPlusEq_4) {
  Matrix m{2, 2};
  init_matrix(m, 0, 5);
  Matrix m2{2, 2};
  init_matrix(m2, 0, 5);
  m += m2;
  Matrix result{2, 2};
  init_matrix(result, 0, 10);
  EXPECT_EQ(result.EqMatrix(m), true);
}
TEST(TestOperatorPlusEq, TestOpPlusEq_5) {
  Matrix m{3, 3};
  init_matrix(m, 1, 0);
  Matrix m2{4, 4};
  init_matrix(m2, 1, 1);
  EXPECT_ANY_THROW(m + m2);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты Operator-
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestOperatorMinus, TestOpMinus_1) {
  Matrix m{3, 3};
  init_matrix(m, 0, 1);
  Matrix m2{3, 3};
  init_matrix(m2, 1, 0);
  Matrix m3(m - m2);
  Matrix result{3, 3};
  init_matrix(result, -1, 1);
  EXPECT_EQ(result.EqMatrix(m3), true);
}
TEST(TestOperatorMinus, TestOpMinus_2) {
  Matrix m;
  Matrix m2{3, 3};
  init_matrix(m2, 1, 0);
  EXPECT_ANY_THROW(m - m2);
}
TEST(TestOperatorMinus, TestOpMinus_3) {
  Matrix m{2, 2};
  init_matrix(m, 0, 5);
  Matrix m2{2, 2};
  init_matrix(m2, 0, 5);
  Matrix m3{2, 2};
  m3 = (m - m2);
  Matrix result{2, 2};
  init_matrix(result, 0, 0);
  EXPECT_EQ(result.EqMatrix(m3), true);
}
TEST(TestOperatorMinus, TestOpMinus_4) {
  Matrix m{3, 3};
  init_matrix(m, 1, 0);
  Matrix m2;
  EXPECT_ANY_THROW(m - m2);
}
TEST(TestOperatorMinus, TestOpMinus_5) {
  Matrix m{3, 3};
  init_matrix(m, 1, 0);
  Matrix m2{4, 4};
  init_matrix(m2, 1, 1);
  EXPECT_ANY_THROW(m - m2);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты Operator-=
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestOperatorMinusEq, TestOpMinusEq_1) {
  Matrix m{3, 3};
  init_matrix(m, 0, 1);
  Matrix m2{3, 3};
  init_matrix(m2, 1, 0);
  m -= m2;
  Matrix result{3, 3};
  init_matrix(result, -1, 1);
  EXPECT_EQ(result.EqMatrix(m), true);
}
TEST(TestOperatorMinusEq, TestOpMinusEq_2) {
  Matrix m;
  Matrix m2{3, 3};
  init_matrix(m2, 1, 0);
  EXPECT_ANY_THROW(m -= m2);
}
TEST(TestOperatorMinusEq, TestOpMinusEq_3) {
  Matrix m1{2, 2};
  init_matrix(m1, 0, 5);
  Matrix m2{2, 2};
  init_matrix(m2, 0, 5);
  m1 -= m2;
  Matrix result{2, 2};
  init_matrix(result, 0, 0);
  EXPECT_EQ(result.EqMatrix(m1), true);
}
TEST(TestOperatorMinusEq, TestOpMinusEq_4) {
  Matrix m{3, 3};
  init_matrix(m, 1, 0);
  Matrix m2;
  EXPECT_ANY_THROW(m -= m2);
}
TEST(TestOperatorMinusEq, TestOpMinusEq_5) {
  Matrix m{3, 3};
  init_matrix(m, 1, 0);
  Matrix m2{4, 4};
  init_matrix(m2, 1, 1);
  EXPECT_ANY_THROW(m - m2);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты Operator*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestOperatorMult, TestOpMult_1) {
  Matrix m{3, 3};
  init_matrix(m, 0, 1);
  Matrix m2{3, 3};
  init_matrix(m2, 1, 0);
  Matrix m3(m * m2);
  Matrix result{3, 3};
  result[0][0] = 6;
  result[0][1] = 6;
  result[0][2] = 6;
  result[1][0] = 15;
  result[1][1] = 15;
  result[1][2] = 15;
  result[2][0] = 24;
  result[2][1] = 24;
  result[2][2] = 24;
  EXPECT_EQ(result.EqMatrix(m3), true);
}
TEST(TestOperatorMult, TestOpMult_2) {
  Matrix m;
  Matrix m2{3, 3};
  init_matrix(m2, 1, 0);
  EXPECT_ANY_THROW(m * m2);
}
TEST(TestOperatorMult, TestOpMult_3) {
  Matrix m{2, 2};
  init_matrix(m, 0, 5);
  Matrix m2{2, 2};
  init_matrix(m2, 0, 5);
  Matrix m3{2, 2};
  m3 = (m * m2);
  Matrix result{2, 2};
  result[0][0] = 175;
  result[0][1] = 250;
  result[1][0] = 375;
  result[1][1] = 550;
  EXPECT_EQ(result.EqMatrix(m3), true);
}
TEST(TestOperatorMult, TestOpMult_4) {
  Matrix m{3, 3};
  init_matrix(m, 1, 0);
  Matrix m2;
  EXPECT_ANY_THROW(m * m2);
}
TEST(TestOperatorMult, TestOpMult_5) {
  Matrix m{3, 3};
  init_matrix(m, 1, 0);
  Matrix m2{4, 4};
  init_matrix(m2, 1, 1);
  EXPECT_ANY_THROW(m * m2);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты Operator*=
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestOperatorMultEq, TestOpMultEq_1) {
  Matrix m{3, 3};
  init_matrix(m, 0, 1);
  Matrix m2{3, 3};
  init_matrix(m2, 1, 0);
  m *= m2;
  Matrix result{3, 3};
  result[0][0] = 6;
  result[0][1] = 6;
  result[0][2] = 6;
  result[1][0] = 15;
  result[1][1] = 15;
  result[1][2] = 15;
  result[2][0] = 24;
  result[2][1] = 24;
  result[2][2] = 24;
  EXPECT_EQ(result.EqMatrix(m), true);
}
TEST(TestOperatorMultEq, TestOpMultEq_2) {
  Matrix m;
  Matrix m2{3, 3};
  init_matrix(m2, 1, 0);
  EXPECT_ANY_THROW(m *= m2);
}
TEST(TestOperatorMultEq, TestOpMultEq_3) {
  Matrix m{2, 2};
  init_matrix(m, 0, 5);
  Matrix m2{2, 2};
  init_matrix(m2, 0, 5);
  m *= m2;
  Matrix result{2, 2};
  result[0][0] = 175;
  result[0][1] = 250;
  result[1][0] = 375;
  result[1][1] = 550;
  EXPECT_EQ(result.EqMatrix(m), true);
}
TEST(TestOperatorMultEq, TestOpMultEq_4) {
  Matrix m{3, 3};
  init_matrix(m, 1, 0);
  Matrix m2;
  EXPECT_ANY_THROW(m *= m2);
}
TEST(TestOperatorMultEq, TestOpMultEq_5) {
  Matrix m{3, 3};
  init_matrix(m, 1, 0);
  Matrix m2{4, 4};
  init_matrix(m2, 1, 1);
  EXPECT_ANY_THROW(m * m2);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты Operator=
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestOperatorAssignment, TestOpAssign_1) {
  Matrix m{3, 3};
  init_matrix(m, 0, 1);
  Matrix m2{3, 3};
  m = m2;
  EXPECT_EQ(m.EqMatrix(m2), true);
}
TEST(TestOperatorAssignment, TestOpAssign_2) {
  Matrix m;
  Matrix m2{3, 3};
  init_matrix(m2, 1, 0);
  EXPECT_ANY_THROW(m *= m2);
}
TEST(TestOperatorAssignment, TestOpAssign_3) {
  Matrix m{3, 3};
  init_matrix(m, 1, 0);
  Matrix m2;
  EXPECT_ANY_THROW(m *= m2);
}
TEST(TestOperatorAssignment, TestOpAssign_4) {
  Matrix m{2, 2};
  init_matrix(m, DOUBLE_MAX, 0);
  Matrix m2{2, 2};
  m = m2;
  EXPECT_EQ(m.EqMatrix(m2), true);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты Operator==
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestOperatorDoubleEqual, TestOpDobEq_1) {
  Matrix m{3, 3};
  init_matrix(m, 0, 1);
  Matrix m2{3, 3};
  init_matrix(m2, 0, 1);
  EXPECT_EQ(m == m2, true);
}
TEST(TestOperatorDoubleEqual, TestOpDobEq_2) {
  Matrix m;
  Matrix m2{3, 3};
  init_matrix(m2, 1, 0);
  EXPECT_ANY_THROW(m == m2);
}
TEST(TestOperatorDoubleEqual, TestOpDobEq_3) {
  Matrix m{3, 3};
  init_matrix(m, 1, 0);
  Matrix m2;
  EXPECT_ANY_THROW(m == m2);
}
TEST(TestOperatorDoubleEqual, TestOpDobEq_4) {
  Matrix m{2, 2};
  init_matrix(m, DOUBLE_MAX, 0);
  Matrix m2{2, 2};
  init_matrix(m2, DOUBLE_MAX, 0);
  EXPECT_EQ(m == m2, true);
}
TEST(TestOperatorDoubleEqual, TestOpDobEq_5) {
  Matrix m{2, 2};
  init_matrix(m, DOUBLE_MAX, 1);
  Matrix m2{2, 2};
  init_matrix(m2, DOUBLE_MAX, 1);
  EXPECT_EQ(m == m2, true);
}
TEST(TestOperatorDoubleEqual, TestOpDobEq_6) {
  Matrix m{2, 2};
  init_matrix(m, DOUBLE_MAX, 0);
  Matrix m2{3, 2};
  init_matrix(m2, DOUBLE_MAX, 0);
  EXPECT_ANY_THROW(m == m2);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты Operator*(const double num)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestOperatorMultNum, TestOpMultNum_1) {
  Matrix m{3, 3};
  init_matrix(m, 0, 1);
  double num = 5;
  Matrix m2 = m.operator*(num);
  Matrix result{3, 3};
  result[0][0] = 5;
  result[0][1] = 10;
  result[0][2] = 15;
  result[1][0] = 20;
  result[1][1] = 25;
  result[1][2] = 30;
  result[2][0] = 35;
  result[2][1] = 40;
  result[2][2] = 45;
  EXPECT_EQ(result.EqMatrix(m2), true);
}
TEST(TestOperatorMultNum, TestOpMultNum_2) {
  Matrix m;
  double num = 5;
  EXPECT_ANY_THROW(m.operator*(num));
}
TEST(TestOperatorMultNum, TestOpMultNum_3) {
  Matrix m{2, 2};
  init_matrix(m, 0, 5);
  double num = -1;
  m = m.operator*(num);
  Matrix result{2, 2};
  result[0][0] = -5;
  result[0][1] = -10;
  result[1][0] = -15;
  result[1][1] = -20;
  EXPECT_EQ(result.EqMatrix(m), true);
}
TEST(TestOperatorMultNum, TestOpMultNum_4) {
  Matrix m{3, 3};
  init_matrix(m, 1, 0);
  double num = DOUBLE_MAX;
  EXPECT_ANY_THROW(m.operator*(num));
}
TEST(TestOperatorMultNum, TestOpMultNum_5) {
  Matrix m{3, 3};
  init_matrix(m, 1, 0);
  double num = INFINITY;
  EXPECT_ANY_THROW(m.operator*(num));
}
TEST(TestOperatorMultNum, TestOpMultNum_6) {
  Matrix m{2, 2};
  init_matrix(m, 0, 5);
  double num = -1;
  m = num * m;
  Matrix result{2, 2};
  result[0][0] = -5;
  result[0][1] = -10;
  result[1][0] = -15;
  result[1][1] = -20;
  EXPECT_EQ(result.EqMatrix(m), true);
}
TEST(TestOperatorMultNum, TestOpMultNum_7) {
  Matrix m{2, 2};
  init_matrix(m, 0, 5);
  double num = -1;
  m = operator*(m, num);
  Matrix result{2, 2};
  result[0][0] = -5;
  result[0][1] = -10;
  result[1][0] = -15;
  result[1][1] = -20;
  EXPECT_EQ(result.EqMatrix(m), true);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты Operator*=(const double num)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestOperatorMultNumEq, TestOpMultNumEq_1) {
  Matrix m{3, 3};
  init_matrix(m, 0, 1);
  double num = 5;
  m *= num;
  Matrix result{3, 3};
  result[0][0] = 5;
  result[0][1] = 10;
  result[0][2] = 15;
  result[1][0] = 20;
  result[1][1] = 25;
  result[1][2] = 30;
  result[2][0] = 35;
  result[2][1] = 40;
  result[2][2] = 45;
  EXPECT_EQ(result.EqMatrix(m), true);
}
TEST(TestOperatorMultNumEq, TestOpMultNumEq_2) {
  Matrix m;
  double num = 5;
  EXPECT_ANY_THROW(m.operator*=(num));
}
TEST(TestOperatorMultNumEq, TestOpMultNumEq_3) {
  Matrix m{2, 2};
  init_matrix(m, 0, 5);
  double num = -1;
  m *= num;
  Matrix result{2, 2};
  result[0][0] = -5;
  result[0][1] = -10;
  result[1][0] = -15;
  result[1][1] = -20;
  EXPECT_EQ(result.EqMatrix(m), true);
}
TEST(TestOperatorMultNumEq, TestOpMultNumEq_4) {
  Matrix m{3, 3};
  init_matrix(m, 1, 0);
  double num = INFINITY;
  EXPECT_ANY_THROW(m.operator*=(num));
}
TEST(TestOperatorMultNumEq, TestOpMultNumEq_5) {
  Matrix m{3, 3};
  init_matrix(m, 1, 0);
  double num = DOUBLE_MAX;
  EXPECT_ANY_THROW(m.operator*=(num));
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты Operator(), []
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestOperatorBrackets, TestOpBrackets_1) {
  Matrix m(2, 2);
  init_matrix(m, 0, 5);
  Matrix result{2, 2};
  result[0][0] = 5;
  result[0][1] = 10;
  result[1][0] = 15;
  result[1][1] = 20;
  EXPECT_EQ(result.EqMatrix(m), true);
}
TEST(TestOperatorBrackets, TestOpBrackets_2) {
  Matrix m(3, 3);
  init_matrix(m, 0, 0);
  Matrix result{3, 3};
  result[0][0] = 0;
  result[0][1] = 0;
  result[1][0] = 0;
  result[1][1] = 0;
  EXPECT_EQ(result.EqMatrix(m), true);
}
TEST(TestOperatorBrackets, TestOpBrackets_3) {
  Matrix m(3, 3);
  init_matrix(m, 0, 0);
  m[1][1] = 5;
  Matrix result{3, 3};
  result[0][0] = 0;
  result[0][1] = 0;
  result[1][0] = 0;
  result[1][1] = 5;
  EXPECT_EQ(result.EqMatrix(m), true);
}
TEST(TestOperatorBrackets, TestOpBrackets_4) {
  Matrix m(3, 3);
  init_matrix(m, 0, 0);
  EXPECT_THROW(m[10][1] = 5, std::invalid_argument);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Тесты исключений разных функций
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TEST(TestMatrixDeterminantEx, TestDeterminantEx_1) {
  Matrix m{2, 2};
  m[0][0] = INFINITY;
  m[0][1] = 2;
  m[1][0] = 0;
  m[1][1] = 4;
  EXPECT_THROW(m.Determinant(), std::out_of_range);
}
TEST(TestMatrixDeterminantEx, TestDeterminantEx_2) {
  Matrix m{2, 2};
  m[0][0] = -INFINITY;
  m[0][1] = 1;
  m[1][0] = 0;
  m[1][1] = 4;
  EXPECT_ANY_THROW(m.Determinant());
}
TEST(TestOperatorBracketsEx, TestOpBracketsEx_1) {
  EXPECT_ANY_THROW(Matrix m(-3, 3));
}
TEST(TestOperatorBracketsEx, TestOpBracketsEx_2) {
  EXPECT_ANY_THROW(Matrix m(3, -3));
}
TEST(TestOperatorBracketsEx, TestOpBracketsEx_3) {
  Matrix m{3, 3};
  EXPECT_ANY_THROW(m[-2][0] = 1);
}

TEST(TestOperatorBracketsEx, TestOpBracketsEx_4) {
  Matrix m{3, 3};
  EXPECT_ANY_THROW(m(10, 3));
}
TEST(TestOperatorBracketsEx, TestOpBracketsEx_5) {
  Matrix m{3, 3};
  EXPECT_ANY_THROW(m(-1, 3));
}
TEST(TestOperatorBracketsEx, TestOpBracketsEx_6) {
  Matrix m{3, 3};
  EXPECT_ANY_THROW(m(3, -1));
}
TEST(TestOperatorBracketsEx, TestOpBracketsEx_7) {
  Matrix m{3, 3};
  EXPECT_ANY_THROW(m(3, 10));
}
TEST(TestOperatorBracketsEx, TestOpBracketsEx_8) {
  Matrix m{3, 3};
  EXPECT_NO_THROW(m(2, 2));
}
TEST(TestOperatorBracketsEx, TestOpBracketsEx_9) {
  Matrix m{3, 3};
  EXPECT_ANY_THROW(m(-3, -3));
}
TEST(TestOperatorBracketsEx, TestOpBracketsEx_10) {
  Matrix m{3, 3};
  EXPECT_ANY_THROW(m[3][-1] = 1);
}
TEST(TestOperatorBracketsEx, TestOpBracketsEx_11) {
  Matrix m{3, 3};
  EXPECT_ANY_THROW(m[-3][-3] = 1);
}
TEST(TestOperatorBracketsConst, TestOpBracketsConst_1) {
  const Matrix m{1, 1};
  m[0][0] = 5;
  EXPECT_EQ(m[0][0], 5);
}
TEST(TestOperatorBracketsEx, TestOpBracketsEx_12) {
  const Matrix m{1, 1};
  EXPECT_ANY_THROW(m[2][1] = 5);
}
TEST(TestOperatorBracketsEx, TestOpBracketsEx_13) {
  const Matrix m{1, 1};
  EXPECT_ANY_THROW(m[1][3] = 5);
}
TEST(TestOperatorBracketsEx, TestOpBracketsEx_14) {
  const Matrix m{1, 1};
  EXPECT_ANY_THROW(m[-1][1] = 5);
}
TEST(TestOperatorBracketsEx, TestOpBracketsEx_15) {
  const Matrix m{1, 1};
  EXPECT_ANY_THROW(m[1][-1] = 5);
}
TEST(TestOperatorBracketsEx, TestOpBracketsEx_16) {
  const Matrix m{3, 3};
  EXPECT_ANY_THROW(m(10, 3));
}
TEST(TestOperatorBracketsEx, TestOpBracketsEx_17) {
  const Matrix m{3, 3};
  EXPECT_ANY_THROW(m(-1, 3));
}
TEST(TestOperatorBracketsEx, TestOpBracketsEx_18) {
  const Matrix m{3, 3};
  EXPECT_ANY_THROW(m(3, -1));
}
TEST(TestOperatorBracketsEx, TestOpBracketsEx_19) {
  const Matrix m{3, 3};
  EXPECT_ANY_THROW(m(3, 10));
}
TEST(TestOperatorBracketsEx, TestOpBracketsEx_20) {
  const Matrix m{3, 3};
  EXPECT_NO_THROW(m(2, 2));
}
TEST(TestOperatorBracketsEx, TestOpBracketsEx_21) {
  const Matrix m{3, 3};
  EXPECT_ANY_THROW(m(-3, -3));
}
TEST(TestSetColEx, TestSetColEx_1) {
  Matrix m{3, 3};
  EXPECT_ANY_THROW(m.set_cols(-5));
}
TEST(TestSetRowEx, TestSetRowEx_1) {
  Matrix m{3, 3};
  EXPECT_ANY_THROW(m.set_rows(-5));
}