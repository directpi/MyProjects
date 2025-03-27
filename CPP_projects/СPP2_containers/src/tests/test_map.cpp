#include "tests_run.hpp"

TEST(Suite_map_constructor, basic_constructor) {
  S21_Map<int, int> mp;
  EXPECT_EQ(mp.size(), 0U);
  EXPECT_EQ(mp.empty(), true);
}

TEST(Suite_map_constructor, basic_constructor_1) {
  S21_Map<int, double> mp;
  EXPECT_EQ(mp.size(), 0U);
  EXPECT_EQ(mp.empty(), true);
}

TEST(Suite_map_constructor, basic_constructor_2) {
  S21_Map<int, char> mp;
  EXPECT_EQ(mp.size(), 0U);
  EXPECT_EQ(mp.empty(), true);
}

TEST(Suite_map_constructor, basic_constructor_3) {
  S21_Map<string, double> mp;
  EXPECT_EQ(mp.size(), 0U);
  EXPECT_EQ(mp.empty(), true);
}

TEST(Suite_map_constructor, init_constructor) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  EXPECT_EQ(mp.size(), 3U);
  EXPECT_EQ(mp.empty(), false);
  EXPECT_EQ(mp[1], 1);
  EXPECT_EQ(mp[2], 2);
  EXPECT_EQ(mp[3], 3);
}

TEST(Suite_map_constructor, init_constructor_1) {
  S21_Map<int, int> mp = {{100, 200}};
  EXPECT_EQ(mp.size(), 1U);
  EXPECT_EQ(mp.empty(), false);
  EXPECT_EQ(mp[100], 200);
}

TEST(Suite_map_constructor, init_constructor_2) {
  S21_Map<int, int> mp = {{100, 200}, {-100, -3}, {33, 34}, {22, 1}};
  EXPECT_EQ(mp.size(), 4U);
  EXPECT_EQ(mp.empty(), false);
  EXPECT_EQ(mp[33], 34);
}

TEST(Suite_map_constructor, copy_constructor) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  S21_Map<int, int> mp2(mp);
  EXPECT_EQ(mp2.size(), 3U);
  EXPECT_EQ(mp2.empty(), false);
  EXPECT_EQ(mp2[1], 1);
  EXPECT_EQ(mp2[2], 2);
  EXPECT_EQ(mp2[3], 3);
}

TEST(Suite_map_constructor, copy_constructor_1) {
  S21_Map<int, int> mp = {{10, 10}};
  S21_Map<int, int> mp2(mp);
  EXPECT_EQ(mp2.size(), 1U);
  EXPECT_EQ(mp2.empty(), false);
  EXPECT_EQ(mp2[10], 10);
}

TEST(Suite_map_constructor, move_constructor) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  S21_Map<int, int> mp2(std::move(mp));
  EXPECT_EQ(mp2.size(), 3U);
  EXPECT_EQ(mp2.empty(), false);
  EXPECT_EQ(mp2[1], 1);
  EXPECT_EQ(mp2[2], 2);
  EXPECT_EQ(mp2[3], 3);

  EXPECT_EQ(mp.size(), 0U);
  EXPECT_EQ(mp.empty(), true);
}

TEST(Suite_map_constructor, move_constructor_1) {
  S21_Map<int, int> mp = {{33, 33}};
  S21_Map<int, int> mp2(std::move(mp));
  EXPECT_EQ(mp2.size(), 1U);
  EXPECT_EQ(mp2.empty(), false);
  EXPECT_EQ(mp2[33], 33);

  EXPECT_EQ(mp.size(), 0U);
  EXPECT_EQ(mp.empty(), true);
}

TEST(Suite_map_constructor, operator_eq) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  S21_Map<int, int> mp2 = std::move(mp);
  EXPECT_EQ(mp2.size(), 3U);
  EXPECT_EQ(mp2.empty(), false);
  EXPECT_EQ(mp2[1], 1);
  EXPECT_EQ(mp2[2], 2);
  EXPECT_EQ(mp2[3], 3);

  EXPECT_EQ(mp.size(), 0U);
  EXPECT_EQ(mp.empty(), true);
}

