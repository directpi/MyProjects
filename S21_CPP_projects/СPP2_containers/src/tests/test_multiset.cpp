#include "tests_run.hpp"

TEST(Suite_multiset_constructor, basic_constructor) {
  S21_Multiset<int> st;
  EXPECT_EQ(st.size(), 0U);
  EXPECT_EQ(st.empty(), true);
}
TEST(Suite_multiset_constructor, basic_constructor_1) {
  S21_Multiset<int> st;
  EXPECT_EQ(st.size(), 0U);
  EXPECT_EQ(st.empty(), true);
}
TEST(Suite_multiset_constructor, basic_constructor_2) {
  S21_Multiset<int> st;
  EXPECT_EQ(st.size(), 0U);
  EXPECT_EQ(st.empty(), true);
}
TEST(Suite_multiset_constructor, basic_constructor_3) {
  S21_Multiset<string> st;
  EXPECT_EQ(st.size(), 0U);
  EXPECT_EQ(st.empty(), true);
}
TEST(Suite_multiset_constructor, init_constructor) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Multiset<int>::iterator iter = st.begin();
  EXPECT_EQ(st.size(), 3U);
  EXPECT_EQ(st.empty(), false);
  EXPECT_EQ(*iter, 1);
  EXPECT_EQ(*(++iter), 2);
  EXPECT_EQ(*(++iter), 3);
}
TEST(Suite_multiset_constructor, init_constructor_1) {
  S21_Multiset<int> st = {100};
  EXPECT_EQ(st.size(), 1U);
  EXPECT_EQ(st.empty(), false);
  EXPECT_EQ(*st.begin(), 100);
}
TEST(Suite_multiset_constructor, init_constructor_2) {
  S21_Multiset<int> st = {100, -100, 33, 22};
  S21_Multiset<int>::iterator iter = st.begin();
  EXPECT_EQ(st.size(), 4U);
  EXPECT_EQ(st.empty(), false);
  EXPECT_EQ(*iter, -100);
  EXPECT_EQ(*(++iter), 22);
  EXPECT_EQ(*(++iter), 33);
  EXPECT_EQ(*(++iter), 100);
}
TEST(Suite_multiset_constructor, copy_constructor) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Multiset<int> st2(st);
  S21_Multiset<int>::iterator iter = st2.begin();
  EXPECT_EQ(st2.size(), 3U);
  EXPECT_EQ(st2.empty(), false);
  EXPECT_EQ(*iter, 1);
  EXPECT_EQ(*(++iter), 2);
  EXPECT_EQ(*(++iter), 3);
}
TEST(Suite_multiset_constructor, copy_constructor_1) {
  S21_Multiset<int> st = {10};
  S21_Multiset<int> st2(st);
  S21_Multiset<int>::iterator iter = st2.begin();
  EXPECT_EQ(st2.size(), 1U);
  EXPECT_EQ(st2.empty(), false);
  EXPECT_EQ(*iter, 10);
}
TEST(Suite_multiset_constructor, copy_constructor_2) {
  S21_Multiset<int> st;
  EXPECT_ANY_THROW(S21_Multiset<int> st2(st));
}

TEST(Suite_multiset_constructor, move_constructor) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Multiset<int> st2(std::move(st));
  S21_Multiset<int>::iterator iter = st2.begin();
  EXPECT_EQ(st2.size(), 3U);
  EXPECT_EQ(st2.empty(), false);
  EXPECT_EQ(*iter, 1);
  EXPECT_EQ(*(++iter), 2);
  EXPECT_EQ(*(++iter), 3);

  EXPECT_EQ(st.size(), 0U);
  EXPECT_EQ(st.empty(), true);
}

TEST(Suite_multiset_constructor, move_constructor_1) {
  S21_Multiset<int> st = {33};
  S21_Multiset<int> st2(std::move(st));
  EXPECT_EQ(st2.size(), 1U);
  EXPECT_EQ(st2.empty(), false);
  EXPECT_EQ(*st2.begin(), 33);

  EXPECT_EQ(st.size(), 0U);
  EXPECT_EQ(st.empty(), true);
}
TEST(Suite_multiset_constructor, move_constructor_2) {
  S21_Multiset<int> st;
  EXPECT_ANY_THROW(S21_Multiset<int> st2(std::move(st)));
}

