#include "tests_run.hpp"

TEST(Suite_stack_constructor, basic_constructor) {
  S21_Stack<int> st;
  EXPECT_EQ(st.size(), 0U);
  EXPECT_EQ(st.empty(), true);
}

TEST(Suite_stack_constructor, basic_constructor_1) {
  S21_Stack<double> st;
  EXPECT_EQ(st.size(), 0U);
  EXPECT_EQ(st.empty(), true);
}

TEST(Suite_stack_constructor, basic_constructor_2) {
  S21_Stack<char> st;
  EXPECT_EQ(st.size(), 0U);
  EXPECT_EQ(st.empty(), true);
}

TEST(Suite_stack_constructor, init_constructor) {
  S21_Stack<int> st = {1, 2, 3, 4, 5};
  EXPECT_EQ(st.size(), 5U);
  EXPECT_EQ(st.top(), 5);
}

TEST(Suite_stack_constructor, init_constructor_1) {
  S21_Stack<int> st = {1};
  EXPECT_EQ(st.size(), 1U);
  EXPECT_EQ(st.top(), 1);
}

TEST(Suite_stack_constructor, init_constructor_2) {
  S21_Stack<int> st = {};
  EXPECT_EQ(st.size(), 0U);
}

TEST(Suite_stack_constructor, copy_constructor) {
  S21_Stack<int> st2 = {1, 2, 3, 4, 5};
  S21_Stack<int> st(st2);
  EXPECT_EQ(st.size(), 5U);
  EXPECT_EQ(st2.size(), 5U);
  EXPECT_EQ(st.top(), 5);
}

TEST(Suite_stack_constructor, copy_constructor_1) {
  S21_Stack<int> st2 = {1};
  S21_Stack<int> st(st2);
  EXPECT_EQ(st.size(), 1U);
  EXPECT_EQ(st2.size(), 1U);
  EXPECT_EQ(st.top(), 1);
}

TEST(Suite_stack_constructor, copy_constructor_2) {
  S21_Stack<int> st2;
  EXPECT_ANY_THROW(S21_Stack<int> st(st2););
}

TEST(Suite_stack_constructor, move_constructor) {
  S21_Stack<int> st2 = {1, 2, 3, 4, 5};
  S21_Stack<int> st(std::move(st2));
  EXPECT_EQ(st.size(), 5U);
  EXPECT_EQ(st2.size(), 0U);
  EXPECT_EQ(st.top(), 5);
}

TEST(Suite_stack_constructor, move_constructor_1) {
  S21_Stack<int> st2 = {1};
  S21_Stack<int> st(std::move(st2));
  EXPECT_EQ(st.size(), 1U);
  EXPECT_EQ(st2.size(), 0U);
  EXPECT_EQ(st.top(), 1);
}

TEST(Suite_stack_constructor, move_constructor_2) {
  S21_Stack<int> st2;
  EXPECT_ANY_THROW(S21_Stack<int> st(std::move(st2)););
}

TEST(Suite_stack_constructor, eq_operator) {
  S21_Stack<int> st2 = {1, 2, 3, 4, 5};
  S21_Stack<int> st = std::move(st2);
  EXPECT_EQ(st.size(), 5U);
  EXPECT_EQ(st2.size(), 0U);
  EXPECT_EQ(st.top(), 5);
}

TEST(Suite_stack_constructor, eq_operator_1) {
  S21_Stack<int> st2 = {1};
  S21_Stack<int> st = std::move(st2);
  EXPECT_EQ(st.size(), 1U);
  EXPECT_EQ(st2.size(), 0U);
  EXPECT_EQ(st.top(), 1);
}

TEST(Suite_stack_constructor, eq_operator_2) {
  S21_Stack<int> st2;
  EXPECT_ANY_THROW(S21_Stack<int> st = std::move(st2););
}

TEST(Suite_stack_constructor, push) {
  S21_Stack<int> st;
  st.push(1);
  EXPECT_EQ(st.size(), 1U);
  EXPECT_EQ(st.top(), 1);
}

