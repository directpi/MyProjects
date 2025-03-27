#include "tests_run.hpp"

TEST(List_Suite_constructor, basic_constructor) {
  S21_List<int> lst;
  EXPECT_EQ(lst.size(), 0U);
}

TEST(List_Suite_constructor, basic_constructor_1) {
  S21_List<double> lst;
  EXPECT_EQ(lst.size(), 0U);
}

TEST(List_Suite_constructor, basic_constructor_2) {
  S21_List<char> lst;
  EXPECT_EQ(lst.size(), 0U);
}

TEST(List_Suite_constructor, size_constructor) {
  S21_List<int> lst(5);
  EXPECT_EQ(lst.size(), 5U);
}

TEST(List_Suite_constructor, size_constructor_1) {
  S21_List<int> lst(0);
  EXPECT_EQ(lst.size(), 0U);
}

TEST(List_Suite_constructor, size_constructor_2) {
  EXPECT_ANY_THROW(S21_List<int> lst(-5););
}

TEST(List_Suite_constructor, init_constructor) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  EXPECT_EQ(lst.size(), 5U);
  EXPECT_EQ(lst[0], 1);
  EXPECT_EQ(*lst.end(), 5);
}

TEST(List_Suite_constructor, init_constructor_1) {
  S21_List<int> lst = {5};
  EXPECT_EQ(lst.size(), 1U);
  EXPECT_EQ(lst[0], 5);
  EXPECT_EQ(*lst.end(), 5);
}

TEST(List_Suite_constructor, init_constructor_2) {
  S21_List<int> lst = {};
  EXPECT_EQ(lst.size(), 0U);
}

TEST(List_Suite_constructor, copy_constructor) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  S21_List<int> lst2(lst);
  EXPECT_EQ(lst2.size(), lst.size());
  EXPECT_EQ(lst2[0], lst[0]);
  EXPECT_EQ(*lst2.end(), *lst.end());
}

TEST(List_Suite_constructor, copy_constructor_1) {
  S21_List<int> lst = {1};
  S21_List<int> lst2(lst);
  EXPECT_EQ(lst2.size(), lst.size());
  EXPECT_EQ(lst2[0], lst[0]);
  EXPECT_EQ(*lst2.end(), *lst.end());
}

TEST(List_Suite_constructor, copy_constructor_2) {
  S21_List<int> lst = {};
  EXPECT_ANY_THROW(S21_List<int> lst2(lst););
}

TEST(List_Suite_constructor, move_constructor) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  S21_List<int> lst2(std::move(lst));
  EXPECT_EQ(lst2.size(), 5U);
  EXPECT_EQ(lst.size(), 0U);
  EXPECT_EQ(lst2[0], 1);
  EXPECT_EQ(*lst2.end(), 5);
}

TEST(List_Suite_constructor, move_constructor_1) {
  S21_List<int> lst = {1};
  S21_List<int> lst2(std::move(lst));
  EXPECT_EQ(lst2.size(), 1U);
  EXPECT_EQ(lst.size(), 0U);
  EXPECT_EQ(lst2[0], 1);
  EXPECT_EQ(*lst2.end(), 1);
}

TEST(List_Suite_constructor, move_constructor_2) {
  S21_List<int> lst = {};
  EXPECT_ANY_THROW(S21_List<int> lst2(std::move(lst)););
}

TEST(List_Suite_access, back) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  EXPECT_EQ(lst.back(), 5);
}

TEST(List_Suite_access, back_1) {
  S21_List<int> lst = {1};
  EXPECT_EQ(lst.back(), 1);
}

TEST(List_Suite_access, back_2) {
  S21_List<int> lst = {};
  EXPECT_ANY_THROW(lst.back());
}

TEST(List_Suite_access, front) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  EXPECT_EQ(lst.front(), 1);
}

TEST(List_Suite_access, front_1) {
  S21_List<int> lst = {1};
  EXPECT_EQ(lst.front(), 1);
}

TEST(List_Suite_access, front_2) {
  S21_List<int> lst = {};
  EXPECT_ANY_THROW(lst.front());
}

