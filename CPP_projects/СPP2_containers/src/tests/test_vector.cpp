#include "tests_run.hpp"

TEST(Suite_vector_constructor, basic_constructor) {
  S21_Vector<int> vct;
  EXPECT_EQ(vct.size(), 0U);
  EXPECT_EQ(vct.capacity(), 0U);
  EXPECT_EQ(vct.data(), nullptr);
}

TEST(Suite_vector_constructor, basic_constructor_1) {
  S21_Vector<double> vct;
  EXPECT_EQ(vct.size(), 0U);
  EXPECT_EQ(vct.capacity(), 0U);
  EXPECT_EQ(vct.data(), nullptr);
}

TEST(Suite_vector_constructor, basic_constructor_2) {
  S21_Vector<char> vct;
  EXPECT_EQ(vct.size(), 0U);
  EXPECT_EQ(vct.capacity(), 0U);
  EXPECT_EQ(vct.data(), nullptr);
}

TEST(Suite_vector_constructor, size_constructor) {
  S21_Vector<int> vct(5);
  EXPECT_EQ(vct.size(), 5U);
  EXPECT_EQ(vct.capacity(), 5U);
  EXPECT_NE(vct.data(), nullptr);
}

TEST(Suite_vector_constructor, init_constructor) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  EXPECT_EQ(vct.size(), 5U);
  EXPECT_EQ(vct.capacity(), 5U);
  EXPECT_EQ(vct[0], 1);
  EXPECT_EQ(vct[4], 5);
}

TEST(Suite_vector_constructor, copy_constructor) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int> another(vct);

  EXPECT_EQ(vct.size(), another.size());
  EXPECT_EQ(vct.capacity(), another.capacity());
  EXPECT_EQ(vct[0], another[0]);
  EXPECT_EQ(vct[4], another[4]);
}

TEST(Suite_vector_constructor, copy_constructor_2) {
  S21_Vector<int> vct;
  EXPECT_ANY_THROW(S21_Vector<int> another(vct););
}

TEST(Suite_vector_constructor, move_constructor) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int> another(std::move(vct));

  EXPECT_EQ(another.size(), 5U);
  EXPECT_EQ(another.capacity(), 5U);
  EXPECT_EQ(another[0], 1);
  EXPECT_EQ(another[4], 5);
  EXPECT_EQ(vct.data(), nullptr);
}

TEST(Suite_vector_constructor, move_constructor_2) {
  S21_Vector<int> vct;
  EXPECT_ANY_THROW(S21_Vector<int> another(std::move(vct)););
}

TEST(Suite_vector_access, access_at) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  EXPECT_EQ(vct.at(0), 1);
  EXPECT_EQ(vct.at(1), 2);
  EXPECT_EQ(vct.at(4), 5);
}

TEST(Suite_vector_access, access_at_1) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  EXPECT_ANY_THROW(vct.at(-1));
  EXPECT_ANY_THROW(vct.at(6));
}

TEST(Suite_vector_access, access_at_2) {
  S21_Vector<int> vct;
  EXPECT_ANY_THROW(vct.at(0));
}

TEST(Suite_vector_access, access_operatorEQ) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  EXPECT_EQ(vct[0], 1);
  EXPECT_EQ(vct[1], 2);
  EXPECT_EQ(vct[4], 5);
}

TEST(Suite_vector_access, access_operatorEQ_1) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  EXPECT_ANY_THROW(vct[-1]);
  EXPECT_ANY_THROW(vct[6]);
}

TEST(Suite_vector_access, access_operatorEQ_3) {
  S21_Vector<int> vct;
  EXPECT_ANY_THROW(vct[0]);
}

TEST(Suite_vector_access, access_front) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  EXPECT_EQ(vct.front(), 1);
}

TEST(Suite_vector_access, access_front_1) {
  S21_Vector<int> vct = {33};
  EXPECT_EQ(vct.front(), 33);
}

TEST(Suite_vector_access, access_front_2) {
  S21_Vector<int> vct;
  EXPECT_ANY_THROW(vct.front(););
}

TEST(Suite_vector_access, access_back) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  EXPECT_EQ(vct.back(), 5);
}

TEST(Suite_vector_access, access_back_1) {
  S21_Vector<int> vct = {33};
  EXPECT_EQ(vct.back(), 33);
}

TEST(Suite_vector_access, access_back_2) {
  S21_Vector<int> vct;
  EXPECT_ANY_THROW(vct.back(););
}

TEST(Suite_vector_access, access_data) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  int* a = vct.data();
  EXPECT_EQ(*a, 1);
}

TEST(Suite_vector_capacity, access_empty) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  EXPECT_EQ(vct.empty(), 0);
}

TEST(Suite_vector_capacity, access_empty_1) {
  S21_Vector<int> vct;
  EXPECT_EQ(vct.empty(), 1);
}

TEST(Suite_vector_capacity, access_size) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  EXPECT_EQ(vct.size(), 5U);
}

