#ifndef __S21_STACK_HPP__
#define __S21_STACK_HPP__

#include <iostream>
namespace s21 {

template <typename Type>
class S21_Stack {
 public:
  // Stack Member type

  using value_type = Type;
  using reference = Type&;
  using const_reference = const Type&;
  using size_type = size_t;

  // Stack Member functions

  S21_Stack();
  S21_Stack(std::initializer_list<value_type> const& items);
  S21_Stack(const S21_Stack& another);
  S21_Stack(S21_Stack&& another);
  ~S21_Stack();
  void operator=(S21_Stack&& another);
  void move(S21_Stack&& another);

  // Stack Element access

  const_reference top();

  // Stack Capacity

  bool empty();
  size_type size();

  // Stack Modifiers

  void push(value_type data);
  void pop();
  void clear();
  void swap(S21_Stack& another);

  template <typename... Args>
  void insert_many_front(Args&&... args);

 private:
  template <typename>
  class Node {
   public:
    Node(value_type data, const Node* pPrev = nullptr) {
      this->data = data;
      this->pPrev = pPrev;
    }
    value_type data;
    const Node* pPrev;
  };
  const Node<value_type>* head;
  size_type Size;
};

}  // namespace s21

#include "s21_stack.tpp"

#endif