TEST(List_Suite_access, operator_access_i) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  EXPECT_EQ(lst[0], 1);
  EXPECT_EQ(lst[4], 5);
}

TEST(List_Suite_access, operator_access_i_1) {
  S21_List<int> lst = {1};
  EXPECT_EQ(lst[0], 1);
}

TEST(List_Suite_access, operator_access_i_2) {
  S21_List<int> lst = {};
  EXPECT_ANY_THROW(lst[0]);
}

TEST(List_Suite_access, operator_access_i_3) {
  S21_List<int> lst = {1, 2, 3};
  EXPECT_ANY_THROW(lst[5]);
}

TEST(List_Suite_iterator, begin) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  S21_List<int>::iterator it = lst.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
  --it;
  EXPECT_EQ(*it, 2);
}

TEST(List_Suite_iterator, end) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  S21_List<int>::iterator it = lst.end();
  EXPECT_EQ(*it, 5);
  it--;
  EXPECT_EQ(*it, 4);
  it--;
  EXPECT_EQ(*it, 3);
  it++;
  EXPECT_EQ(*it, 4);
}

TEST(List_Suite_iterator, equal) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  S21_List<int>::iterator it = lst.begin();
  S21_List<int>::iterator it2 = it;
  EXPECT_EQ(it == it2, true);
  ++it2;
  EXPECT_EQ(it == it2, false);
}

TEST(List_Suite_iterator, not_equal) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  S21_List<int>::iterator it = lst.begin();
  S21_List<int>::iterator it2 = it;
  EXPECT_EQ(it != it2, false);
  ++it2;
  EXPECT_EQ(it != it2, true);
}

TEST(List_Suite_const_iterator, cbegin) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  S21_List<int>::const_iterator it = lst.cbegin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
  --it;
  EXPECT_EQ(*it, 2);
}

TEST(List_Suite_const_iterator, cend) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  S21_List<int>::const_iterator it = lst.cend();
  EXPECT_EQ(*it, 5);
  it--;
  EXPECT_EQ(*it, 4);
  it--;
  EXPECT_EQ(*it, 3);
  it++;
  EXPECT_EQ(*it, 4);
}

TEST(List_Suite_const_iterator, equal) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  S21_List<int>::const_iterator it = lst.cbegin();
  S21_List<int>::const_iterator it2 = it;
  EXPECT_EQ(it == it2, true);
  ++it2;
  EXPECT_EQ(it == it2, false);
}

TEST(List_Suite_const_iterator, not_equal) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  S21_List<int>::const_iterator it = lst.cbegin();
  S21_List<int>::const_iterator it2 = it;
  EXPECT_EQ(it != it2, false);
  ++it2;
  EXPECT_EQ(it != it2, true);
}

TEST(List_Suite_capacity, empty) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  EXPECT_EQ(lst.empty(), false);
}

TEST(List_Suite_capacity, empty_1) {
  S21_List<int> lst;
  EXPECT_EQ(lst.empty(), true);
}

TEST(List_Suite_capacity, size) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  EXPECT_EQ(lst.size(), 5U);
}

TEST(List_Suite_capacity, size_1) {
  S21_List<int> lst;
  EXPECT_EQ(lst.size(), 0U);
}

TEST(List_Suite_capacity, size_2) {
  S21_List<int> lst = {1};
  EXPECT_EQ(lst.size(), 1U);
}

TEST(List_Suite_capacity, max_size) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  auto max_allocatable = std::numeric_limits<std::size_t>::max() / sizeof(int);
  EXPECT_EQ(lst.max_size(), max_allocatable);
}

TEST(List_Suite_modifiers, clear) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  lst.clear();
  EXPECT_EQ(lst.size(), 0U);
}

TEST(List_Suite_modifiers, clear_1) {
  S21_List<int> lst;
  lst.clear();
  EXPECT_EQ(lst.size(), 0U);
}

TEST(List_Suite_modifiers, push_back) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  lst.push_back(6);
  EXPECT_EQ(lst.back(), 6);
}

