#include "tests_run.hpp"

TEST(Suite_queue_constructor, basic_constructor) {
  S21_Queue<int> qu;
  EXPECT_EQ(qu.size(), 0U);
  EXPECT_EQ(qu.empty(), true);
}

TEST(Suite_queue_constructor, basic_constructor_1) {
  S21_Queue<double> qu;
  EXPECT_EQ(qu.size(), 0U);
  EXPECT_EQ(qu.empty(), true);
}

TEST(Suite_queue_constructor, basic_constructor_2) {
  S21_Queue<char> qu;
  EXPECT_EQ(qu.size(), 0U);
  EXPECT_EQ(qu.empty(), true);
}

TEST(Suite_queue_constructor, init_constructor) {
  S21_Queue<int> qu = {1};
  EXPECT_EQ(qu.size(), 1U);
  EXPECT_EQ(qu.front(), 1);
  EXPECT_EQ(qu.back(), 1);
}

TEST(Suite_queue_constructor, init_constructor_2) {
  S21_Queue<int> qu = {1, 2, 3, 4};
  EXPECT_EQ(qu.size(), 4U);
  EXPECT_EQ(qu.front(), 1);
  EXPECT_EQ(qu.back(), 4);
}

TEST(Suite_queue_constructor, init_constructor_3) {
  S21_Queue<int> qu = {};
  EXPECT_EQ(qu.size(), 0U);
  EXPECT_EQ(qu.empty(), true);
}

TEST(Suite_queue_constructor, copy_constructor) {
  S21_Queue<int> qu = {1, 2, 3, 4};
  S21_Queue<int> qu2(qu);

  EXPECT_EQ(qu2.size(), 4U);
  EXPECT_EQ(qu2.front(), 1);
  EXPECT_EQ(qu2.back(), 4);
}

TEST(Suite_queue_constructor, copy_constructor_1) {
  S21_Queue<int> qu = {1};
  S21_Queue<int> qu2(qu);

  EXPECT_EQ(qu2.size(), 1U);
  EXPECT_EQ(qu2.front(), 1);
  EXPECT_EQ(qu2.back(), 1);
}

TEST(Suite_queue_constructor, copy_constructor_2) {
  S21_Queue<int> qu;
  EXPECT_ANY_THROW(S21_Queue<int> qu2(qu););
}

TEST(Suite_queue_constructor, move_constructor) {
  S21_Queue<int> qu = {1, 2, 3, 4};
  S21_Queue<int> qu2(std::move(qu));

  EXPECT_EQ(qu.size(), 0U);
  EXPECT_EQ(qu2.size(), 4U);
  EXPECT_EQ(qu2.front(), 1);
  EXPECT_EQ(qu2.back(), 4);
}

TEST(Suite_queue_constructor, move_constructor_1) {
  S21_Queue<int> qu = {1};
  S21_Queue<int> qu2(std::move(qu));

  EXPECT_EQ(qu.size(), 0U);
  EXPECT_EQ(qu2.size(), 1U);
  EXPECT_EQ(qu2.front(), 1);
  EXPECT_EQ(qu2.back(), 1);
}

TEST(Suite_queue_constructor, move_constructor_2) {
  S21_Queue<int> qu;
  EXPECT_ANY_THROW(S21_Queue<int> qu2(std::move(qu)););
}

TEST(Suite_queue_constructor, eq_operator) {
  S21_Queue<int> qu = {1, 2, 3, 4};
  S21_Queue<int> qu2 = std::move(qu);

  EXPECT_EQ(qu.size(), 0U);
  EXPECT_EQ(qu2.size(), 4U);
  EXPECT_EQ(qu2.front(), 1);
  EXPECT_EQ(qu2.back(), 4);
}

TEST(Suite_queue_constructor, eq_operator_1) {
  S21_Queue<int> qu = {1};
  S21_Queue<int> qu2 = std::move(qu);

  EXPECT_EQ(qu.size(), 0U);
  EXPECT_EQ(qu2.size(), 1U);
  EXPECT_EQ(qu2.front(), 1);
  EXPECT_EQ(qu2.back(), 1);
}

TEST(Suite_queue_constructor, eq_operator_2) {
  S21_Queue<int> qu;
  EXPECT_ANY_THROW(S21_Queue<int> qu2 = std::move(qu););
}