TEST(Suite_map_access, at) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};

  EXPECT_EQ(mp.at(1), 1);
  EXPECT_EQ(mp.at(2), 2);
  EXPECT_EQ(mp.at(3), 3);
}

TEST(Suite_map_access, at_1) {
  S21_Map<int, int> mp = {{4, 34}};

  EXPECT_EQ(mp.at(4), 34);
}

TEST(Suite_map_access, at_2) {
  S21_Map<int, int> mp = {{4, 34}};

  EXPECT_ANY_THROW(mp.at(5));
}

TEST(Suite_map_access, at_3) {
  S21_Map<int, int> mp;

  EXPECT_ANY_THROW(mp.at(5));
}

TEST(Suite_map_access, operator_brackets) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  EXPECT_EQ(mp[1], 1);
  EXPECT_EQ(mp[2], 2);
  EXPECT_EQ(mp[3], 3);
}

TEST(Suite_map_access, operator_brackets_1) {
  S21_Map<int, int> mp = {{4, 34}};

  EXPECT_EQ(mp[4], 34);
}

TEST(Suite_map_access, operator_brackets_2) {
  S21_Map<int, int> mp = {{4, 34}};

  mp[5] = 44;
  EXPECT_EQ(mp[5], 44);
}

TEST(Suite_map_iterators, begin) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};

  S21_Map<int, int>::iterator iter = mp.begin();
  std::pair<int, int> pair = *iter;
  EXPECT_EQ(pair.first, 1);
  EXPECT_EQ(pair.second, 1);
}

TEST(Suite_map_iterators, begin_1) {
  S21_Map<int, int> mp = {{3, 3}};

  S21_Map<int, int>::iterator iter = mp.begin();
  std::pair<int, int> pair = *iter;
  EXPECT_EQ(pair.first, 3);
  EXPECT_EQ(pair.second, 3);
}

TEST(Suite_map_iterators, begin_2) {
  S21_Map<int, int> mp;

  S21_Map<int, int>::iterator iter = mp.begin();
  EXPECT_EQ(iter, nullptr);
}

TEST(Suite_map_iterators, end) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};

  S21_Map<int, int>::iterator iter = mp.end();
  EXPECT_EQ(iter, nullptr);
}

TEST(Suite_map_iterators, end_1) {
  S21_Map<int, int> mp;

  S21_Map<int, int>::iterator iter = mp.end();
  EXPECT_EQ(iter, nullptr);
}

TEST(Suite_map_iterators, operator_star) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};

  S21_Map<int, int>::iterator iter = mp.begin();
  std::pair<int, int> pair = *iter;
  EXPECT_EQ(pair.first, 1);
  EXPECT_EQ(pair.second, 1);
}

TEST(Suite_map_iterators, operator_reference) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};

  S21_Map<int, int>::iterator iter = mp.begin();
  EXPECT_EQ(iter->first, 1);
  EXPECT_EQ(iter->second, 1);
}

TEST(Suite_map_iterators, operator_increment) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};

  S21_Map<int, int>::iterator iter = mp.begin();
  ++iter;
  std::pair<int, int> pair = *iter;
  EXPECT_EQ(pair.first, 2);
  EXPECT_EQ(pair.second, 2);
}

TEST(Suite_map_iterators, operator_decrement) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};

  S21_Map<int, int>::iterator iter = mp.begin();
  ++iter;
  --iter;
  std::pair<int, int> pair = *iter;
  EXPECT_EQ(pair.first, 1);
  EXPECT_EQ(pair.second, 1);
}

TEST(Suite_map_iterators, operator_equal) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};

  S21_Map<int, int>::iterator iter = mp.begin();
  S21_Map<int, int>::iterator iter2 = mp.begin();
  EXPECT_EQ(iter == iter2, true);
  ++iter;
  EXPECT_EQ(iter == iter2, false);
}

TEST(Suite_map_iterators, operator_not_equal) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};

  S21_Map<int, int>::iterator iter = mp.begin();
  S21_Map<int, int>::iterator iter2 = mp.begin();
  EXPECT_EQ(iter != iter2, false);
  ++iter;
  EXPECT_EQ(iter != iter2, true);
}