TEST(List_Suite_modifiers, push_back_1) {
  S21_List<int> lst = {1};
  lst.push_back(6);
  EXPECT_EQ(lst.back(), 6);
}

TEST(List_Suite_modifiers, push_back_2) {
  S21_List<int> lst = {};
  lst.push_back(6);
  EXPECT_EQ(lst.back(), 6);
}

TEST(List_Suite_modifiers, push_front) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  lst.push_front(6);
  EXPECT_EQ(lst.front(), 6);
}

TEST(List_Suite_modifiers, push_front_1) {
  S21_List<int> lst = {1};
  lst.push_front(6);
  EXPECT_EQ(lst.front(), 6);
}

TEST(List_Suite_modifiers, push_front_2) {
  S21_List<int> lst = {};
  lst.push_front(6);
  EXPECT_EQ(lst.front(), 6);
}

TEST(List_Suite_modifiers, pop_front) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  lst.pop_front();
  EXPECT_EQ(lst.front(), 2);
}

TEST(List_Suite_modifiers, pop_front_1) {
  S21_List<int> lst = {1};
  lst.pop_front();
  EXPECT_EQ(lst.size(), 0U);
}

TEST(List_Suite_modifiers, pop_front_2) {
  S21_List<int> lst;
  EXPECT_ANY_THROW(lst.pop_front(););
}

TEST(List_Suite_modifiers, pop_back) {
  S21_List<int> lst = {1, 2, 3, 4, 5};
  lst.pop_back();
  EXPECT_EQ(lst.back(), 4);
}

TEST(List_Suite_modifiers, pop_back_1) {
  S21_List<int> lst = {1};
  lst.pop_back();
  EXPECT_EQ(lst.size(), 0U);
}

TEST(List_Suite_modifiers, pop_back_2) {
  S21_List<int> lst;
  EXPECT_ANY_THROW(lst.pop_back(););
}

TEST(List_Suite_modifiers, insert) {
  S21_List<int> lst = {1, 2, 3, 4};
  S21_List<int>::iterator it = lst.begin();
  S21_List<int>::iterator res = lst.insert(it, 0);

  EXPECT_EQ(*res, 0);
  EXPECT_EQ(lst.front(), 0);
}

TEST(List_Suite_modifiers, insert_1) {
  S21_List<int> lst = {1, 2, 3, 4};
  S21_List<int>::iterator it = lst.begin();
  ++it;
  S21_List<int>::iterator res = lst.insert(it, 0);

  EXPECT_EQ(*res, 0);
  EXPECT_EQ(lst[1], 0);
}

TEST(List_Suite_modifiers, insert_2) {
  S21_List<int> lst = {1, 2, 3, 4};
  S21_List<int>::iterator it = lst.end();
  S21_List<int>::iterator res = lst.insert(it, 0);

  EXPECT_EQ(*res, 0);
  EXPECT_EQ(lst[3], 0);
}

TEST(List_Suite_modifiers, insert_3) {
  S21_List<int> lst;
  S21_List<int>::iterator it = lst.begin();
  EXPECT_ANY_THROW(lst.insert(it, 0););
}

TEST(List_Suite_modifiers, erase) {
  S21_List<int> lst = {1, 2, 3, 4};
  S21_List<int>::iterator it = lst.begin();
  lst.erase(it);
  EXPECT_EQ(lst.front(), 2);
}

TEST(List_Suite_modifiers, erase_1) {
  S21_List<int> lst = {1, 2, 3, 4};
  S21_List<int>::iterator it = lst.end();
  lst.erase(it);
  EXPECT_EQ(lst.back(), 3);
}

TEST(List_Suite_modifiers, erase_2) {
  S21_List<int> lst = {1};
  S21_List<int>::iterator it = lst.end();
  lst.erase(it);
  EXPECT_EQ(lst.size(), 0U);
}

TEST(List_Suite_modifiers, erase_3) {
  S21_List<int> lst;
  S21_List<int>::iterator it = lst.end();
  EXPECT_ANY_THROW(lst.erase(it););
}

