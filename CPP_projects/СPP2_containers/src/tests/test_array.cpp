#include "tests_run.hpp"

TEST(Suite_array_constructor, default_constructor_1) {
  S21_array<int, 0> my_array;
  EXPECT_EQ(my_array.size(), 0U);
}
TEST(Suite_array_constructor, default_constructor_2) {
  S21_array<int, 3> my_array;
  EXPECT_EQ(my_array.max_size(), 3U);
  EXPECT_EQ(*my_array.data(), 0);
}
TEST(Suite_array_constructor, default_constructor_3) {
  S21_array<double, 5> my_array;
  EXPECT_EQ(my_array.size(), 5U);
  EXPECT_EQ(*my_array.data(), 0);
}
TEST(Suite_array_constructor, default_constructor_4) {
  S21_array<char, 2> my_array;
  EXPECT_EQ(my_array.size(), 2U);
  EXPECT_EQ(*my_array.data(), 0);
}
TEST(Suite_array_constructor, default_constructor_5) {
  S21_array<int, 5> my_array;
  EXPECT_EQ(my_array.size(), 5U);
  EXPECT_EQ(*my_array.data(), 0);
}
TEST(Suite_array_constructor, default_constructor_6) {
  S21_array<int, 1000> my_array;
  EXPECT_EQ(my_array.size(), 1000U);
  EXPECT_EQ(*my_array.data(), 0);
}
TEST(Suite_array_constructor, initializer_list_constructor_1) {
  S21_array<int, 3> my_array = {1, 2, 3};
  EXPECT_EQ(my_array.size(), 3U);
  EXPECT_EQ(my_array[0], 1);
  EXPECT_EQ(my_array[1], 2);
  EXPECT_EQ(my_array[2], 3);
}
TEST(Suite_array_constructor, initializer_list_constructor_2) {
  S21_array<double, 3> my_array = {-1.123, 2.234, 3.456};
  EXPECT_EQ(my_array.size(), 3U);
  EXPECT_EQ(my_array[0], -1.123);
  EXPECT_EQ(my_array[1], 2.234);
  EXPECT_EQ(my_array[2], 3.456);
}
TEST(Suite_array_constructor, initializer_list_constructor_3) {
  S21_array<char, 2> my_array = {'q', 'a'};
  EXPECT_EQ(my_array.size(), 2U);
  EXPECT_EQ(my_array[0], 'q');
  EXPECT_EQ(my_array[1], 'a');
}
TEST(Suite_array_constructor, copy_constructor_1) {
  S21_array<int, 3> my_array = {1, 2, 3};
  S21_array<int, 3> copy_array(my_array);
  EXPECT_EQ(my_array[0], 1);
  EXPECT_EQ(my_array[1], 2);
  EXPECT_EQ(my_array[2], 3);
  EXPECT_EQ(my_array.size(), 3U);
  EXPECT_EQ(copy_array.size(), 3U);
  EXPECT_EQ(copy_array[0], 1);
  EXPECT_EQ(copy_array[1], 2);
  EXPECT_EQ(copy_array[2], 3);
}
TEST(Suite_array_constructor, copy_constructor_2) {
  S21_array<double, 3> my_array = {DBL_MAX, DBL_MAX, DBL_MAX};
  S21_array<double, 3> copy_array(my_array);
  EXPECT_EQ(my_array[0], DBL_MAX);
  EXPECT_EQ(my_array[1], DBL_MAX);
  EXPECT_EQ(my_array[2], DBL_MAX);
  EXPECT_EQ(my_array.size(), 3U);
  EXPECT_EQ(copy_array.size(), 3U);
  EXPECT_EQ(copy_array[0], DBL_MAX);
  EXPECT_EQ(copy_array[1], DBL_MAX);
  EXPECT_EQ(copy_array[2], DBL_MAX);
}
TEST(Suite_array_constructor, move_constructor_1) {
  S21_array<int, 3> my_array = {1, 2, 3};
  S21_array<int, 3> move_array(std::move(my_array));
  EXPECT_EQ(my_array.size(), 0U);
  EXPECT_EQ(move_array.size(), 3U);
  EXPECT_EQ(move_array[0], 1);
  EXPECT_EQ(move_array[1], 2);
  EXPECT_EQ(move_array[2], 3);
}
TEST(Suite_array_constructor, move_constructor_2) {
  S21_array<double, 3> my_array = {DBL_MAX, DBL_MAX, DBL_MAX};
  S21_array<double, 3> move_array(std::move(my_array));
  EXPECT_EQ(my_array.size(), 0U);
  EXPECT_EQ(move_array.size(), 3U);
  EXPECT_EQ(move_array[0], DBL_MAX);
  EXPECT_EQ(move_array[1], DBL_MAX);
  EXPECT_EQ(move_array[2], DBL_MAX);
}
TEST(Suite_array_constructor, operator_eq_test_1) {
  S21_array<int, 3> my_array = {1, 2, 3};
  S21_array<int, 3> move_array;
  move_array = std::move(my_array);
  EXPECT_EQ(my_array.size(), 0U);
  EXPECT_EQ(move_array.size(), 3U);
  EXPECT_EQ(move_array[0], 1);
  EXPECT_EQ(move_array[1], 2);
  EXPECT_EQ(move_array[2], 3);
}
TEST(Suite_array_constructor, operator_eq_test_2) {
  S21_array<double, 3> my_array = {DBL_MAX, DBL_MAX, DBL_MAX};
  S21_array<double, 3> move_array;
  move_array = std::move(my_array);
  EXPECT_EQ(my_array.empty(), 1);
  EXPECT_EQ(move_array.size(), 3U);
  EXPECT_EQ(move_array[0], DBL_MAX);
  EXPECT_EQ(move_array[1], DBL_MAX);
  EXPECT_EQ(move_array[2], DBL_MAX);
}
TEST(Suite_array_access, at_test_1) {
  S21_array<int, 3> my_array = {1, 2, 3};
  EXPECT_EQ(my_array.size(), 3U);
  EXPECT_EQ(my_array.at(2), 3);
}
TEST(Suite_array_access, at_test_2) {
  S21_array<double, 3> my_array = {DBL_MAX, DBL_MAX, DBL_MAX};
  EXPECT_EQ(my_array.size(), 3U);
  EXPECT_THROW(my_array.at(7), std::out_of_range);
}
TEST(Suite_array_access, front_test_1) {
  S21_array<double, 3> my_array = {-1.0, 2.0, 3.0};
  EXPECT_EQ(my_array.size(), 3U);
  EXPECT_EQ(my_array.front(), -1.0);
}
TEST(Suite_array_access, front_test_2) {
  S21_array<int, 3> my_array;
  EXPECT_EQ(my_array.size(), 3U);
  EXPECT_EQ(my_array.front(), 0);
}
TEST(Suite_array_access, front_test_3) {
  S21_array<char, 5> my_array = {'q', 'w', 'e', 'r', 't'};
  EXPECT_EQ(my_array.size(), 5U);
  EXPECT_EQ(my_array.front(), 'q');
}
TEST(Suite_array_access, back_test_1) {
  S21_array<double, 3> my_array = {-1.0, 2.0, 3.0};
  EXPECT_EQ(my_array.size(), 3U);
  EXPECT_EQ(my_array.back(), 3.0);
}
TEST(Suite_array_access, back_test_2) {
  S21_array<int, 3> my_array;
  EXPECT_EQ(my_array.size(), 3U);
  EXPECT_EQ(my_array.back(), 0);
}
TEST(Suite_array_access, back_test_3) {
  S21_array<char, 6> my_array = {'q', 'w', 'e', 'r', 't', 'y'};
  EXPECT_EQ(my_array.size(), 6U);
  EXPECT_EQ(my_array.back(), 'y');
}
TEST(Suite_array_access, data_test_1) {
  S21_array<double, 3> my_array = {-1.0, 2.0, 3.0};
  EXPECT_EQ(my_array.size(), 3U);
  double* temp = my_array.data();
  EXPECT_EQ(temp[1], 2.0);
}
TEST(Suite_array_access, data_test_2) {
  S21_array<int, 3> my_array;
  int* temp = my_array.data();
  EXPECT_EQ(my_array.size(), 3U);
  EXPECT_EQ(temp[2], 0);
}
TEST(Suite_array_access, data_test_3) {
  S21_array<char, 6> my_array = {'q', 'w', 'e', 'r', 't', 'y'};
  char* temp = my_array.data();
  EXPECT_EQ(my_array.size(), 6U);
  EXPECT_EQ(temp[0], 'q');
}
TEST(Suite_array_access, data_test_4) {
  S21_array<int, 3> my_array = {1, 2, 3};
  EXPECT_THROW(my_array[5], std::out_of_range);
}
TEST(Suite_array_iterators, begin_test_1) {
  S21_array<double, 3> my_array = {-1.0, 2.0, 3.0};
  EXPECT_EQ(my_array.size(), 3U);
  EXPECT_EQ(*my_array.begin(), -1.0);
}
TEST(Suite_array_iterators, begin_test_2) {
  S21_array<int, 3> my_array;
  EXPECT_EQ(my_array.size(), 3U);
  EXPECT_EQ(*my_array.begin(), 0);
}
TEST(Suite_array_iterators, begin_test_3) {
  S21_array<char, 6> my_array = {'q', 'w', 'e', 'r', 't', 'y'};
  EXPECT_EQ(my_array.size(), 6U);
  EXPECT_EQ(*my_array.begin(), 'q');
}
TEST(Suite_array_iterators, begin_test_4) {
  S21_array<double, 3> my_array = {-1.0, 2.0, 3.0};
  double* iter = my_array.begin();
  ++iter;
  ++iter;
  EXPECT_EQ(my_array.size(), 3U);
  EXPECT_EQ(*iter, 3.0);
}