TEST(Suite_map_capacity, empty) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  EXPECT_EQ(mp.empty(), false);
}

TEST(Suite_map_capacity, empty_1) {
  S21_Map<int, int> mp;
  EXPECT_EQ(mp.empty(), true);
}

TEST(Suite_map_capacity, size) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  EXPECT_EQ(mp.size(), 3U);
}

TEST(Suite_map_capacity, size_1) {
  S21_Map<int, int> mp;
  EXPECT_EQ(mp.size(), 0U);
}

TEST(Suite_map_capacity, max_size) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  EXPECT_EQ(mp.max_size(), std::numeric_limits<std::size_t>::max() /
                               sizeof(std::pair<int, int>));
}

TEST(Suite_map_modifiers, clear) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  mp.clear();
  EXPECT_EQ(mp.size(), 0U);
}

TEST(Suite_map_modifiers, clear_1) {
  S21_Map<int, int> mp;
  mp.clear();
  EXPECT_EQ(mp.size(), 0U);
}

TEST(Suite_map_modifiers, insert_pair) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  std::pair<S21_Map<int, int>::iterator, bool> result =
      mp.insert(std::make_pair(5, 5));
  EXPECT_EQ(mp.size(), 4U);
  EXPECT_EQ(mp.at(5), 5);
  EXPECT_EQ(result.second, true);
  std::pair<int, int> pair = *result.first;
  EXPECT_EQ(pair.first, 5);
}

TEST(Suite_map_modifiers, insert_pair_1) {
  S21_Map<int, int> mp;
  std::pair<S21_Map<int, int>::iterator, bool> result =
      mp.insert(std::make_pair(5, 5));
  EXPECT_EQ(mp.size(), 1U);
  EXPECT_EQ(mp.at(5), 5);
  EXPECT_EQ(result.second, true);
  std::pair<int, int> pair = *result.first;
  EXPECT_EQ(pair.first, 5);
}

TEST(Suite_map_modifiers, insert_pair_2) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  std::pair<S21_Map<int, int>::iterator, bool> result =
      mp.insert(std::make_pair(3, 44));
  EXPECT_EQ(mp.size(), 3U);
  EXPECT_EQ(mp.at(3), 3);
  EXPECT_EQ(result.second, false);
  EXPECT_EQ(result.first, nullptr);
}

TEST(Suite_map_modifiers, insert_key_value) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  std::pair<S21_Map<int, int>::iterator, bool> result = mp.insert(5, 5);
  EXPECT_EQ(mp.size(), 4U);
  EXPECT_EQ(mp.at(5), 5);
  EXPECT_EQ(result.second, true);
  std::pair<int, int> pair = *result.first;
  EXPECT_EQ(pair.first, 5);
}

TEST(Suite_map_modifiers, insert_key_value_1) {
  S21_Map<int, int> mp;
  std::pair<S21_Map<int, int>::iterator, bool> result = mp.insert(5, 5);
  EXPECT_EQ(mp.size(), 1U);
  EXPECT_EQ(mp.at(5), 5);
  EXPECT_EQ(result.second, true);
  std::pair<int, int> pair = *result.first;
  EXPECT_EQ(pair.first, 5);
}

TEST(Suite_map_modifiers, insert_key_value_2) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  std::pair<S21_Map<int, int>::iterator, bool> result = mp.insert(3, 44);
  EXPECT_EQ(mp.size(), 3U);
  EXPECT_EQ(mp.at(3), 3);
  EXPECT_EQ(result.second, false);
  EXPECT_EQ(result.first, nullptr);
}

TEST(Suite_map_modifiers, insert_or_assign) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  std::pair<S21_Map<int, int>::iterator, bool> result =
      mp.insert_or_assign(5, 5);
  EXPECT_EQ(mp.size(), 4U);
  EXPECT_EQ(mp.at(5), 5);
  EXPECT_EQ(result.second, true);
  std::pair<int, int> pair = *result.first;
  EXPECT_EQ(pair.first, 5);
}