TEST(Suite_stack_constructor, push_1) {
  S21_Stack<int> st;
  st.push(1);
  st.push(2);
  st.push(3);
  EXPECT_EQ(st.size(), 3U);
  EXPECT_EQ(st.top(), 3);
}

TEST(Suite_stack_constructor, push_2) {
  S21_Stack<int> st = {1, 2, 3};
  st.push(4);
  st.push(5);
  st.push(6);
  EXPECT_EQ(st.size(), 6U);
  EXPECT_EQ(st.top(), 6);
}

TEST(Suite_stack_constructor, pop) {
  S21_Stack<int> st;
  EXPECT_ANY_THROW(st.pop(););
}

TEST(Suite_stack_constructor, pop_1) {
  S21_Stack<int> st = {1};
  st.pop();
  EXPECT_EQ(st.size(), 0U);
}

TEST(Suite_stack_constructor, pop_2) {
  S21_Stack<int> st = {1, 2, 3, 4};
  st.pop();
  EXPECT_EQ(st.size(), 3U);
}

TEST(Suite_stack_constructor, swap) {
  S21_Stack<int> st = {1};
  S21_Stack<int> st2 = {2};
  st.swap(st2);
  EXPECT_EQ(st.size(), 1U);
  EXPECT_EQ(st2.size(), 1U);
}

TEST(Suite_stack_constructor, swap_1) {
  S21_Stack<int> st = {1, 2, 3};
  S21_Stack<int> st2 = {5, 6};
  st.swap(st2);
  EXPECT_EQ(st.size(), 2U);
  EXPECT_EQ(st2.size(), 3U);
}

TEST(Suite_stack_constructor, clear) {
  S21_Stack<int> st = {1};
  st.clear();
  EXPECT_EQ(st.size(), 0U);
}

TEST(Suite_stack_constructor, clear_1) {
  S21_Stack<int> st = {1, 2, 3};
  st.clear();
  EXPECT_EQ(st.size(), 0U);
}

TEST(Suite_stack_constructor, empty) {
  S21_Stack<int> st = {1};
  EXPECT_EQ(st.empty(), false);
}

TEST(Suite_stack_constructor, empty_1) {
  S21_Stack<int> st;
  EXPECT_EQ(st.empty(), true);
}

TEST(Suite_stack_constructor, size) {
  S21_Stack<int> st = {1};
  EXPECT_EQ(st.size(), 1U);
}

TEST(Suite_stack_constructor, size_1) {
  S21_Stack<int> st = {1, 2, 3, 4, 5, 6};
  EXPECT_EQ(st.size(), 6U);
}

TEST(Suite_stack_constructor, size_2) {
  S21_Stack<int> st;
  EXPECT_EQ(st.size(), 0U);
}

TEST(Suite_stack_constructor, insert_many_front) {
  S21_Stack<int> st;
  st.insert_many_front(1, 2, 3, 4);
  EXPECT_EQ(st.size(), 4U);
  EXPECT_EQ(st.top(), 4);
}

TEST(Suite_stack_constructor, insert_many_front_1) {
  S21_Stack<int> st = {5, 6};
  st.insert_many_front(1, 2, 3, 4, 5);
  EXPECT_EQ(st.size(), 7U);
  EXPECT_EQ(st.top(), 5);
}

TEST(Suite_stack_constructor, insert_many_front_2) {
  S21_Stack<int> st = {5, 6};
  st.insert_many_front(1, 2, 3);
  EXPECT_EQ(st.size(), 5U);
  EXPECT_EQ(st.top(), 3);
}

TEST(Suite_stack_constructor, insert_many_front_3) {
  S21_Stack<int> st = {5, 6};
  st.insert_many_front(1);
  EXPECT_EQ(st.size(), 3U);
  EXPECT_EQ(st.top(), 1);
}

TEST(Suite_stack_constructor, insert_many_front_4) {
  S21_Stack<int> st = {5, 6};
  st.insert_many_front();
  EXPECT_EQ(st.size(), 2U);
}