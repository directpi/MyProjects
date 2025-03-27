#include "tests_run.hpp"

TEST(Suite_set_constructor, basic_constructor) {
  S21_Set<int> st;
  EXPECT_EQ(st.size(), 0U);
  EXPECT_EQ(st.empty(), true);
}
TEST(Suite_set_constructor, basic_constructor_1) {
  S21_Set<int> st;
  EXPECT_EQ(st.size(), 0U);
  EXPECT_EQ(st.empty(), true);
}
TEST(Suite_set_constructor, basic_constructor_2) {
  S21_Set<int> st;
  EXPECT_EQ(st.size(), 0U);
  EXPECT_EQ(st.empty(), true);
}
TEST(Suite_set_constructor, basic_constructor_3) {
  S21_Set<string> st;
  EXPECT_EQ(st.size(), 0U);
  EXPECT_EQ(st.empty(), true);
}
TEST(Suite_set_constructor, init_constructor) {
  S21_Set<int> st = {1, 2, 3};
  S21_Set<int>::iterator iter = st.begin();
  EXPECT_EQ(st.size(), 3U);
  EXPECT_EQ(st.empty(), false);
  EXPECT_EQ(*iter, 1);
  EXPECT_EQ(*(++iter), 2);
  EXPECT_EQ(*(++iter), 3);
}
TEST(Suite_set_constructor, init_constructor_1) {
  S21_Set<int> st = {100};
  EXPECT_EQ(st.size(), 1U);
  EXPECT_EQ(st.empty(), false);
  EXPECT_EQ(*st.begin(), 100);
}
TEST(Suite_set_constructor, init_constructor_2) {
  S21_Set<int> st = {100, -100, 33, 22};
  S21_Set<int>::iterator iter = st.begin();
  EXPECT_EQ(st.size(), 4U);
  EXPECT_EQ(st.empty(), false);
  EXPECT_EQ(*iter, -100);
  EXPECT_EQ(*(++iter), 22);
  EXPECT_EQ(*(++iter), 33);
  EXPECT_EQ(*(++iter), 100);
}
TEST(Suite_set_constructor, copy_constructor) {
  S21_Set<int> st = {1, 2, 3};
  S21_Set<int> st2(st);
  S21_Set<int>::iterator iter = st2.begin();
  EXPECT_EQ(st2.size(), 3U);
  EXPECT_EQ(st2.empty(), false);
  EXPECT_EQ(*iter, 1);
  EXPECT_EQ(*(++iter), 2);
  EXPECT_EQ(*(++iter), 3);
}
TEST(Suite_set_constructor, copy_constructor_1) {
  S21_Set<int> st = {10};
  S21_Set<int> st2(st);
  S21_Set<int>::iterator iter = st2.begin();
  EXPECT_EQ(st2.size(), 1U);
  EXPECT_EQ(st2.empty(), false);
  EXPECT_EQ(*iter, 10);
}
TEST(Suite_set_constructor, copy_constructor_2) {
  S21_Set<int> st;
  EXPECT_ANY_THROW(S21_Set<int> st2(st));
}

TEST(Suite_set_constructor, move_constructor) {
  S21_Set<int> st = {1, 2, 3};
  S21_Set<int> st2(std::move(st));
  S21_Set<int>::iterator iter = st2.begin();
  EXPECT_EQ(st2.size(), 3U);
  EXPECT_EQ(st2.empty(), false);
  EXPECT_EQ(*iter, 1);
  EXPECT_EQ(*(++iter), 2);
  EXPECT_EQ(*(++iter), 3);

  EXPECT_EQ(st.size(), 0U);
  EXPECT_EQ(st.empty(), true);
}

TEST(Suite_set_constructor, move_constructor_1) {
  S21_Set<int> st = {33};
  S21_Set<int> st2(std::move(st));
  EXPECT_EQ(st2.size(), 1U);
  EXPECT_EQ(st2.empty(), false);
  EXPECT_EQ(*st2.begin(), 33);

  EXPECT_EQ(st.size(), 0U);
  EXPECT_EQ(st.empty(), true);
}
TEST(Suite_set_constructor, move_constructor_2) {
  S21_Set<int> st;
  EXPECT_ANY_THROW(S21_Set<int> st2(std::move(st)));
}

TEST(Suite_set_constructor, operator_eq) {
  S21_Set<int> st = {1, 2, 3};
  S21_Set<int> st2 = std::move(st);
  S21_Set<int>::iterator iter = st2.begin();
  EXPECT_EQ(st2.size(), 3U);
  EXPECT_EQ(st2.empty(), false);
  EXPECT_EQ(*iter, 1);
  EXPECT_EQ(*(++iter), 2);
  EXPECT_EQ(*(++iter), 3);

  EXPECT_EQ(st.size(), 0U);
  EXPECT_EQ(st.empty(), true);
}