TEST(Suite_map_modifiers, insert_or_assign_1) {
  S21_Map<int, int> mp;
  std::pair<S21_Map<int, int>::iterator, bool> result =
      mp.insert_or_assign(5, 5);
  EXPECT_EQ(mp.size(), 1U);
  EXPECT_EQ(mp.at(5), 5);
  EXPECT_EQ(result.second, true);
  std::pair<int, int> pair = *result.first;
  EXPECT_EQ(pair.first, 5);
}

TEST(Suite_map_modifiers, insert_or_assign_2) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  std::pair<S21_Map<int, int>::iterator, bool> result =
      mp.insert_or_assign(3, 5);
  EXPECT_EQ(mp.size(), 3U);
  EXPECT_EQ(mp.at(3), 5);
  EXPECT_EQ(result.second, true);
  std::pair<int, int> pair = *result.first;
  EXPECT_EQ(pair.first, 3);
}

TEST(Suite_map_modifiers, erase) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  S21_Map<int, int>::iterator iter = mp.begin();
  mp.erase(iter);
  EXPECT_EQ(mp.size(), 2U);
  EXPECT_EQ(mp.contains(1), false);
}

TEST(Suite_map_modifiers, erase_1) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  S21_Map<int, int>::iterator iter = mp.begin();
  ++iter;
  mp.erase(iter);
  EXPECT_EQ(mp.size(), 2U);
  EXPECT_EQ(mp.contains(2), false);
}

TEST(Suite_map_modifiers, erase_2) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  S21_Map<int, int>::iterator iter = mp.begin();
  ++iter;
  ++iter;
  mp.erase(iter);
  EXPECT_EQ(mp.size(), 2U);
  EXPECT_EQ(mp.contains(3), false);
}

TEST(Suite_map_modifiers, erase_3) {
  S21_Map<int, int> mp;
  S21_Map<int, int>::iterator iter = mp.begin();
  EXPECT_ANY_THROW(mp.erase(iter););
}

TEST(Suite_map_modifiers, swap) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  S21_Map<int, int> mp2 = {{5, 5}, {6, 6}};

  mp.swap(mp2);

  EXPECT_EQ(mp.size(), 2U);
  EXPECT_EQ(mp.at(5), 5);
  EXPECT_EQ(mp.at(6), 6);

  EXPECT_EQ(mp2.size(), 3U);
  EXPECT_EQ(mp2.at(1), 1);
  EXPECT_EQ(mp2.at(2), 2);
  EXPECT_EQ(mp2.at(3), 3);
}

TEST(Suite_map_modifiers, merge) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  S21_Map<int, int> mp2 = {{5, 5}, {6, 6}};

  mp.merge(mp2);

  EXPECT_EQ(mp.size(), 5U);
  EXPECT_EQ(mp.at(5), 5);
  EXPECT_EQ(mp.at(6), 6);
  EXPECT_EQ(mp.at(1), 1);
  EXPECT_EQ(mp.at(2), 2);
  EXPECT_EQ(mp.at(3), 3);

  EXPECT_EQ(mp2.size(), 0U);
}

TEST(Suite_map_modifiers, merge_2) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  S21_Map<int, int> mp2 = {{5, 5}, {6, 6}, {3, 5}};

  mp.merge(mp2);

  EXPECT_EQ(mp.size(), 5U);
  EXPECT_EQ(mp.at(5), 5);
  EXPECT_EQ(mp.at(6), 6);
  EXPECT_EQ(mp.at(1), 1);
  EXPECT_EQ(mp.at(2), 2);
  EXPECT_EQ(mp.at(3), 3);

  EXPECT_EQ(mp2.size(), 1U);
  EXPECT_EQ(mp2.at(3), 5);
}

TEST(Suite_map_modifiers, insert_many) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  S21_Vector<std::pair<S21_Map<int, int>::iterator, bool>> result =
      mp.insert_many(std::make_pair(5, 5), std::make_pair(4, 4),
                     std::make_pair(6, 6));
  EXPECT_EQ(mp.size(), 6U);
  EXPECT_EQ(mp.at(5), 5);
  EXPECT_EQ(mp.at(4), 4);
  EXPECT_EQ(mp.at(6), 6);
  EXPECT_EQ(result[0].first->first, 5);
  EXPECT_EQ(result[0].second, true);
  EXPECT_EQ(result[1].first->first, 4);
  EXPECT_EQ(result[1].second, true);
  EXPECT_EQ(result[2].first->first, 6);
  EXPECT_EQ(result[2].second, true);
}