TEST(Suite_array_iterators, end_test_1) {
  S21_array<double, 3> my_array = {-1.0, 2.0, 3.0};
  EXPECT_EQ(my_array.size(), 3U);
  EXPECT_EQ(*(my_array.end() - 1), 3.0);
}
TEST(Suite_array_iterators, end_test_2) {
  S21_array<int, 3> my_array;
  EXPECT_EQ(my_array.size(), 3U);
  EXPECT_EQ(*(my_array.end() - 1), 0);
}
TEST(Suite_array_iterators, end_test_3) {
  S21_array<char, 6> my_array = {'q', 'w', 'e', 'r', 't', 'y'};
  EXPECT_EQ(my_array.size(), 6U);
  S21_array<char, 6>::iterator iter = my_array.end();
  EXPECT_EQ(*(iter - 1), 'y');
}
TEST(Suite_array_iterators, oper_plus_test_1) {
  S21_array<int, 5> my_array = {1, 2, 3, 4, 5};
  EXPECT_EQ(my_array.size(), 5U);
  S21_array<int, 5>::iterator iter = my_array.begin();
  EXPECT_EQ(*(iter + 1), 2);
}
TEST(Suite_array_iterators, oper_plus_eq_test_1) {
  S21_array<int, 5> my_array = {1, 2, 3, 4, 5};
  EXPECT_EQ(my_array.size(), 5U);
  S21_array<int, 5>::iterator iter = my_array.begin();
  iter += 4;
  EXPECT_EQ(*(iter), 5);
}
TEST(Suite_array_iterators, oper_increment_test_1) {
  S21_array<int, 5> my_array = {1, 2, 3, 4, 5};
  EXPECT_EQ(my_array.size(), 5U);
  S21_array<int, 5>::iterator iter = my_array.begin();
  iter++;
  EXPECT_EQ(*(iter), 2);
}
TEST(Suite_array_iterators, oper_increment_test_2) {
  S21_array<int, 5> my_array = {1, 2, 3, 4, 5};
  EXPECT_EQ(my_array.size(), 5U);
  S21_array<int, 5>::iterator iter = my_array.begin();
  ++iter;
  EXPECT_EQ(*(iter), 2);
}
TEST(Suite_array_iterators, oper_minus_test_1) {
  S21_array<int, 5> my_array = {1, 2, 3, 4, 5};
  EXPECT_EQ(my_array.size(), 5U);
  S21_array<int, 5>::iterator iter = my_array.end();
  EXPECT_EQ(*(iter - 4), 2);
}
TEST(Suite_array_iterators, oper_minus_eq_test_1) {
  S21_array<int, 5> my_array = {1, 2, 3, 4, 5};
  EXPECT_EQ(my_array.size(), 5U);
  S21_array<int, 5>::iterator iter = my_array.end();
  iter -= 4;
  EXPECT_EQ(*(iter), 2);
}
TEST(Suite_array_iterators, oper_decrement_1) {
  S21_array<int, 5> my_array = {1, 2, 3, 4, 5};
  EXPECT_EQ(my_array.size(), 5U);
  S21_array<int, 5>::iterator iter = my_array.end();
  iter--;
  EXPECT_EQ(*(iter), 5);
}
TEST(Suite_array_iterators, oper_decrement_2) {
  S21_array<int, 5> my_array = {1, 2, 3, 4, 5};
  EXPECT_EQ(my_array.size(), 5U);
  S21_array<int, 5>::iterator iter = my_array.end();
  --iter;
  EXPECT_EQ(*(iter), 5);
}
TEST(Suite_array_iterators, iterators_eq) {
  S21_array<int, 3> my_array = {1, 2, 3};
  S21_array<int, 3>::iterator it = my_array.begin();
  S21_array<int, 3>::iterator it_2 = my_array.begin();
  EXPECT_EQ(it == it_2, true);
}