TEST(Suite_set_iterators, begin) {
  S21_Set<int> st = {1, 2, 3};

  S21_Set<int>::iterator iter = st.begin();
  int p = *iter;
  EXPECT_EQ(p, 1);
}

TEST(Suite_set_iterators, begin_1) {
  S21_Set<int> st = {3};

  S21_Set<int>::iterator iter = st.begin();
  int p = *iter;
  EXPECT_EQ(p, 3);
}

TEST(Suite_set_iterators, begin_2) {
  S21_Set<int> st;

  S21_Set<int>::iterator iter = st.begin();
  EXPECT_EQ(iter, nullptr);
}

TEST(Suite_set_iterators, end) {
  S21_Set<int> st = {1, 2, 3};
  S21_Set<int>::iterator iter = st.end();
  EXPECT_EQ(iter, nullptr);
}

TEST(Suite_set_iterators, end_1) {
  S21_Set<int> st;

  S21_Set<int>::iterator iter = st.end();
  EXPECT_EQ(iter, nullptr);
}

TEST(Suite_set_iterators, operator_star) {
  S21_Set<int> st = {1, 2, 3};
  S21_Set<int>::iterator iter = st.begin();
  int p = *iter;
  EXPECT_EQ(p, 1);
}

TEST(Suite_set_iterators, operator_reference) {
  S21_Set<int> st = {1, 2, 3};
  S21_Set<int>::iterator iter = st.begin();
  EXPECT_EQ(iter->data, 1);
}

TEST(Suite_set_iterators, operator_increment) {
  S21_Set<int> st = {1, 2, 3};
  S21_Set<int>::iterator iter = st.begin();
  ++iter;
  int p = *iter;
  EXPECT_EQ(p, 2);
}

TEST(Suite_set_iterators, operator_decrement) {
  S21_Set<int> st = {1, 2, 3};
  S21_Set<int>::iterator iter = st.begin();
  ++iter;
  --iter;
  int p = *iter;
  EXPECT_EQ(p, 1);
}

TEST(Suite_set_iterators, operator_equal) {
  S21_Set<int> st = {1, 2, 3};
  S21_Set<int>::iterator iter = st.begin();
  S21_Set<int>::iterator iter2 = st.begin();
  EXPECT_EQ(iter == iter2, true);
  ++iter;
  EXPECT_EQ(iter == iter2, false);
}

TEST(Suite_set_iterators, operator_not_equal) {
  S21_Set<int> st = {1, 2, 3};

  S21_Set<int>::iterator iter = st.begin();
  S21_Set<int>::iterator iter2 = st.begin();
  EXPECT_EQ(iter != iter2, false);
  ++iter;
  EXPECT_EQ(iter != iter2, true);
}

TEST(Suite_set_capacity, empty) {
  S21_Set<int> st = {1, 2, 3};
  EXPECT_EQ(st.empty(), false);
}

TEST(Suite_set_capacity, empty_1) {
  S21_Set<int> st;
  EXPECT_EQ(st.empty(), true);
}

TEST(Suite_set_capacity, size) {
  S21_Set<int> st = {1, 2, 3};
  EXPECT_EQ(st.size(), 3U);
}

TEST(Suite_set_capacity, size_1) {
  S21_Set<int> st;
  EXPECT_EQ(st.size(), 0U);
}

TEST(Suite_set_capacity, max_size) {
  S21_Set<int> st = {1, 2, 3};
  EXPECT_EQ(st.max_size(),
            std::numeric_limits<std::size_t>::max() / sizeof(int));
}

TEST(Suite_set_modifiers, clear) {
  S21_Set<int> st = {1, 2, 3};
  st.clear();
  EXPECT_EQ(st.size(), 0U);
}

TEST(Suite_set_modifiers, clear_1) {
  S21_Set<int> st;
  st.clear();
  EXPECT_EQ(st.size(), 0U);
}

TEST(Suite_set_modifiers, insert) {
  S21_Set<int> st = {1, 2, 3};
  std::pair<S21_Set<int>::iterator, bool> result = st.insert(5);
  S21_Set<int>::iterator iter = st.begin();
  for (size_t i = 0; i < st.size() - 1; ++i) {
    ++iter;
  }
  EXPECT_EQ(st.size(), 4U);
  EXPECT_EQ(*iter, 5);
  EXPECT_EQ(result.second, true);
  int p = *result.first;
  EXPECT_EQ(p, 5);
}
TEST(Suite_set_modifiers, insert_1) {
  S21_Set<int> st;
  std::pair<S21_Set<int>::iterator, bool> result = st.insert(5);
  S21_Set<int>::iterator iter = st.begin();
  EXPECT_EQ(st.size(), 1U);
  EXPECT_EQ(*iter, 5);
  EXPECT_EQ(result.second, true);
  int p = *result.first;
  EXPECT_EQ(p, 5);
}

