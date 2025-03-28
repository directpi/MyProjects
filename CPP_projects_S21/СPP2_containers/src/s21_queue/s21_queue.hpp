#ifndef __S21_QUEUE_HPP__
#define __S21_QUEUE_HPP__

#include <iostream>
using namespace std;

namespace s21 {

template <typename Type>
class S21_Queue {
 public:
  // Stack Member type

  using value_type = Type;
  using reference = Type&;
  using const_reference = const Type&;
  using size_type = size_t;

  // Stack Member functions

  S21_Queue();
  S21_Queue(std::initializer_list<value_type> const& items);
  S21_Queue(const S21_Queue& another);
  S21_Queue(S21_Queue&& another);
  ~S21_Queue();

  void copy(const S21_Queue& another);
  void move(S21_Queue&& another);
  void operator=(S21_Queue&& another);

  // Stack Element access

  const_reference front();
  const_reference back();

  // Stack Capacity

  bool empty();
  size_type size();

  // Stack Modifiers

  void push(const_reference data);
  void pop();
  void clear();
  void swap(S21_Queue& another);

  template <typename... Args>
  void insert_many_back(Args&&... args);

 private:
  template <typename>
  class Node {
   public:
    Node(value_type data, Node* pNext = nullptr, Node* pPrev = nullptr) {
      this->data = data;
      this->pNext = pNext;
      this->pPrev = pPrev;
    }
    value_type data;
    Node* pPrev;
    Node* pNext;
  };
  Node<value_type>* head;
  Node<value_type>* tail;
  size_type Size;
};

}  // namespace s21

#include "s21_queue.tpp"

#endif