TEST(Suite_multiset_constructor, operator_eq) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Multiset<int> st2 = std::move(st);
  S21_Multiset<int>::iterator iter = st2.begin();
  EXPECT_EQ(st2.size(), 3U);
  EXPECT_EQ(st2.empty(), false);
  EXPECT_EQ(*iter, 1);
  EXPECT_EQ(*(++iter), 2);
  EXPECT_EQ(*(++iter), 3);

  EXPECT_EQ(st.size(), 0U);
  EXPECT_EQ(st.empty(), true);
}

TEST(Suite_multiset_iterators, begin) {
  S21_Multiset<int> st = {1, 2, 3};

  S21_Multiset<int>::iterator iter = st.begin();
  int p = *iter;
  EXPECT_EQ(p, 1);
}

TEST(Suite_multiset_iterators, begin_1) {
  S21_Multiset<int> st = {3};

  S21_Multiset<int>::iterator iter = st.begin();
  int p = *iter;
  EXPECT_EQ(p, 3);
}

TEST(Suite_multiset_iterators, begin_2) {
  S21_Multiset<int> st;

  S21_Multiset<int>::iterator iter = st.begin();
  EXPECT_EQ(iter, nullptr);
}

TEST(Suite_multiset_iterators, end) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Multiset<int>::iterator iter = st.end();
  EXPECT_EQ(iter, nullptr);
}

TEST(Suite_multiset_iterators, end_1) {
  S21_Multiset<int> st;

  S21_Multiset<int>::iterator iter = st.end();
  EXPECT_EQ(iter, nullptr);
}

TEST(Suite_multiset_iterators, operator_star) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Multiset<int>::iterator iter = st.begin();
  int p = *iter;
  EXPECT_EQ(p, 1);
}

TEST(Suite_multiset_iterators, operator_reference) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Multiset<int>::iterator iter = st.begin();
  EXPECT_EQ(iter->data, 1);
}

TEST(Suite_multiset_iterators, operator_increment) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Multiset<int>::iterator iter = st.begin();
  ++iter;
  int p = *iter;
  EXPECT_EQ(p, 2);
}

TEST(Suite_multiset_iterators, operator_decrement) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Multiset<int>::iterator iter = st.begin();
  ++iter;
  --iter;
  int p = *iter;
  EXPECT_EQ(p, 1);
}

TEST(Suite_multiset_iterators, operator_equal) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Multiset<int>::iterator iter = st.begin();
  S21_Multiset<int>::iterator iter2 = st.begin();
  EXPECT_EQ(iter == iter2, true);
  ++iter;
  EXPECT_EQ(iter == iter2, false);
}

TEST(Suite_multiset_iterators, operator_not_equal) {
  S21_Multiset<int> st = {1, 2, 3};

  S21_Multiset<int>::iterator iter = st.begin();
  S21_Multiset<int>::iterator iter2 = st.begin();
  EXPECT_EQ(iter != iter2, false);
  ++iter;
  EXPECT_EQ(iter != iter2, true);
}

TEST(Suite_multiset_capacity, empty) {
  S21_Multiset<int> st = {1, 2, 3};
  EXPECT_EQ(st.empty(), false);
}

TEST(Suite_multiset_capacity, empty_1) {
  S21_Multiset<int> st;
  EXPECT_EQ(st.empty(), true);
}

TEST(Suite_multiset_capacity, size) {
  S21_Multiset<int> st = {1, 2, 3};
  EXPECT_EQ(st.size(), 3U);
}

TEST(Suite_multiset_capacity, size_1) {
  S21_Multiset<int> st;
  EXPECT_EQ(st.size(), 0U);
}

TEST(Suite_multiset_capacity, max_size) {
  S21_Multiset<int> st = {1, 2, 3};
  EXPECT_EQ(st.max_size(),
            std::numeric_limits<std::size_t>::max() / sizeof(int));
}