TEST(List_Suite_modifiers, swap) {
  S21_List<int> lst = {1, 2, 3, 4};
  S21_List<int> lst2 = {33, 34, 35};
  lst.swap(lst2);
  EXPECT_EQ(lst.front(), 33);
  EXPECT_EQ(lst.size(), 3U);
  EXPECT_EQ(lst2.size(), 4U);
}

TEST(List_Suite_modifiers, unique) {
  S21_List<int> lst = {1, 1, 2, 2, 3, 4, 4};
  lst.unique();
  EXPECT_EQ(lst.size(), 4U);
}

TEST(List_Suite_modifiers, reverse) {
  S21_List<int> lst = {1, 2, 3, 4};
  lst.reverse();
  EXPECT_EQ(lst.front(), 4);
  EXPECT_EQ(lst.back(), 1);
}

TEST(List_Suite_modifiers, reverse_1) {
  S21_List<int> lst;
  lst.reverse();
  EXPECT_EQ(lst.size(), 0U);
}

TEST(List_Suite_modifiers, splice) {
  S21_List<int> lst = {1, 2, 3, 4};
  S21_List<int> lst2 = {10, 11, 12};
  S21_List<int>::iterator it = lst.begin();
  lst.splice(it, lst2);
  EXPECT_EQ(lst[0], 10);
  EXPECT_EQ(lst[1], 11);
  EXPECT_EQ(lst[2], 12);
  EXPECT_EQ(lst[3], 1);
}

TEST(List_Suite_modifiers, splice_1) {
  S21_List<int> lst = {1, 2, 3, 4};
  S21_List<int> lst2 = {10, 11, 12};
  S21_List<int>::iterator it = lst.end();
  lst.splice(it, lst2);
  EXPECT_EQ(lst[3], 10);
  EXPECT_EQ(lst[4], 11);
  EXPECT_EQ(lst[5], 12);
  EXPECT_EQ(lst[6], 4);
}

TEST(List_Suite_modifiers, splice_2) {
  S21_List<int> lst;
  S21_List<int> lst2 = {10, 11, 12};
  S21_List<int>::iterator it = lst.begin();
  EXPECT_ANY_THROW(lst.splice(it, lst2););
}

TEST(List_Suite_modifiers, sort) {
  S21_List<int> lst = {1, 2, 3, 4, 5, 6};
  lst.sort();
  EXPECT_EQ(lst[0], 1);
  EXPECT_EQ(lst[1], 2);
  EXPECT_EQ(lst[2], 3);
  EXPECT_EQ(lst[3], 4);
  EXPECT_EQ(lst[4], 5);
  EXPECT_EQ(lst[5], 6);
}

TEST(List_Suite_modifiers, sort_1) {
  S21_List<int> lst = {
      6, 5, 4, 3, 2, 1,
  };
  lst.sort();
  EXPECT_EQ(lst[0], 1);
  EXPECT_EQ(lst[1], 2);
  EXPECT_EQ(lst[2], 3);
  EXPECT_EQ(lst[3], 4);
  EXPECT_EQ(lst[4], 5);
  EXPECT_EQ(lst[5], 6);
}

TEST(List_Suite_modifiers, sort_2) {
  S21_List<int> lst = {
      6, 5, 4, 4, 5, 6,
  };
  lst.sort();
  EXPECT_EQ(lst[0], 4);
  EXPECT_EQ(lst[1], 4);
  EXPECT_EQ(lst[2], 5);
  EXPECT_EQ(lst[3], 5);
  EXPECT_EQ(lst[4], 6);
  EXPECT_EQ(lst[5], 6);
}

TEST(List_Suite_modifiers, sort_3) {
  S21_List<int> lst = {4, 3, 2, 1, 2, 3, 4};
  lst.sort();
  EXPECT_EQ(lst[0], 1);
  EXPECT_EQ(lst[1], 2);
  EXPECT_EQ(lst[2], 2);
  EXPECT_EQ(lst[3], 3);
  EXPECT_EQ(lst[4], 3);
  EXPECT_EQ(lst[5], 4);
  EXPECT_EQ(lst[6], 4);
}

TEST(List_Suite_modifiers, sort_4) {
  S21_List<int> lst = {1};
  lst.sort();
  EXPECT_EQ(lst[0], 1);
}