TEST(Suite_map_modifiers, insert_many_1) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  S21_Vector<std::pair<S21_Map<int, int>::iterator, bool>> result =
      mp.insert_many(std::make_pair(5, 5), std::make_pair(4, 4),
                     std::make_pair(3, 6));
  EXPECT_EQ(mp.size(), 5U);
  EXPECT_EQ(mp.at(5), 5);
  EXPECT_EQ(mp.at(4), 4);
  EXPECT_EQ(result[0].first->first, 5);
  EXPECT_EQ(result[0].second, true);
  EXPECT_EQ(result[1].first->first, 4);
  EXPECT_EQ(result[1].second, true);
  EXPECT_EQ(result[2].first, nullptr);
  EXPECT_EQ(result[2].second, false);
}

TEST(Suite_map_modifiers, insert_many_2) {
  S21_Map<int, int> mp;
  S21_Vector<std::pair<S21_Map<int, int>::iterator, bool>> result =
      mp.insert_many(std::make_pair(5, 5), std::make_pair(4, 4),
                     std::make_pair(3, 6));
  EXPECT_EQ(mp.size(), 3U);
  EXPECT_EQ(mp.at(5), 5);
  EXPECT_EQ(mp.at(4), 4);
  EXPECT_EQ(mp.at(3), 6);
  EXPECT_EQ(result[0].first->first, 5);
  EXPECT_EQ(result[0].second, true);
  EXPECT_EQ(result[1].first->first, 4);
  EXPECT_EQ(result[1].second, true);
  EXPECT_EQ(result[2].first->first, 3);
  EXPECT_EQ(result[2].second, true);
}

TEST(Suite_map_modifiers, insert_many_3) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  S21_Vector<std::pair<S21_Map<int, int>::iterator, bool>> result =
      mp.insert_many(std::make_pair(5, 5));
  EXPECT_EQ(mp.size(), 4U);
  EXPECT_EQ(mp.at(5), 5);
  EXPECT_EQ(result[0].first->first, 5);
  EXPECT_EQ(result[0].second, true);
}

TEST(Suite_map_modifiers, insert_many_4) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  S21_Vector<std::pair<S21_Map<int, int>::iterator, bool>> result =
      mp.insert_many();
  EXPECT_EQ(mp.size(), 3U);
  EXPECT_EQ(result[0].first, nullptr);
  EXPECT_EQ(result[0].second, false);
}

TEST(Suite_map_modifiers, insert_many_5) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};
  S21_Vector<std::pair<S21_Map<int, int>::iterator, bool>> result =
      mp.insert_many(std::make_pair(5, 5), std::make_pair(6, 6),
                     std::make_pair(7, 7), std::make_pair(8, 8),
                     std::make_pair(9, 9));
  EXPECT_EQ(mp.size(), 8U);
  EXPECT_EQ(result[0].first->first, 5);
  EXPECT_EQ(result[0].second, true);
  EXPECT_EQ(result[1].first->first, 6);
  EXPECT_EQ(result[1].second, true);
  EXPECT_EQ(result[2].first->first, 7);
  EXPECT_EQ(result[2].second, true);
  EXPECT_EQ(result[3].first->first, 8);
  EXPECT_EQ(result[3].second, true);
  EXPECT_EQ(result[4].first->first, 9);
  EXPECT_EQ(result[4].second, true);
}

TEST(Suite_map_modifiers, contains) {
  S21_Map<int, int> mp = {{1, 1}, {2, 2}, {3, 3}};

  EXPECT_EQ(mp.contains(1), true);
  EXPECT_EQ(mp.contains(2), true);
  EXPECT_EQ(mp.contains(3), true);
  EXPECT_EQ(mp.contains(0), false);
}