TEST(Suite_multiset_modifiers, clear) {
  S21_Multiset<int> st = {1, 2, 3};
  st.clear();
  EXPECT_EQ(st.size(), 0U);
}

TEST(Suite_multiset_modifiers, clear_1) {
  S21_Multiset<int> st;
  st.clear();
  EXPECT_EQ(st.size(), 0U);
}

TEST(Suite_multiset_modifiers, insert) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Multiset<int>::iterator result = st.insert(5);
  S21_Multiset<int>::iterator iter = st.begin();
  for (size_t i = 0; i < st.size() - 1; ++i) {
    ++iter;
  }
  EXPECT_EQ(st.size(), 4U);
  EXPECT_EQ(*iter, 5);
  int p = *result;
  EXPECT_EQ(p, 5);
}
TEST(Suite_multiset_modifiers, insert_1) {
  S21_Multiset<int> st;
  S21_Multiset<int>::iterator result = st.insert(5);
  S21_Multiset<int>::iterator iter = st.begin();
  EXPECT_EQ(st.size(), 1U);
  EXPECT_EQ(*iter, 5);
  int p = *result;
  EXPECT_EQ(p, 5);
}

TEST(Suite_multiset_modifiers, insert_2) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Multiset<int>::iterator result = st.insert(3);
  EXPECT_EQ(st.size(), 4U);
  EXPECT_EQ(*result, 3);
}

TEST(Suite_multiset_modifiers, insert_key) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Multiset<int>::iterator result = st.insert(3);
  EXPECT_EQ(*result, 3);
}

TEST(Suite_multiset_modifiers, insert_key_1) {
  S21_Multiset<int> st;
  S21_Multiset<int>::iterator result = st.insert(5);
  EXPECT_EQ(st.size(), 1U);
  EXPECT_EQ(*result, 5);
}

TEST(Suite_multiset_modifiers, insert_key_2) {
  S21_Multiset<int> st = {1, 2};
  S21_Multiset<int>::iterator result = st.insert(3);
  EXPECT_EQ(st.size(), 3U);
  EXPECT_EQ(*result, 3);
}

TEST(Suite_multiset_modifiers, erase) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Multiset<int>::iterator iter = st.begin();
  st.erase(iter);
  EXPECT_EQ(st.size(), 2U);
  EXPECT_EQ(st.contains(1), false);
}

TEST(Suite_multiset_modifiers, erase_1) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Multiset<int>::iterator iter = st.begin();
  ++iter;
  st.erase(iter);
  EXPECT_EQ(st.size(), 2U);
  EXPECT_EQ(st.contains(2), false);
}

TEST(Suite_multiset_modifiers, erase_2) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Multiset<int>::iterator iter = st.begin();
  ++iter;
  ++iter;
  st.erase(iter);
  EXPECT_EQ(st.size(), 2U);
  EXPECT_EQ(st.contains(3), false);
}

TEST(Suite_multiset_modifiers, erase_3) {
  S21_Multiset<int> st;
  S21_Multiset<int>::iterator iter = st.begin();
  EXPECT_ANY_THROW(st.erase(iter));
}

TEST(Suite_multiset_modifiers, swap) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Multiset<int> st2 = {5, 6};

  st.swap(st2);

  EXPECT_EQ(st.size(), 2U);
  EXPECT_EQ(st.contains(5), 1);
  EXPECT_EQ(st.contains(6), 1);

  EXPECT_EQ(st2.size(), 3U);
  EXPECT_EQ(st2.contains(1), 1);
  EXPECT_EQ(st2.contains(2), 1);
  EXPECT_EQ(st2.contains(3), 1);
}

TEST(Suite_multiset_modifiers, merge) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Multiset<int> st2 = {5, 6};

  st.merge(st2);

  EXPECT_EQ(st.size(), 5U);
  EXPECT_EQ(st.contains(5), 1);
  EXPECT_EQ(st.contains(6), 1);
  EXPECT_EQ(st.contains(1), 1);
  EXPECT_EQ(st.contains(2), 1);
  EXPECT_EQ(st.contains(3), 1);

  EXPECT_EQ(st2.size(), 0U);
}

