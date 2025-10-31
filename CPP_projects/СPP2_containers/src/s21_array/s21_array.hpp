#ifndef __S21_ARRAY_HPP__
#define __S21_ARRAY_HPP__
#include <iostream>

// using namespace std;
namespace s21 {

template <typename Type>
class ArrayIterator {
 public:
  using value_type = Type;
  using reference = Type &;
  using iterator = ArrayIterator;

  ArrayIterator(value_type *iter);

  reference operator*();
  iterator operator++();
  iterator operator++(int);
  iterator operator+(int n);
  iterator operator+=(int n);
  iterator operator--();
  iterator operator--(int);
  iterator operator-(int n);
  iterator operator-=(int n);

  bool operator!=(const iterator &another);
  bool operator==(const iterator &another);

 private:
  value_type *iter;
};

template <typename Type>
class ArrayConstIterator {
 public:
  using value_type = Type;
  using const_reference = const Type &;
  using const_iterator = const Type *;

  ArrayConstIterator(const value_type *iter);

  const_reference operator*() const;
  const_iterator operator++();
  const_iterator operator++(int);
  const_iterator operator+(int n);
  const_iterator operator+=(int n);
  const_iterator operator--();
  const_iterator operator--(int);
  const_iterator operator-(int n);
  const_iterator operator-=(int n);

  bool operator!=(const const_iterator &another);
  bool operator==(const const_iterator &another);

 private:
  const value_type *iter;
};

template <typename Type, size_t N>
class S21_array {
 public:
  /* Array Member type */

  using value_type = Type;
  using reference = Type &;
  using const_reference = const Type &;
  using iterator = Type *;
  using const_iterator = const Type *;
  using size_type = size_t;

  /* Array Member functions */

  S21_array();
  S21_array(std::initializer_list<value_type> const &items);
  S21_array(const S21_array &a);
  S21_array(S21_array &&a);
  ~S21_array();

  S21_array<Type, N> operator=(S21_array &&a);

  /* Array Element access */

  reference at(size_type pos);
  reference operator[](size_type pos);
  const_reference front();
  const_reference back();
  iterator data();

  /* Array Iterators */

  iterator begin();
  iterator end();

  /* Array Capacity */

  bool empty();
  size_type size();
  size_type max_size();

  /* Array Modifiers */

  void swap(S21_array &other);
  void fill(const_reference value);

 private:
  size_type size_ = N;
  value_type data_[N];
};

};  // namespace s21

#include "s21_array.tpp"

#endif
