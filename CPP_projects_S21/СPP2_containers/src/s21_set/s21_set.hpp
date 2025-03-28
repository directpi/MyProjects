#ifndef __S21_SET_HPP__
#define __S21_SET_HPP__
#include <iostream>
#include <limits>

#include "../s21_vector/s21_vector.hpp"
#define RED 2
#define BLACK 1

using namespace std;
namespace s21 {
template <typename Key>
class S21_Set {
 private:
  template <typename>
  class Node {
   public:
    Node(Key data, Node *pLeft = nullptr, Node *pRight = nullptr,
         Node *parent = nullptr, int color = RED) {
      this->data = data;
      this->parent = parent;
      this->pLeft = pLeft;
      this->pRight = pRight;
      this->color = color;
    }
    Node<Key> *pLeft;
    Node<Key> *pRight;
    Node<Key> *parent;
    int color;
    Key data;
  };
  Node<Key> *root;
  size_t Size;

 public:
  class SetIterator;
  class SetConstIterator;

  /* Set Member type */

  using key_type = Key;
  using value_type = Key;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = SetIterator;
  using const_iterator = SetConstIterator;
  using size_type = size_t;

  /* Set Member functions */

  S21_Set();
  S21_Set(std::initializer_list<value_type> const &items);
  S21_Set(const S21_Set &s);
  S21_Set(S21_Set &&s);
  ~S21_Set();
  void operator=(S21_Set &&s);

  /* Set Iterators */

  iterator begin();
  iterator end();

  /* Set Capacity */

  bool empty();
  size_type size();
  size_type max_size();

  /* Set Modifiers */

  void clear();
  std::pair<iterator, bool> insert(const value_type &value);
  void erase(S21_Set::iterator pos);
  void swap(S21_Set &other);
  void merge(S21_Set &other);

  template <typename... Args>
  S21_Vector<std::pair<iterator, bool>> insert_many(Args &&...args);

  /* Set Lookup */

  iterator find(const Key &key);
  bool contains(const Key &key);

  // S21_Set::iterator recursive_search(const Key &key, iterator iter_node);
  std::pair<iterator, bool> recursive_search(iterator iter_node,
                                             const Key &key);
  // template <typename T>
  class SetIterator {
   private:
    Node<Key> *node;

   public:
    SetIterator(Node<Key> *node = nullptr) { this->node = node; }
    Node<Key> *operator->();
    value_type operator*();
    iterator operator++();
    iterator operator--();
    bool operator==(const iterator &iterator) const;
    bool operator!=(const iterator &iterator) const;
    Node<Key> *get_node();
  };
  class SetConstIterator {
   private:
    const Node<Key> *node;

   public:
    SetConstIterator(Node<Key> *node = nullptr) { this->node = node; }
    value_type operator*();
    const_iterator operator++();
    const_iterator operator--();
    bool operator==(const const_iterator &iterator) const;
    bool operator!=(const const_iterator &iterator) const;
  };

 private:
  void balance_tree(Node<Key> *node);
  void recursive_clear(const Node<Key> *node);
  std::pair<iterator, bool> recursive_insert(Node<Key> *node,
                                             const value_type &value);
  void copy(const Node<Key> *node);
  void move(S21_Set &&another);
  bool recursive_contains(const Key &key, const Node<Key> *node);
};
};  // namespace s21
#include "s21_set.tpp"
#endif