TEST(Suite_multiset_modifiers, merge_2) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Multiset<int> st2 = {5, 6, 3};
  st.merge(st2);
  S21_Multiset<int>::iterator iter = st.begin();

  EXPECT_EQ(st.size(), 6U);
  EXPECT_EQ(*iter, 1);
  EXPECT_EQ(*(++iter), 2);
  EXPECT_EQ(*(++iter), 3);
  EXPECT_EQ(*(++iter), 3);
  EXPECT_EQ(*(++iter), 5);
  EXPECT_EQ(*(++iter), 6);

  EXPECT_EQ(st2.size(), 0U);
}

TEST(Suite_multiset_modifiers, insert_many) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Vector<std::pair<S21_Multiset<int>::iterator, bool>> result =
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

TEST(Suite_multiset_modifiers, insert_many_1) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Vector<std::pair<S21_Multiset<int>::iterator, bool>> result =
      st.insert_many(5, 4, 3);
  EXPECT_EQ(st.size(), 6U);
  EXPECT_EQ(st.contains(5), 1);
  EXPECT_EQ(st.contains(4), 1);
  EXPECT_EQ(result[0].first->data, 5);
  EXPECT_EQ(result[0].second, true);
  EXPECT_EQ(result[1].first->data, 4);
  EXPECT_EQ(result[1].second, true);
  EXPECT_EQ(result[2].first->data, 3);
  EXPECT_EQ(result[2].second, true);
}

TEST(Suite_multiset_modifiers, insert_many_2) {
  S21_Multiset<int> st;
  S21_Vector<std::pair<S21_Multiset<int>::iterator, bool>> result =
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

TEST(Suite_multiset_modifiers, insert_many_3) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Vector<std::pair<S21_Multiset<int>::iterator, bool>> result =
      st.insert_many();
  EXPECT_EQ(st.size(), 3U);
  EXPECT_EQ(result[0].first, nullptr);
  EXPECT_EQ(result[0].second, false);
}

TEST(Suite_multiset_modifiers, insert_many_4) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Vector<std::pair<S21_Multiset<int>::iterator, bool>> result =
      st.insert_many(5);
  EXPECT_EQ(st.size(), 4U);
  EXPECT_EQ(result[0].first->data, 5);
  EXPECT_EQ(result[0].second, true);
}