TEST(List_Suite_modifiers, sort_5) {
  S21_List<int> lst = {10, 1};
  lst.sort();
  EXPECT_EQ(lst[0], 1);
  EXPECT_EQ(lst[1], 10);
}

TEST(List_Suite_modifiers, sort_6) {
  S21_List<int> lst;
  lst.sort();
  EXPECT_EQ(lst.size(), 0U);
}

TEST(List_Suite_modifiers, merge) {
  S21_List<int> lst = {1, 2, 3};
  S21_List<int> lst2 = {4, 5, 6};
  lst.merge(lst2);
  EXPECT_EQ(lst[0], 1);
  EXPECT_EQ(lst[1], 2);
  EXPECT_EQ(lst[2], 3);
  EXPECT_EQ(lst[3], 4);
  EXPECT_EQ(lst[4], 5);
  EXPECT_EQ(lst[5], 6);
}

TEST(List_Suite_modifiers, merge_1) {
  S21_List<int> lst = {1, 2, 3};
  S21_List<int> lst2 = {1, 2, 3};
  lst.merge(lst2);
  EXPECT_EQ(lst[0], 1);
  EXPECT_EQ(lst[1], 1);
  EXPECT_EQ(lst[2], 2);
  EXPECT_EQ(lst[3], 2);
  EXPECT_EQ(lst[4], 3);
  EXPECT_EQ(lst[5], 3);
}

TEST(List_Suite_modifiers, merge_2) {
  S21_List<int> lst = {4, 5, 6};
  S21_List<int> lst2 = {1, 2, 3};
  lst.merge(lst2);
  EXPECT_EQ(lst[0], 1);
  EXPECT_EQ(lst[1], 2);
  EXPECT_EQ(lst[2], 3);
  EXPECT_EQ(lst[3], 4);
  EXPECT_EQ(lst[4], 5);
  EXPECT_EQ(lst[5], 6);
}

TEST(List_Suite_modifiers, merge_3) {
  S21_List<int> lst;
  ;
  S21_List<int> lst2 = {1, 2, 3};
  lst.merge(lst2);
  EXPECT_EQ(lst[0], 1);
  EXPECT_EQ(lst[1], 2);
  EXPECT_EQ(lst[2], 3);
}

TEST(List_Suite_modifiers, merge_4) {
  S21_List<int> lst = {1, 2, 3};
  S21_List<int> lst2;
  lst.merge(lst2);
  EXPECT_EQ(lst[0], 1);
  EXPECT_EQ(lst[1], 2);
  EXPECT_EQ(lst[2], 3);
}

TEST(List_Suite_modifiers, insertAt) {
  S21_List<int> lst = {1, 2, 3, 4};
  lst.insertAt(10, 0);
  EXPECT_EQ(lst[0], 10);
}

TEST(List_Suite_modifiers, insertAt_1) {
  S21_List<int> lst;
  lst.insertAt(10, 0);
  EXPECT_EQ(lst[0], 10);
}

TEST(List_Suite_modifiers, removeAt) {
  S21_List<int> lst = {1, 2, 3, 4};
  lst.removeAt(3);
  EXPECT_EQ(lst.size(), 3U);
  EXPECT_EQ(lst[2], 3);
}

TEST(List_Suite_modifiers, removeAt_1) {
  S21_List<int> lst = {1, 2, 3, 4};
  lst.removeAt(1);
  EXPECT_EQ(lst.size(), 3U);
  EXPECT_EQ(lst[1], 3);
}

TEST(List_Suite_modifiers, removeAt_2) {
  S21_List<int> lst = {1, 2, 3, 4};
  lst.removeAt(0);
  EXPECT_EQ(lst.size(), 3U);
  EXPECT_EQ(lst[1], 3);
}

TEST(List_Suite_modifiers, operatorEQ) {
  S21_List<int> lst = {1, 2, 3, 4};
  S21_List<int> lst2 = std::move(lst);
  EXPECT_EQ(lst.size(), 0U);
  EXPECT_EQ(lst2[0], 1);
}