TEST(Suite_set_modifiers, insert_2) {
  S21_Set<int> st = {1, 2, 3};
  std::pair<S21_Set<int>::iterator, bool> result = st.insert(3);
  EXPECT_EQ(st.size(), 3U);
  EXPECT_EQ(result.second, false);
  EXPECT_EQ(result.first, nullptr);
}

TEST(Suite_set_modifiers, insert_key) {
  S21_Set<int> st = {1, 2, 3};
  std::pair<S21_Set<int>::iterator, bool> result = st.insert(5);
  S21_Set<int>::iterator iter = st.begin();
  EXPECT_EQ(st.size(), 4U);
  for (size_t i = 0; i < st.size() - 1; ++i) {
    ++iter;
  }
  EXPECT_EQ(*iter, 5);
  EXPECT_EQ(result.second, true);
}

TEST(Suite_set_modifiers, insert_key_1) {
  S21_Set<int> st;
  std::pair<S21_Set<int>::iterator, bool> result = st.insert(5);
  S21_Set<int>::iterator iter = st.begin();
  EXPECT_EQ(st.size(), 1U);
  EXPECT_EQ(*iter, 5);
  EXPECT_EQ(result.second, true);
}

TEST(Suite_set_modifiers, insert_key_2) {
  S21_Set<int> st = {1, 2};
  S21_Set<int>::iterator iter = st.begin();
  st.insert(3);
  EXPECT_EQ(st.size(), 3U);
  ++iter;
  ++iter;
  EXPECT_EQ(*(iter), 3);
}

//

TEST(Suite_set_modifiers, erase) {
  S21_Set<int> st = {1, 2, 3};
  S21_Set<int>::iterator iter = st.begin();
  st.erase(iter);
  EXPECT_EQ(st.size(), 2U);
  EXPECT_EQ(st.contains(1), false);
}

TEST(Suite_set_modifiers, erase_1) {
  S21_Set<int> st = {1, 2, 3};
  S21_Set<int>::iterator iter = st.begin();
  ++iter;
  st.erase(iter);
  EXPECT_EQ(st.size(), 2U);
  EXPECT_EQ(st.contains(2), false);
}

TEST(Suite_set_modifiers, erase_2) {
  S21_Set<int> st = {1, 2, 3};
  S21_Set<int>::iterator iter = st.begin();
  ++iter;
  ++iter;
  st.erase(iter);
  EXPECT_EQ(st.size(), 2U);
  EXPECT_EQ(st.contains(3), false);
}

TEST(Suite_set_modifiers, erase_3) {
  S21_Set<int> st;
  S21_Set<int>::iterator iter = st.begin();
  EXPECT_ANY_THROW(st.erase(iter));
}

TEST(Suite_set_modifiers, swap) {
  S21_Set<int> st = {1, 2, 3};
  S21_Set<int> st2 = {5, 6};

  st.swap(st2);

  EXPECT_EQ(st.size(), 2U);
  EXPECT_EQ(st.contains(5), 1);
  EXPECT_EQ(st.contains(6), 1);

  EXPECT_EQ(st2.size(), 3U);
  EXPECT_EQ(st2.contains(1), 1);
  EXPECT_EQ(st2.contains(2), 1);
  EXPECT_EQ(st2.contains(3), 1);
}

TEST(Suite_set_modifiers, merge) {
  S21_Set<int> st = {1, 2, 3};
  S21_Set<int> st2 = {5, 6};

  st.merge(st2);

  EXPECT_EQ(st.size(), 5U);
  EXPECT_EQ(st.contains(5), 1);
  EXPECT_EQ(st.contains(6), 1);
  EXPECT_EQ(st.contains(1), 1);
  EXPECT_EQ(st.contains(2), 1);
  EXPECT_EQ(st.contains(3), 1);

  EXPECT_EQ(st2.size(), 0U);
}