TEST(Suite_vector_capacity, access_size_1) {
  S21_Vector<int> vct;
  EXPECT_EQ(vct.size(), 0U);
}

TEST(Suite_vector_capacity, access_max_size) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  auto max_allocatable = std::numeric_limits<std::size_t>::max() / sizeof(int);
  EXPECT_EQ(vct.max_size(), max_allocatable);
}

TEST(Suite_vector_capacity, access_max_size_1) {
  S21_Vector<double> vct = {1.0, 2.0, 3.0, 4.0, 5.0};
  auto max_allocatable =
      std::numeric_limits<std::size_t>::max() / sizeof(double);
  EXPECT_EQ(vct.max_size(), max_allocatable);
}

TEST(Suite_vector_capacity, access_reserve) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  vct.reserve(10);
  EXPECT_EQ(vct.capacity(), 10U);
}

TEST(Suite_vector_capacity, access_capacity) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  EXPECT_EQ(vct.capacity(), 5U);
}

TEST(Suite_vector_capacity, access_shrink_to_fit) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  vct.push_back(6);
  EXPECT_EQ(vct.capacity(), 10U);
  vct.shrink_to_fit();
  EXPECT_EQ(vct.capacity(), 6U);
}

TEST(Suite_vector_iterators, iterators_begin) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.begin();
  EXPECT_EQ(*it, 1);
}

TEST(Suite_vector_iterators, iterators_end) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.end();
  EXPECT_EQ(*(it - 1), 5);
}

TEST(Suite_vector_iterators, iterators_rbegin) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.rbegin();
  EXPECT_EQ(*it, 5);
}

TEST(Suite_vector_iterators, iterators_rend) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.rend();
  EXPECT_EQ(*(it + 1), 1);
}

TEST(Suite_vector_iterators, iterators_plus) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.begin();
  EXPECT_EQ(*(it + 1), 2);
}

TEST(Suite_vector_iterators, iterators_minus) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.end();
  EXPECT_EQ(*(it - 2), 4);
}

TEST(Suite_vector_iterators, iterators_p_eq) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.begin();
  it += 2;
  EXPECT_EQ(*(it), 3);
}

TEST(Suite_vector_iterators, iterators_m_eq) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.end();
  it -= 2;
  EXPECT_EQ(*(it), 4);
}

TEST(Suite_vector_iterators, iterators_increment) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.begin();
  it++;
  EXPECT_EQ(*(it), 2);
}

TEST(Suite_vector_iterators, iterators_increment_2) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.begin();
  ++it;
  EXPECT_EQ(*(it), 2);
}

TEST(Suite_vector_iterators, iterators_decriment) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.end();
  it--;
  EXPECT_EQ(*(it), 5);
}

TEST(Suite_vector_iterators, iterators_decriment_2) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.end();
  --it;
  EXPECT_EQ(*(it), 5);
}

TEST(Suite_vector_iterators, iterators_eq) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.begin();
  S21_Vector<int>::iterator it_2 = vct.begin();
  EXPECT_EQ(it == it_2, true);
}

TEST(Suite_vector_iterators, iterators_n_eq) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.begin();
  S21_Vector<int>::iterator it_2 = vct.begin();
  it_2++;
  EXPECT_EQ(it == it_2, false);
}

TEST(Suite_vector_iterators, iterators_greater) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.begin();
  S21_Vector<int>::iterator it_2 = vct.begin();
  it_2++;
  EXPECT_EQ(it_2 > it, true);
}

TEST(Suite_vector_iterators, iterators_g_eq) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.begin();
  S21_Vector<int>::iterator it_2 = vct.begin();
  EXPECT_EQ(it_2 >= it, true);
}

TEST(Suite_vector_iterators, iterators_less) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.begin();
  S21_Vector<int>::iterator it_2 = vct.begin();
  it_2++;
  EXPECT_EQ(it < it_2, true);
}

TEST(Suite_vector_iterators, iterators_l_eq) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.begin();
  S21_Vector<int>::iterator it_2 = vct.begin();
  EXPECT_EQ(it_2 <= it, true);
}

TEST(Suite_vector_modifiers, clear) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  vct.clear();
  EXPECT_EQ(vct.size(), 0U);
}

TEST(Suite_vector_modifiers, insert) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.begin();
  vct.insert(it, 55);
  EXPECT_EQ(vct[0], 55);
}

TEST(Suite_vector_modifiers, insert_1) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.end() - 1;
  vct.insert(it, 55);
  EXPECT_EQ(vct[4], 55);
}

TEST(Suite_vector_modifiers, insert_2) {
  S21_Vector<int> vct = {1};
  S21_Vector<int>::iterator it = vct.begin();
  vct.insert(it, 55);
  EXPECT_EQ(vct[0], 55);
}

TEST(Suite_vector_modifiers, insert_3) {
  S21_Vector<int> vct;
  S21_Vector<int>::iterator it = vct.begin();
  vct.insert(it, 55);
  EXPECT_EQ(vct[0], 55);
}

