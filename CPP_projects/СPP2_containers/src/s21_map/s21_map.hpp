#ifndef __S21_MAP_HPP__
#define __S21_MAP_HPP__

#include <iostream>

#include "../s21_vector/s21_vector.hpp"
#define RED 2
#define BLACK 1

using namespace std;
namespace s21 {

template <typename Key, typename Type>
class S21_Map {
 private:
  template <typename, typename>
  class Node {
   public:
    Node(const std::pair<Key, Type>& pair, Node* pLeft = nullptr,
         Node* pRight = nullptr, Node* Parent = nullptr, size_t color = RED) {
      this->first = pair.first;
      this->second = pair.second;
      this->pLeft = pLeft;
      this->pRight = pRight;
      this->Parent = Parent;
      this->color = color;
    }
    Node<Key, Type>* pLeft;
    Node<Key, Type>* pRight;
    Node<Key, Type>* Parent;
    size_t color;
    Key first;
    Type second;
  };

  Node<Key, Type>* root;
  size_t Size;

 public:
  class MapIterator;
  class MapConstIterator;

  // Map Member type

  using key_type = Key;
  using mapped_type = Type;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = MapIterator;
  using const_iterator = MapConstIterator;
  using size_type = size_t;

  class MapIterator {
   private:
    Node<key_type, mapped_type>* node;

   public:
    MapIterator(Node<key_type, mapped_type>* node = nullptr);

    value_type operator*();
    Node<key_type, mapped_type>* operator->();
    iterator operator++();
    iterator operator--();
    bool operator==(const iterator& iterator) const;
    bool operator!=(const iterator& iterator) const;
    Node<key_type, mapped_type>* get_node();
  };

  class MapConstIterator {
   private:
    const Node<key_type, mapped_type>* node;

   public:
    MapConstIterator(const Node<key_type, mapped_type>* node = nullptr);

    value_type operator*();
    const_iterator operator++();
    const_iterator operator--();
    bool operator==(const const_iterator& iterator) const;
    bool operator!=(const const_iterator& iterator) const;
  };

  // Map Member functions

  S21_Map();
  S21_Map(std::initializer_list<value_type> const& items);
  S21_Map(const S21_Map& another);
  S21_Map(S21_Map&& another);
  ~S21_Map();
  void operator=(S21_Map&& another);

  // Map Element access

  Type& at(const Key& key);
  Type& operator[](const Key& key);

  // Map Iterators

  iterator begin();
  iterator end();

  // Map Capacity

  bool empty();
  size_type size();
  size_type max_size();

  // Map Modifiers

  void clear();
  std::pair<iterator, bool> insert(const value_type& value);
  std::pair<iterator, bool> insert(const key_type& key, const Type& value);
  std::pair<iterator, bool> insert_or_assign(const Key& key, const Type& value);
  void erase(iterator pos);
  void swap(S21_Map& another);
  void merge(S21_Map& another);

  template <typename... Args>
  S21_Vector<std::pair<iterator, bool>> insert_many(Args&&... args);

  // Map Lookup

  bool contains(const Key& key);

 private:
  void recursive_clear(const Node<Key, Type>* node);
  void copy(const Node<Key, Type>* node);
  void move(S21_Map&& another);
  bool recursive_contains(const Key& key, const Node<Key, Type>* node);
  std::pair<iterator, bool> recursive_search(iterator iter_node,
                                             const Key& key);
  std::pair<iterator, bool> recursive_insert(Node<Key, Type>* node,
                                             const key_type& key,
                                             const Type& value);
  void balance_tree(Node<Key, Type>* node);
};

};  // namespace s21

#include "s21_map.tpp"

#endif