TEST(Suite_set_modifiers, merge_2) {
  S21_Set<int> st = {1, 2, 3};
  S21_Set<int> st2 = {5, 6, 3};
  st.merge(st2);
  S21_Set<int>::iterator iter = st.begin();
  S21_Set<int>::iterator iter2 = st2.begin();

  EXPECT_EQ(st.size(), 5U);
  EXPECT_EQ(*iter, 1);
  EXPECT_EQ(*(++iter), 2);
  EXPECT_EQ(*(++iter), 3);
  EXPECT_EQ(*(++iter), 5);
  EXPECT_EQ(*(++iter), 6);

  EXPECT_EQ(st2.size(), 1U);
  EXPECT_EQ(*iter2, 3);
}

TEST(Suite_set_modifiers, insert_many) {
  S21_Set<int> st = {1, 2, 3};
  S21_Vector<std::pair<S21_Set<int>::iterator, bool>> result =
      st.insert_many(5, 4, 6);
  EXPECT_EQ(st.size(), 6U);
  EXPECT_EQ(st.contains(5), 1);
  EXPECT_EQ(st.contains(4), 1);
  EXPECT_EQ(st.contains(6), 1);
  EXPECT_EQ(result[0].first->data, 5);
  EXPECT_EQ(result[0].second, true);
  EXPECT_EQ(result[1].first->data, 4);
  EXPECT_EQ(result[1].second, true);
  EXPECT_EQ(result[2].first->data, 6);
  EXPECT_EQ(result[2].second, true);
}

TEST(Suite_set_modifiers, insert_many_1) {
  S21_Set<int> st = {1, 2, 3};
  S21_Vector<std::pair<S21_Set<int>::iterator, bool>> result =
      st.insert_many(5, 4, 3);
  EXPECT_EQ(st.size(), 5U);
  EXPECT_EQ(st.contains(5), 1);
  EXPECT_EQ(st.contains(4), 1);
  EXPECT_EQ(result[0].first->data, 5);
  EXPECT_EQ(result[0].second, true);
  EXPECT_EQ(result[1].first->data, 4);
  EXPECT_EQ(result[1].second, true);
  EXPECT_EQ(result[2].first, nullptr);
  EXPECT_EQ(result[2].second, false);
}

TEST(Suite_set_modifiers, insert_many_2) {
  S21_Set<int> st;
  S21_Vector<std::pair<S21_Set<int>::iterator, bool>> result =
      st.insert_many(5, 4, 3);
  EXPECT_EQ(st.size(), 3U);
  EXPECT_EQ(st.contains(5), 1);
  EXPECT_EQ(st.contains(4), 1);
  EXPECT_EQ(st.contains(3), 1);
  EXPECT_EQ(result[0].first->data, 5);
  EXPECT_EQ(result[0].second, true);
  EXPECT_EQ(result[1].first->data, 4);
  EXPECT_EQ(result[1].second, true);
  EXPECT_EQ(result[2].first->data, 3);
  EXPECT_EQ(result[2].second, true);
}

TEST(Suite_set_modifiers, insert_many_3) {
  S21_Set<int> st = {1, 2, 3};
  S21_Vector<std::pair<S21_Set<int>::iterator, bool>> result = st.insert_many();
  EXPECT_EQ(st.size(), 3U);
  EXPECT_EQ(result[0].first, nullptr);
  EXPECT_EQ(result[0].second, false);
}

TEST(Suite_set_modifiers, insert_many_4) {
  S21_Set<int> st = {1, 2, 3};
  S21_Vector<std::pair<S21_Set<int>::iterator, bool>> result =
      st.insert_many(5);
  EXPECT_EQ(st.size(), 4U);
  EXPECT_EQ(result[0].first->data, 5);
  EXPECT_EQ(result[0].second, true);
}

TEST(Suite_set_modifiers, insert_many_5) {
  S21_Set<int> st = {1, 2, 3};
  S21_Vector<std::pair<S21_Set<int>::iterator, bool>> result =
      st.insert_many(5, 6, 7, 8, 9);
  EXPECT_EQ(st.size(), 8U);
  EXPECT_EQ(result[0].first->data, 5);
  EXPECT_EQ(result[0].second, true);
  EXPECT_EQ(result[1].first->data, 6);
  EXPECT_EQ(result[1].second, true);
  EXPECT_EQ(result[2].first->data, 7);
  EXPECT_EQ(result[2].second, true);
  EXPECT_EQ(result[3].first->data, 8);
  EXPECT_EQ(result[3].second, true);
  EXPECT_EQ(result[4].first->data, 9);
  EXPECT_EQ(result[4].second, true);
}

TEST(Suite_set_modifiers, contains) {
  S21_Set<int> st = {1, 2, 3};

  EXPECT_EQ(st.contains(1), true);
  EXPECT_EQ(st.contains(2), true);
  EXPECT_EQ(st.contains(3), true);
  EXPECT_EQ(st.contains(0), false);
}