TEST(Suite_multiset_modifiers, insert_many_5) {
  S21_Multiset<int> st = {1, 2, 3};
  S21_Vector<std::pair<S21_Multiset<int>::iterator, bool>> result =
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

TEST(Suite_multiset_lookup, count) {
  S21_Multiset<int> st = {1, 2, 3, 4, 3, 2, 3};
  EXPECT_EQ(st.count(3), 3U);
}

TEST(Suite_multiset_lookup, count_1) {
  S21_Multiset<int> st = {1, 2, 3, 4, 3, 2, 3};
  EXPECT_EQ(st.count(5), 0U);
}

TEST(Suite_multiset_lookup, count_2) {
  S21_Multiset<int> st = {1, 1, 1};
  EXPECT_EQ(st.count(1), 3U);
}

TEST(Suite_multiset_lookup, count_3) {
  S21_Multiset<int> st;
  EXPECT_EQ(st.count(1), 0U);
}

TEST(Suite_multiset_lookup, find) {
  S21_Multiset<int> st = {1, 2, 3, 4, 3, 2, 3};
  S21_Multiset<int>::iterator iter = st.find(3);
  EXPECT_EQ(*iter, 3);
}

TEST(Suite_multiset_lookup, find_1) {
  S21_Multiset<int> st = {1, 2, 3, 4, 3, 2, 3};
  S21_Multiset<int>::iterator iter = st.find(2);
  EXPECT_EQ(*iter, 2);
}

TEST(Suite_multiset_lookup, find_2) {
  S21_Multiset<int> st = {1, 2, 3, 4, 3, 2, 3};
  S21_Multiset<int>::iterator iter = st.find(0);
  EXPECT_EQ(iter, nullptr);
}

TEST(Suite_multiset_lookup, find_3) {
  S21_Multiset<int> st;
  S21_Multiset<int>::iterator iter;
  EXPECT_ANY_THROW(st.find(33););
}

TEST(Suite_multiset_lookup, contains) {
  S21_Multiset<int> st = {1, 2, 3, 4, 3, 2, 3};
  EXPECT_EQ(st.contains(3), true);
}

TEST(Suite_multiset_lookup, contains_1) {
  S21_Multiset<int> st = {1, 2, 3, 4, 3, 2, 3};
  EXPECT_EQ(st.contains(2), true);
}

TEST(Suite_multiset_lookup, contains_2) {
  S21_Multiset<int> st = {1, 2, 3, 4, 3, 2, 3};
}

TEST(Suite_multiset_lookup, contains_3) {
  S21_Multiset<int> st;
  EXPECT_ANY_THROW(st.contains(0));
}

TEST(Suite_multiset_lookup, equal_range) {
  S21_Multiset<int> st = {1, 2, 3, 4, 3, 2, 3};
  std::pair<S21_Multiset<int>::iterator, S21_Multiset<int>::iterator> pair =
      st.equal_range(3);
  EXPECT_EQ(*pair.first, 3);
  EXPECT_EQ(*pair.second, 4);

  size_t count = 0;
  while (pair.first != pair.second) {
    ++count;
    ++pair.first;
  }
  EXPECT_EQ(count, 3U);
}

TEST(Suite_multiset_lookup, equal_range_1) {
  S21_Multiset<int> st = {1, 2, 3, 4, 3, 2, 3};
  std::pair<S21_Multiset<int>::iterator, S21_Multiset<int>::iterator> pair =
      st.equal_range(0);
  EXPECT_EQ(pair.first, nullptr);
  EXPECT_EQ(pair.second, nullptr);

  size_t count = 0;
  while (pair.first != pair.second) {
    ++count;
    ++pair.first;
  }
  EXPECT_EQ(count, 0U);
}

TEST(Suite_multiset_lookup, lower_bound) {
  S21_Multiset<int> st = {1, 2, 3, 4, 3, 2, 3};
  S21_Multiset<int>::iterator iter = st.lower_bound(3);
  EXPECT_EQ(*iter, 3);
}

TEST(Suite_multiset_lookup, lower_bound_1) {
  S21_Multiset<int> st = {1, 2, 3, 4, 3, 2, 3};
  S21_Multiset<int>::iterator iter = st.lower_bound(2);
  EXPECT_EQ(*iter, 2);
}

TEST(Suite_multiset_lookup, lower_bound_2) {
  S21_Multiset<int> st = {1, 2, 3, 4, 3, 2, 3};
  S21_Multiset<int>::iterator iter = st.lower_bound(0);
  EXPECT_EQ(iter, nullptr);
}

TEST(Suite_multiset_lookup, lower_bound_3) {
  S21_Multiset<int> st;
  S21_Multiset<int>::iterator iter = st.lower_bound(33);
  EXPECT_EQ(iter, nullptr);
}

TEST(Suite_multiset_lookup, upper_bound) {
  S21_Multiset<int> st = {1, 2, 3, 4, 3, 2, 3};
  S21_Multiset<int>::iterator iter = st.upper_bound(3);
  EXPECT_EQ(*iter, 4);
}

TEST(Suite_multiset_lookup, upper_bound_1) {
  S21_Multiset<int> st = {1, 2, 3, 4, 3, 2, 3};
  S21_Multiset<int>::iterator iter = st.upper_bound(4);
  EXPECT_EQ(iter, nullptr);
}

TEST(Suite_multiset_lookup, upper_bound_2) {
  S21_Multiset<int> st = {1, 2, 3, 4, 3, 2, 3};
  S21_Multiset<int>::iterator iter = st.upper_bound(0);
  EXPECT_EQ(iter, nullptr);
}

TEST(Suite_multiset_lookup, upper_bound_3) {
  S21_Multiset<int> st;
  S21_Multiset<int>::iterator iter = st.upper_bound(33);
  EXPECT_EQ(iter, nullptr);
}