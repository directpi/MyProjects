#ifndef __S21_VECTOR_HPP__
#define __S21_VECTOR_HPP__

#include <iostream>
using namespace std;

namespace s21 {

template <typename Type>
class VectorIterator {
 public:
  using value_type = Type;
  using reference = Type&;
  using iterator = VectorIterator;
  using size_type = size_t;

  VectorIterator(value_type* iter);
  iterator operator+(int n);
  iterator operator+=(int n);
  iterator operator-(int n);
  iterator operator-=(int n);
  iterator operator++(int);
  iterator operator--(int);
  iterator operator++();
  iterator operator--();
  reference operator*();
  size_type operator-(iterator another);
  size_type operator+(iterator another);

  bool operator!=(const iterator& another);
  bool operator==(const iterator& another);
  bool operator>(const iterator& another);
  bool operator>=(const iterator& another);
  bool operator<(const iterator& another);
  bool operator<=(const iterator& another);

 private:
  value_type* iter;
};

template <typename Type>
class VectorConstIterator {
 public:
  using value_type = Type;
  using const_reference = const Type&;
  using const_iterator = VectorConstIterator;

  VectorConstIterator(const value_type* iter);
  const_iterator operator+(int n);
  const_iterator operator+=(int n);
  const_iterator operator-(int n);
  const_iterator operator-=(int n);
  const_iterator operator++(int);
  const_iterator operator--(int);
  const_iterator operator++();
  const_iterator operator--();
  const_reference operator*() const;

  bool operator!=(const const_iterator& another);
  bool operator==(const const_iterator& another);
  bool operator>(const const_iterator& another);
  bool operator>=(const const_iterator& another);
  bool operator<(const const_iterator& another);
  bool operator<=(const const_iterator& another);

 private:
  const value_type* iter;
};

template <typename Type>
class S21_Vector {
 public:
  // member types

  using value_type = Type;
  using reference = Type&;
  using const_reference = const Type&;
  using iterator = VectorIterator<Type>;
  using const_iterator = VectorConstIterator<Type>;
  using size_type = size_t;

  // Vector Member functions

  S21_Vector();
  S21_Vector(size_type n);
  S21_Vector(std::initializer_list<Type> const& items);
  S21_Vector(S21_Vector& another);
  S21_Vector(S21_Vector&& another);
  ~S21_Vector();

  // Vector Element access

  reference at(size_type index);
  reference operator[](size_type index);
  const_reference front();
  const_reference back();

  /*direct access to the underlying array*/
  value_type* data() { return ptr; }

  // Vector VectorIterators

  iterator begin();
  iterator end();
  iterator rbegin();
  iterator rend();
  const_iterator cbegin();
  const_iterator cend();
  const_iterator crbegin();
  const_iterator crend();

  // Vector Capacity
  /*checks whether the container is empty*/
  bool empty() { return ptr == nullptr ? 1 : 0; }
  /*returns the number of elements*/
  size_type size() { return Size; }
  size_type max_size();
  void reserve(size_type n);
  /*returns the number of elements that can be held in currently allocated
   * storage*/
  size_type capacity() { return Capacity; }
  void shrink_to_fit();

  // Vector Modifiers

  void clear();
  iterator insert(iterator pos, const_reference value);
  void erase(iterator pos);
  void push_back(const_reference element);
  void pop_back();
  void swap(S21_Vector& another);
  void copy(S21_Vector& another);
  void move(S21_Vector&& another);
  void operator=(S21_Vector&& another) { this->move(std::move(another)); }

  template <typename... Args>
  void insert_many_back(Args&&... args);
  template <typename... Args>
  iterator insert_many(const_iterator pos, Args&&... args);

 private:
  size_type Size;
  size_type Capacity;
  value_type* ptr;
};

};  // namespace s21

#include "s21_vector.tpp"

#endif