#ifndef __S21_LIST_HPP__
#define __S21_LIST_HPP__

#include <iostream>
namespace s21 {

template <typename Type>
class Node {
 public:
  Node(Type data = Type(), Node *pNext = nullptr, Node *pPrev = nullptr) {
    this->data = data;
    this->pNext = pNext;
    this->pPrev = pPrev;
  }
  Node *pNext;
  Node *pPrev;
  Type data;
};

template <typename Type>
class ListIterator {
 private:
  Node<Type> *iter;

 public:
  using value_type = Type;
  using reference = Type &;
  using iterator = ListIterator;

  ListIterator(Node<value_type> *iter);
  reference operator*();
  iterator operator++(int);
  iterator operator--(int);
  iterator operator++();
  iterator operator--();
  bool operator!=(const iterator &Iterator);
  bool operator==(const iterator &Iterator);
  Node<value_type> *get_node();
};

template <typename Type>
class ListConstIterator {
 private:
  const Node<Type> *iter;

 public:
  using value_type = Type;
  using const_reference = const Type &;
  using const_iterator = ListConstIterator;

  ListConstIterator(const Node<value_type> *iter);
  const_reference operator*() const;
  const_iterator operator++(int);
  const_iterator operator--(int);
  const_iterator operator++();
  const_iterator operator--();
  bool operator!=(const const_iterator &Iterator);
  bool operator==(const const_iterator &Iterator);
};

template <typename Type>
class S21_List {
 public:
  // member types

  using value_type = Type;
  using reference = Type &;
  using const_reference = const Type &;
  using iterator = ListIterator<Type>;
  using const_iterator = ListConstIterator<Type>;
  using size_type = size_t;

  // constructors and destructor

  S21_List();
  S21_List(size_type n);
  S21_List(std::initializer_list<Type> const &items);
  S21_List(const S21_List &another);
  S21_List(S21_List &&another);
  ~S21_List();

  // List Element access

  const_reference back();
  const_reference front();
  reference operator[](const size_type index);

  // List Iterators

  iterator begin();
  iterator end();
  const_iterator cbegin();
  const_iterator cend();

  // List Capacity

  bool empty();
  size_type size();
  size_type max_size();

  // List Modifiers

  void clear();
  void copy(const S21_List &another);
  void move(S21_List &&another);
  void push_back(const_reference value);
  void push_front(const_reference value);
  void pop_front();
  void pop_back();
  iterator insert(iterator pos, const_reference value);
  void erase(iterator pos);
  void swap(S21_List &another);
  void unique();
  void reverse();
  void splice(iterator pos, S21_List another);
  void sort();
  void merge(S21_List &another);
  void insertAt(value_type data, size_type index);
  void removeAt(size_type index);
  void operator=(S21_List &&another);

  template <typename... Args>
  void insert_many_back(Args &&...args);
  template <typename... Args>
  void insert_many_front(Args &&...args);
  template <typename... Args>
  iterator insert_many(const_iterator pos, Args &&...args);

 private:
  Node<value_type> *head;
  Node<value_type> *tail;
  size_type Size;

  void quick_sort(iterator left, iterator right);
  iterator get_c_element(iterator left, iterator right, size_type *counter);
  void swap_nodes(iterator &left_tmp, iterator &right_tmp, iterator &left,
                  iterator &right);
};

};  // namespace s21

#include "s21_list.tpp"

#endif