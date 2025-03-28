#ifndef __S21_MULTISET_HPP__
#define __S21_MULTISET_HPP__
#include <iostream>
#include <limits>

#include "../s21_vector/s21_vector.hpp"
#define RED 2
#define BLACK 1

using namespace std;
namespace s21 {
template <typename Key>
class S21_Multiset {
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
  class MultisetIterator;
  class MultisetConstIterator;

  /* multiset Member type */

  using key_type = Key;
  using value_type = Key;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = MultisetIterator;
  using const_iterator = MultisetConstIterator;
  using size_type = size_t;

  /* multiset Member functions */

  S21_Multiset();
  S21_Multiset(std::initializer_list<value_type> const &items);
  S21_Multiset(const S21_Multiset &ms);
  S21_Multiset(S21_Multiset &&ms);
  ~S21_Multiset();
  void operator=(S21_Multiset &&ms);

  /* multiset Iterators */

  iterator begin();
  iterator end();

  /* multiset Capacity */

  bool empty();
  size_type size();
  size_type max_size();

  /* multiset Modifiers */

  void clear();
  iterator insert(const value_type &value);
  void erase(S21_Multiset::iterator pos);
  void swap(S21_Multiset &other);
  void merge(S21_Multiset &other);

  template <typename... Args>
  S21_Vector<std::pair<iterator, bool>> insert_many(Args &&...args);

  /* multiset Lookup */

  size_type count(const Key &key);
  bool contains(const Key &key);
  iterator find(const Key &key);
  std::pair<iterator, iterator> equal_range(const Key &key);
  iterator lower_bound(const Key &key);
  iterator upper_bound(const Key &key);

  class MultisetIterator {
   private:
    Node<Key> *node;

   public:
    MultisetIterator(Node<Key> *node = nullptr) { this->node = node; }
    Node<Key> *operator->();
    value_type operator*();
    iterator operator++();
    iterator operator--();
    bool operator==(const iterator &iterator) const;
    bool operator!=(const iterator &iterator) const;
    Node<Key> *get_node();
  };
  class MultisetConstIterator {
   private:
    const Node<Key> *node;

   public:
    MultisetConstIterator(Node<Key> *node = nullptr) { this->node = node; }
    value_type operator*();
    const_iterator operator++();
    const_iterator operator--();
    bool operator==(const const_iterator &iterator) const;
    bool operator!=(const const_iterator &iterator) const;
  };

 private:
  bool recursive_contains(const Key &key, const Node<Key> *node);
  void balance_tree(Node<Key> *node);
  void recursive_clear(const Node<Key> *node);
  void copy(const Node<Key> *node);
  void move(S21_Multiset &&another);
  iterator recursive_insert(Node<Key> *node, const value_type &value);
  std::pair<iterator, bool> recursive_search(iterator iter_node,
                                             const Key &key);
};
};  // namespace s21
#include "s21_multiset.tpp"
#endif