TEST(List_Suite_modifiers, insert_many_back) {
  S21_List<int> lst = {1, 2, 3, 4};
  lst.insert_many_back(4, 5, 6);
  EXPECT_EQ(lst.size(), 7U);
  EXPECT_EQ(lst[6], 6);
}

TEST(List_Suite_modifiers, insert_many_back_1) {
  S21_List<int> lst;
  lst.insert_many_back(4, 5, 6);
  EXPECT_EQ(lst.size(), 3U);
  EXPECT_EQ(lst[2], 6);
}

TEST(List_Suite_modifiers, insert_many_back_2) {
  S21_List<int> lst;
  lst.insert_many_back(4, 5, 6, 6, 1);
  EXPECT_EQ(lst.size(), 5U);
  EXPECT_EQ(lst[2], 6);
}

TEST(List_Suite_modifiers, insert_many_back_3) {
  S21_List<int> lst = {1, 2, 3, 4};
  lst.insert_many_back();
  EXPECT_EQ(lst.size(), 4U);
}

TEST(List_Suite_modifiers, insert_many_front) {
  S21_List<int> lst = {1, 2, 3, 4};
  lst.insert_many_front(4, 5, 6);
  EXPECT_EQ(lst.size(), 7U);
  EXPECT_EQ(lst[0], 4);
}

TEST(List_Suite_modifiers, insert_many_front_1) {
  S21_List<int> lst;
  ;
  lst.insert_many_front(4, 5, 6);
  EXPECT_EQ(lst.size(), 3U);
  EXPECT_EQ(lst[2], 6);
}

TEST(List_Suite_modifiers, insert_many_front_2) {
  S21_List<int> lst;
  lst.insert_many_front(4, 5, 6, 6, 1);
  EXPECT_EQ(lst.size(), 5U);
  EXPECT_EQ(lst[2], 6);
}

TEST(List_Suite_modifiers, insert_many_front_3) {
  S21_List<int> lst = {1, 2, 3, 4};
  lst.insert_many_front();
  EXPECT_EQ(lst.size(), 4U);
}

TEST(List_Suite_modifiers, insert_many) {
  S21_List<int> lst = {1, 2, 3, 4};
  S21_List<int>::const_iterator iter = lst.cbegin();
  lst.insert_many(iter, 4, 5, 6);
  EXPECT_EQ(lst.size(), 7U);
  EXPECT_EQ(lst[0], 4);
}

TEST(List_Suite_modifiers, insert_many_1) {
  S21_List<int> lst = {1, 2, 3, 4};
  S21_List<int>::const_iterator iter = lst.cbegin();
  ++iter;
  lst.insert_many(iter, 4, 5, 6);
  EXPECT_EQ(lst.size(), 7U);
  EXPECT_EQ(lst[1], 4);
}

TEST(List_Suite_modifiers, insert_many_2) {
  S21_List<int> lst = {1, 2, 3, 4};
  S21_List<int>::const_iterator iter = lst.cend();
  lst.insert_many(iter, 9, 5, 6);
  EXPECT_EQ(lst.size(), 7U);
  EXPECT_EQ(lst[3], 9);
}

TEST(List_Suite_modifiers, insert_many_3) {
  S21_List<int> lst = {1, 2, 3, 4};
  S21_List<int>::const_iterator iter = lst.cend();
  lst.insert_many(iter);
  EXPECT_EQ(lst.size(), 4U);
}

TEST(List_Suite_modifiers, insert_many_4) {
  S21_List<int> lst = {1, 2, 3, 4};
  S21_List<int>::const_iterator iter = lst.cend();
  lst.insert_many(iter, 5);
  EXPECT_EQ(lst.size(), 5U);
  EXPECT_EQ(lst[3], 5);
}

TEST(List_Suite_modifiers, insert_many_5) {
  S21_List<int> lst = {1, 2, 3, 4};
  S21_List<int>::const_iterator iter = lst.cend();
  lst.insert_many(iter, 5, 6, 7, 8, 9);
  EXPECT_EQ(lst.size(), 9U);
  EXPECT_EQ(lst[8], 4);
}