TEST(Suite_queue_constructor, front) {
  S21_Queue<int> qu = {1, 2, 3, 4};

  EXPECT_EQ(qu.size(), 4U);
  EXPECT_EQ(qu.front(), 1);
}

TEST(Suite_queue_constructor, front_1) {
  S21_Queue<int> qu = {33};

  EXPECT_EQ(qu.size(), 1U);
  EXPECT_EQ(qu.front(), 33);
}

TEST(Suite_queue_constructor, front_2) {
  S21_Queue<int> qu;
  EXPECT_ANY_THROW(qu.front());
}

TEST(Suite_queue_constructor, back) {
  S21_Queue<int> qu = {1, 2, 3, 4};

  EXPECT_EQ(qu.size(), 4U);
  EXPECT_EQ(qu.back(), 4);
}

TEST(Suite_queue_constructor, back_1) {
  S21_Queue<int> qu = {33};

  EXPECT_EQ(qu.size(), 1U);
  EXPECT_EQ(qu.back(), 33);
}

TEST(Suite_queue_constructor, back_2) {
  S21_Queue<int> qu;
  EXPECT_ANY_THROW(qu.back());
}

TEST(Suite_queue_constructor, empty) {
  S21_Queue<int> qu = {1, 2, 3, 4};
  EXPECT_EQ(qu.empty(), false);
}

TEST(Suite_queue_constructor, empty_1) {
  S21_Queue<int> qu;
  ;
  EXPECT_EQ(qu.empty(), true);
}

TEST(Suite_queue_constructor, size) {
  S21_Queue<int> qu = {1, 2, 3, 4};
  EXPECT_EQ(qu.size(), 4U);
}

TEST(Suite_queue_constructor, size_1) {
  S21_Queue<int> qu;
  ;
  EXPECT_EQ(qu.size(), 0U);
}

TEST(Suite_queue_constructor, swap) {
  S21_Queue<int> qu = {1, 2, 3, 4};
  S21_Queue<int> qu2 = {5, 6, 7};
  qu.swap(qu2);
  EXPECT_EQ(qu.size(), 3U);
  EXPECT_EQ(qu2.size(), 4U);
}

TEST(Suite_queue_constructor, swap_1) {
  S21_Queue<int> qu;
  S21_Queue<int> qu2 = {5, 6, 7};
  EXPECT_ANY_THROW(qu.swap(qu2););
}

TEST(Suite_queue_constructor, swap_2) {
  S21_Queue<int> qu = {1, 2, 3, 4};
  S21_Queue<int> qu2;
  EXPECT_ANY_THROW(qu.swap(qu2););
}

TEST(Suite_queue_constructor, insert_many_back) {
  S21_Queue<int> qu = {1, 2, 3, 4};
  qu.insert_many_back(5, 6, 7, 8);
  EXPECT_EQ(qu.size(), 8U);
  EXPECT_EQ(qu.front(), 1);
  EXPECT_EQ(qu.back(), 8);
}

TEST(Suite_queue_constructor, insert_many_back_1) {
  S21_Queue<int> qu;
  qu.insert_many_back(5, 6, 7, 8);
  EXPECT_EQ(qu.size(), 4U);
  EXPECT_EQ(qu.front(), 5);
  EXPECT_EQ(qu.back(), 8);
}

TEST(Suite_queue_constructor, insert_many_back_2) {
  S21_Queue<int> qu = {1, 2, 3, 4};
  qu.insert_many_back(5, 6, 7);
  EXPECT_EQ(qu.size(), 7U);
  EXPECT_EQ(qu.front(), 1);
  EXPECT_EQ(qu.back(), 7);
}

TEST(Suite_queue_constructor, insert_many_back_3) {
  S21_Queue<int> qu = {1, 2, 3, 4};
  qu.insert_many_back(5);
  EXPECT_EQ(qu.size(), 5U);
  EXPECT_EQ(qu.front(), 1);
  EXPECT_EQ(qu.back(), 5);
}

TEST(Suite_queue_constructor, insert_many_back_4) {
  S21_Queue<int> qu = {1, 2, 3, 4};
  qu.insert_many_back();
  EXPECT_EQ(qu.size(), 4U);
  EXPECT_EQ(qu.front(), 1);
  EXPECT_EQ(qu.back(), 4);
}