TEST(Suite_vector_modifiers, erase) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.begin();
  vct.erase(it);
  EXPECT_EQ(vct[0], 2);
}

TEST(Suite_vector_modifiers, erase_1) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.begin() + 4;
  vct.erase(it);
  EXPECT_EQ(vct[3], 4);
  EXPECT_EQ(vct.size(), 4U);
}

TEST(Suite_vector_modifiers, erase_2) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  S21_Vector<int>::iterator it = vct.begin() + 2;
  vct.erase(it);
  EXPECT_EQ(vct[2], 4);
}

TEST(Suite_vector_modifiers, erase_3) {
  S21_Vector<int> vct;
  S21_Vector<int>::iterator it = vct.begin();
  EXPECT_ANY_THROW(vct.erase(it););
}

TEST(Suite_vector_modifiers, push_back) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  vct.push_back(6);
  EXPECT_EQ(vct[5], 6);
}

TEST(Suite_vector_modifiers, push_back_1) {
  S21_Vector<int> vct;
  vct.push_back(6);
  EXPECT_EQ(vct[0], 6);
}

TEST(Suite_vector_modifiers, pop_back) {
  S21_Vector<int> vct = {1, 2, 3, 4, 5};
  vct.pop_back();
  EXPECT_EQ(vct[3], 4);
  EXPECT_EQ(vct.size(), 4U);
}

TEST(Suite_vector_modifiers, pop_back_1) {
  S21_Vector<int> vct;
  EXPECT_ANY_THROW(vct.pop_back(););
}

TEST(Suite_vector_modifiers, swap) {
  S21_Vector<int> vct = {1, 2, 3};
  S21_Vector<int> another = {4, 5, 6, 7};
  vct.swap(another);
  EXPECT_EQ(vct[0], 4);
  EXPECT_EQ(another[0], 1);
}

TEST(Suite_vector_modifiers, operator_eq) {
  S21_Vector<int> vct = {1, 2, 3};
  S21_Vector<int> another = vct;
  EXPECT_EQ(another[0], 1);
  EXPECT_EQ(another.size(), 3U);
}

TEST(Suite_vector_modifiers, insert_many_back) {
  S21_Vector<int> vct = {1, 2, 3};
  vct.insert_many_back(4, 5, 6, 7, 8);
  EXPECT_EQ(vct[3], 4);
  EXPECT_EQ(vct.size(), 8U);
}

TEST(Suite_vector_modifiers, insert_many_back_1) {
  S21_Vector<int> vct = {1, 2, 3};
  vct.insert_many_back(4, 5, 6);
  EXPECT_EQ(vct[3], 4);
  EXPECT_EQ(vct.size(), 6U);
}

TEST(Suite_vector_modifiers, insert_many_back_2) {
  S21_Vector<int> vct = {1, 2, 3};
  vct.insert_many_back(4);
  EXPECT_EQ(vct[3], 4);
  EXPECT_EQ(vct.size(), 4U);
}

TEST(Suite_vector_modifiers, insert_many_back_3) {
  S21_Vector<int> vct = {1, 2, 3};
  vct.insert_many_back();
  EXPECT_EQ(vct.size(), 3U);
}

TEST(Suite_vector_modifiers, insert_many) {
  S21_Vector<int> vct = {1, 2, 3};
  S21_Vector<int>::const_iterator it = vct.cbegin();
  vct.insert_many(it, 4, 5, 6, 7, 8);
  EXPECT_EQ(vct[0], 4);
  EXPECT_EQ(vct[3], 7);
  EXPECT_EQ(vct[5], 1);
  EXPECT_EQ(vct.size(), 8U);
}

TEST(Suite_vector_modifiers, insert_many_1) {
  S21_Vector<int> vct = {1, 2, 3};
  S21_Vector<int>::const_iterator it = vct.cend();
  EXPECT_ANY_THROW(vct.insert_many(it, 4, 5, 6, 7););
}

TEST(Suite_vector_modifiers, insert_many_2) {
  S21_Vector<int> vct = {1, 2, 3};
  S21_Vector<int>::const_iterator it = vct.cbegin();
  vct.insert_many(it, 4, 5, 6);
  EXPECT_EQ(vct[0], 4);
  EXPECT_EQ(vct[2], 6);
  EXPECT_EQ(vct[3], 1);
  EXPECT_EQ(vct.size(), 6U);
}

TEST(Suite_vector_modifiers, insert_many_3) {
  S21_Vector<int> vct = {1, 2, 3};
  S21_Vector<int>::const_iterator it = vct.cbegin();
  vct.insert_many(it, 4);
  EXPECT_EQ(vct[0], 4);
  EXPECT_EQ(vct[1], 1);
  EXPECT_EQ(vct.size(), 4U);
}

TEST(Suite_vector_modifiers, insert_many_4) {
  S21_Vector<int> vct = {1, 2, 3};
  S21_Vector<int>::const_iterator it = vct.cbegin();
  vct.insert_many(it);
  EXPECT_EQ(vct.size(), 3U);
}