TEST(Suite_array_iterators, iterators_n_eq) {
  S21_array<int, 3> my_array = {1, 2, 3};
  S21_array<int, 3>::iterator it = my_array.begin();
  S21_array<int, 3>::iterator it_2 = my_array.begin();
  it_2++;
  EXPECT_EQ(it != it_2, true);
}
TEST(Suite_array_iterators, end_test_5) {
  S21_array<int, 5> my_array = {1, 2, 3, 4, 5};
  EXPECT_EQ(my_array.size(), 5U);
  S21_array<int, 5>::iterator iter = my_array.end();
  --iter;
  EXPECT_EQ(*(iter), 5);
}
TEST(Suite_array_capacity, empty_test_1) {
  S21_array<double, 3> my_array = {-1.0, 2.0, 3.0};
  S21_array<double, 3> temp_array;
  temp_array = std::move(my_array);
  EXPECT_EQ(my_array.size(), 0U);
  EXPECT_EQ(my_array.empty(), 1);
}
TEST(Suite_array_capacity, empty_test_2) {
  S21_array<int, 0> my_array;
  EXPECT_EQ(my_array.size(), 0U);
  EXPECT_EQ(my_array.empty(), 1);
}
TEST(Suite_array_capacity, empty_test_3) {
  S21_array<int, 3> my_array;
  EXPECT_EQ(my_array.empty(), 0);
}
TEST(Suite_array_capacity, size_test_1) {
  S21_array<char, 6> my_array = {'q', 'w', 'e', 'r', 't', 'y'};
  EXPECT_EQ(my_array.size(), 6U);
}
TEST(Suite_array_capacity, size_test_2) {
  S21_array<int, 3> my_array;
  EXPECT_EQ(my_array.size(), 3U);
}
TEST(Suite_array_capacity, maxsize_test_1) {
  S21_array<char, 6> my_array = {'q', 'w', 'e', 'r', 't', 'y'};
  EXPECT_EQ(my_array.max_size(), 6U);
}
TEST(Suite_array_capacity, maxsize_test_2) {
  S21_array<int, 3> my_array;
  EXPECT_EQ(my_array.max_size(), 3U);
}
TEST(Suite_array_modifiers, swap_test_1) {
  S21_array<double, 3> array1 = {1.0, 2.0, 3.0};
  S21_array<double, 3> array2 = {4.0, 5.0, 6.0};
  array1.swap(array2);
  EXPECT_EQ(array1.front(), 4.0);
  EXPECT_EQ(array1.back(), 6.0);
  EXPECT_EQ(array2.front(), 1.0);
  EXPECT_EQ(array2.back(), 3.0);
}
TEST(Suite_array_modifiers, swap_test_2) {
  S21_array<int, 3> array1 = {10, 20, 30};
  S21_array<int, 3> array2 = {40, 50, 60};
  array1.swap(array2);
  EXPECT_EQ(array1.front(), 40);
  EXPECT_EQ(array1.back(), 60);
  EXPECT_EQ(array2.front(), 10);
  EXPECT_EQ(array2.back(), 30);
}
TEST(Suite_array_modifiers, fill_test_1) {
  S21_array<double, 3> my_array;
  my_array.fill(5);
  EXPECT_EQ(my_array.front(), 5.0);
  EXPECT_EQ(*my_array.data(), 5.0);
  EXPECT_EQ(my_array.back(), 5.0);
  EXPECT_EQ(my_array[1], 5.0);
}