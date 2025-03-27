
namespace s21 {

/*default iterator constructor*/
template <typename Type>
ListIterator<Type>::ListIterator(Node<value_type> *iter) {
  this->iter = iter;
}

/*operator *: returns node value*/
template <typename Type>
typename ListIterator<Type>::reference ListIterator<Type>::operator*() {
  return iter->data;
}

/*increment iterator*/
template <typename Type>
typename ListIterator<Type>::iterator ListIterator<Type>::operator++(int) {
  Node<value_type> *tmp = iter;
  iter = iter->pNext;
  return tmp;
}

/*decrement iterator*/
template <typename Type>
typename ListIterator<Type>::iterator ListIterator<Type>::operator--(int) {
  Node<value_type> *tmp = iter;
  iter = iter->pPrev;
  return tmp;
}

/*increment iterator*/
template <typename Type>
typename ListIterator<Type>::iterator ListIterator<Type>::operator++() {
  iter = iter->pNext;
  return iter;
}

/*decrement iterator*/
template <typename Type>
typename ListIterator<Type>::iterator ListIterator<Type>::operator--() {
  iter = iter->pPrev;
  return iter;
}

/*compares two iterators
returns true if not equal*/
template <typename Type>
bool ListIterator<Type>::operator!=(const iterator &Iterator) {
  return this->iter != Iterator.iter;
}

/*compares two iterators
returns true if  equal*/
template <typename Type>
bool ListIterator<Type>::operator==(const iterator &Iterator) {
  return this->iter == Iterator.iter;
}

/*returns node*/
template <typename Type>
Node<Type> *ListIterator<Type>::get_node() {
  return iter;
}

/*const iterator constructor*/
template <typename Type>
ListConstIterator<Type>::ListConstIterator(const Node<value_type> *iter) {
  this->iter = iter;
}

/*operator *: returns node value*/
template <typename Type>
typename ListConstIterator<Type>::const_reference
ListConstIterator<Type>::operator*() const {
  return iter->data;
}

/*increment operator*/
template <typename Type>
typename ListConstIterator<Type>::const_iterator
ListConstIterator<Type>::operator++(int) {
  const Node<value_type> *tmp = iter;
  iter = iter->pNext;
  return tmp;
}

/*decrement operator*/
template <typename Type>
typename ListConstIterator<Type>::const_iterator
ListConstIterator<Type>::operator--(int) {
  const Node<value_type> *tmp = iter;
  iter = iter->pPrev;
  return tmp;
}

/*increment operator*/
template <typename Type>
typename ListConstIterator<Type>::const_iterator
ListConstIterator<Type>::operator++() {
  iter = iter->pNext;
  return iter;
}

/*decrement operator*/
template <typename Type>
typename ListConstIterator<Type>::const_iterator
ListConstIterator<Type>::operator--() {
  iter = iter->pPrev;
  return iter;
}

/*compares two iterators
returns true if not equal*/
template <typename Type>
bool ListConstIterator<Type>::operator!=(const const_iterator &Iterator) {
  return this->iter != Iterator.iter;
}

/*compares two iterators
returns true if equal*/
template <typename Type>
bool ListConstIterator<Type>::operator==(const const_iterator &Iterator) {
  return this->iter == Iterator.iter;
}

/*default constructor, creates empty list*/
template <typename Type>
S21_List<Type>::S21_List() {
  Size = 0;
  head = nullptr;
  tail = nullptr;
}

/*parameterized constructor, creates the list of size n*/
template <typename Type>
S21_List<Type>::S21_List(size_type n) {
  if (n >= this->max_size()) throw std::invalid_argument("invalid list size");
  if (!n) {
    Size = 0;
    head = nullptr;
    tail = nullptr;
  } else {
    Node<value_type> *prev_node = new Node<value_type>(0);
    this->head = this->tail = prev_node;
    for (size_type i = 0; i < n - 1; ++i) {
      Node<value_type> *new_node = new Node<value_type>(0, nullptr, prev_node);
      prev_node->pNext = new_node;
      prev_node = new_node;
    }
    this->tail = prev_node;
    this->Size = n;
  }
}

/*initializer list constructor, creates list initizialized using
 * std::initializer_list<T>*/
template <typename Type>
S21_List<Type>::S21_List(std::initializer_list<value_type> const &items) {
  if (items.size() == 0) {
    Size = 0;
    head = nullptr;
    tail = nullptr;
  } else {
    auto it = items.begin();
    Node<value_type> *prev_node = new Node<value_type>(*it++);
    this->head = this->tail = prev_node;
    Node<value_type> *new_node = nullptr;
    while (it != items.end()) {
      new_node = new Node<value_type>(*it++, nullptr, prev_node);
      prev_node->pNext = new_node;
      prev_node = new_node;
    }
    this->tail = prev_node;
    this->Size = items.size();
  }
}

/*copy constructor*/
template <typename Type>
S21_List<Type>::S21_List(const S21_List &another) {
  this->copy(another);
}

/*move constructor*/
template <typename Type>
S21_List<Type>::S21_List(S21_List &&another) {
  this->move(std::move(another));
}

/*Destructor*/
template <typename Type>
S21_List<Type>::~S21_List() {
  this->clear();
}

/*assignment operator overload for moving object*/
template <typename Type>
void S21_List<Type>::operator=(S21_List &&another) {
  this->move(std::move(another));
}

/*copy the list*/
template <typename Type>
void S21_List<Type>::copy(const S21_List &another) {
  if (another.Size == 0)
    throw std::invalid_argument("unable to copy empty list");
  Node<value_type> *another_node = another.head;
  Node<value_type> *prev_node = new Node<value_type>(another_node->data);
  another_node = another_node->pNext;
  this->head = this->tail = prev_node;
  for (size_type i = 1; i < another.Size; ++i) {
    Node<value_type> *new_node =
        new Node<value_type>(another_node->data, nullptr, prev_node);
    prev_node->pNext = new_node;
    prev_node = new_node;
    another_node = another_node->pNext;
  }
  this->tail = prev_node;
  this->Size = another.Size;
}

/*move the list*/
template <typename Type>
void S21_List<Type>::move(S21_List &&another) {
  this->copy(another);
  another.clear();
}

/*clear the list*/
template <typename Type>
void S21_List<Type>::clear() {
  while (this->Size) {
    pop_front();
  }
}

/*access the last element*/
template <typename Type>
const Type &S21_List<Type>::back() {
  if (this->Size == 0) throw std::out_of_range("error: list is empty");
  return this->tail->data;
}

/*access the first element*/
template <typename Type>
const Type &S21_List<Type>::front() {
  if (this->Size == 0) throw std::out_of_range("error: list is empty");
  return this->head->data;
}

/*adds an element to the end*/
template <typename Type>
void S21_List<Type>::push_back(const_reference value) {
  if (this->tail == nullptr) {
    this->head = this->tail = new Node<Type>(value);
  } else {
    Node<Type> *tmp = new Node<Type>(value, nullptr, this->tail);
    this->tail->pNext = tmp;
    this->tail = tmp;
  }
  this->Size++;
}

/*add element at the first place*/
template <typename Type>
void S21_List<Type>::push_front(const_reference value) {
  if (this->head == nullptr) {
    this->head = this->tail = new Node<Type>(value);
  } else {
    this->head = new Node<Type>(value, head);
    this->head->pNext->pPrev = this->head;
  }
  this->Size++;
}

/*removes the first element*/
template <typename Type>
void S21_List<Type>::pop_front() {
  if (this->Size == 0) throw std::out_of_range("list is empty");
  if (this->head->pNext == nullptr) {
    delete this->head;
  } else {
    Node<Type> *tmp = this->head;
    this->head = this->head->pNext;
    this->head->pPrev = nullptr;
    delete tmp;
  }
  this->Size--;
  if (!Size) head = tail = nullptr;
}

/*removes the last element*/
template <typename Type>
void S21_List<Type>::pop_back() {
  if (this->Size == 0) throw std::out_of_range("list is empty");
  if (this->tail->pPrev == nullptr) {
    delete this->tail;
  } else {
    Node<value_type> *tmp = this->tail;
    this->tail = this->tail->pPrev;
    this->tail->pNext = nullptr;
    delete tmp;
  }
  this->Size--;
}

/*inserts element at specified index*/
template <typename Type>
void S21_List<Type>::insertAt(value_type data, size_type index) {
  if (index > this->Size - 1)
    throw std::invalid_argument("insert error: index out of bounds");
  if (!index) {
    this->push_front(data);
  } else {
    Node<value_type> *current = this->head;
    size_type counter = 0;
    while (counter != index - 1) {
      current = current->pNext;
      counter++;
    }
    Node<value_type> *new_node =
        new Node<value_type>(data, current->pNext, current);
    current->pNext = new_node;
    new_node->pNext->pPrev = new_node;
    this->Size++;
  }
}

/*inserts element at specified index*/
template <typename Type>
void S21_List<Type>::removeAt(size_type index) {
  if (index > this->Size - 1)
    throw std::invalid_argument("removeAt error: index out of bounds");
  if (!index) {
    this->pop_front();
  } else if (index == this->Size - 1) {
    this->pop_back();
  } else {
    Node<value_type> *current = this->head;
    size_type counter = 0;
    while (counter != index - 1) {
      current = current->pNext;
      counter++;
    }
    Node<value_type> *node_to_remove = current->pNext;
    current->pNext = node_to_remove->pNext;
    node_to_remove->pNext->pPrev = current;
    delete node_to_remove;
    this->Size--;
  }
}

/*operator []*/
template <typename Type>
Type &S21_List<Type>::operator[](const size_type index) {
  if (index > this->Size - 1) throw std::out_of_range("index out of bounds");
  if (this->Size == 0) throw std::invalid_argument("list is empty");
  Node<value_type> *current = nullptr;
  if (Size - index < index) {
    size_type counter = this->Size - 1;
    current = this->tail;
    while (counter != index) {
      current = current->pPrev;
      counter--;
    }
  } else {
    size_type counter = 0;
    current = this->head;
    while (counter != index) {
      current = current->pNext;
      counter++;
    }
  }
  return current->data;
}

/*returns the maximum possible number of elements*/
template <typename Type>
size_t S21_List<Type>::max_size() {
  auto max_allocatable =
      std::numeric_limits<std::size_t>::max() / sizeof(value_type);
  return max_allocatable;
}

/*inserts element into concrete pos and returns the iterator that points to the
 * new element*/
template <typename Type>
typename S21_List<Type>::iterator S21_List<Type>::insert(
    iterator pos, const_reference value) {
  if (!this->Size) throw std::invalid_argument("insert error: list is empty");
  if (pos.get_node() == nullptr)
    throw std::invalid_argument("insert error: incorrect iterator pos");
  Node<value_type> *new_node = nullptr;
  if (pos.get_node() == this->head) {
    this->push_front(value);
    new_node = this->head;
  } else {
    Node<value_type> *current = pos.get_node();
    new_node = new Node<value_type>(value, current, current->pPrev);
    if (current->pPrev != nullptr) current->pPrev->pNext = new_node;
    current->pPrev = new_node;
    if (this->Size == 1) this->head = new_node;
    this->Size++;
  }
  return new_node;
}

/*erases element at pos*/
template <typename Type>
void S21_List<Type>::erase(iterator pos) {
  if (!this->Size) throw std::invalid_argument("erase error: list is empty");
  if (pos.get_node() == nullptr)
    throw std::invalid_argument("erase error: incorrect iterator pos");
  Node<value_type> *current = pos.get_node();
  if (current == this->head)
    pop_front();
  else if (current == this->tail)
    pop_back();
  else {
    current->pPrev->pNext = current->pNext;
    current->pNext->pPrev = current->pPrev;
    this->Size--;
    delete current;
  }
}

/*swaps the contents*/
template <typename Type>
void S21_List<Type>::swap(S21_List &another) {
  size_type tmp_size = this->Size;
  Node<value_type> *tmp_head = this->head;
  Node<value_type> *tmp_tail = this->tail;

  this->Size = another.Size;
  this->head = another.head;
  this->tail = another.tail;

  another.Size = tmp_size;
  another.head = tmp_head;
  another.tail = tmp_tail;
}

/*removes consecutive duplicate elements*/
template <typename Type>
void S21_List<Type>::unique() {
  S21_List<value_type>::iterator it = this->begin();
  while (it != this->end()) {
    S21_List<value_type>::iterator tmp = it++;
    if (*it == *tmp) this->erase(tmp);
  }
}

/*reverses the order of the elements*/
template <typename Type>
void S21_List<Type>::reverse() {
  Node<value_type> *current = this->head;
  Node<value_type> *tmp = nullptr;
  while (current != nullptr) {
    tmp = current->pPrev;
    current->pPrev = current->pNext;
    current->pNext = tmp;
    current = current->pPrev;
  }
  tmp = this->head;
  this->head = this->tail;
  this->tail = tmp;
}

/*transfers elements from list other starting from pos*/
template <typename Type>
void S21_List<Type>::splice(iterator pos, S21_List another) {
  for (size_type i = 0; i < another.size(); ++i) {
    pos = this->insert(pos, another[i]);
    ++pos;
  }
}

/*return iterator to the first element*/
template <typename Type>
typename S21_List<Type>::iterator S21_List<Type>::begin() {
  return this->head;
}

/*return iterator to the last element*/
template <typename Type>
typename S21_List<Type>::iterator S21_List<Type>::end() {
  return this->tail;
}

/*return const iterator to the first element*/
template <typename Type>
typename S21_List<Type>::const_iterator S21_List<Type>::cbegin() {
  return this->head;
}

/*return const iterator to the last element*/
template <typename Type>
typename S21_List<Type>::const_iterator S21_List<Type>::cend() {
  return this->tail;
}

/*checks whether the container is empty*/
template <typename Type>
bool S21_List<Type>::empty() {
  return Size ? false : true;
}

/*returns the number of elements*/
template <typename Type>
size_t S21_List<Type>::size() {
  return Size;
}

/*sorts the elements*/
template <typename Type>
void S21_List<Type>::sort() {
  if (this->Size > 1) quick_sort(this->begin(), this->end());
}

/*recursive quick sort algorithm*/
template <typename Type>
void S21_List<Type>::quick_sort(iterator left, iterator right) {
  if (left != right) {
    size_type counter = 1;
    S21_List<value_type>::iterator tmp = get_c_element(left, right, &counter),
                                   left_tmp = left, right_tmp = right;
    while (left_tmp != tmp || right_tmp != tmp) {
      bool l_found = false, r_found = false;
      while (left_tmp != tmp) {
        if (*left_tmp > *tmp) {
          l_found = true;
          break;
        }
        ++left_tmp;
      }
      while (right_tmp != tmp) {
        if (*right_tmp < *tmp) {
          r_found = true;
          break;
        }
        --right_tmp;
      }
      if (l_found && r_found)
        swap_nodes(left_tmp, right_tmp, left, right);
      else if (l_found && !r_found) {
        Node<value_type> *tmp_node = tmp.get_node();
        S21_List<value_type>::iterator rm_iter = left_tmp;
        if (tmp_node->pNext == nullptr) {
          push_back(*left_tmp);
        } else {
          insert(++tmp, *left_tmp);
          --tmp;
          --tmp;
        }
        ++left_tmp;
        Node<value_type> *rm_node = rm_iter.get_node();
        if (rm_node->pPrev == nullptr) ++left;
        erase(rm_iter);
      } else if (r_found && !l_found) {
        Node<value_type> *tmp_node = tmp.get_node();
        S21_List<value_type>::iterator rm_iter = right_tmp;
        if (tmp_node->pPrev == nullptr) {
          push_front(*right_tmp);
        } else {
          insert(tmp, *right_tmp);
        }
        --right_tmp;
        Node<value_type> *rm_node = rm_iter.get_node();
        if (rm_node->pNext == nullptr) --right;
        erase(rm_iter);
      }
    }
    if (left != tmp && counter > 1) quick_sort(left, tmp);
    if (right != tmp) quick_sort(tmp, right);
  }
}

/*return central list element for partition*/
template <typename Type>
typename S21_List<Type>::iterator S21_List<Type>::get_c_element(
    iterator left, iterator right, size_type *counter) {
  S21_List<value_type>::iterator tmp = left;
  while (tmp != right) {
    ++tmp;
    *counter += 1;
  }
  tmp = left;
  *counter = *counter / 2;
  for (size_type i = 0; i < *counter; ++i) {
    ++tmp;
  }
  return tmp;
}

/*swaps nodes*/
template <typename Type>
void S21_List<Type>::swap_nodes(iterator &left_tmp, iterator &right_tmp,
                                iterator &left, iterator &right) {
  Node<value_type> *left_node = left_tmp.get_node();
  Node<value_type> *right_node = right_tmp.get_node();
  if (left_node->pPrev != nullptr) left_node->pPrev->pNext = right_node;
  left_node->pNext->pPrev = right_node;
  right_node->pPrev->pNext = left_node;
  if (right_node->pNext != nullptr) right_node->pNext->pPrev = left_node;
  Node<value_type> *tmp_l_next = left_node->pNext;
  Node<value_type> *tmp_l_prev = left_node->pPrev;
  Node<value_type> *tmp_r_next = right_node->pNext;
  Node<value_type> *tmp_r_prev = right_node->pPrev;
  left_node->pNext = tmp_r_next;
  left_node->pPrev = tmp_r_prev;
  right_node->pNext = tmp_l_next;
  right_node->pPrev = tmp_l_prev;
  if (left_tmp == left) left = right_tmp;
  if (right_tmp == right) right = left_tmp;
  if (left_tmp == head) head = right_node;
  if (right_tmp == tail) tail = left_node;
  S21_List<value_type>::iterator tmp_left_iter = left_tmp;
  left_tmp = right_tmp;
  right_tmp = tmp_left_iter;
}

/*merges two sorted lists*/
template <typename Type>
void S21_List<Type>::merge(S21_List &another) {
  Node<value_type> *dst_cur = this->head;
  Node<value_type> *src_cur = another.head;
  if (!this->Size) {
    this->head = another.head;
    this->tail = another.tail;
    this->Size = another.Size;
  } else {
    while (dst_cur != nullptr) {
      while (src_cur != nullptr) {
        Node<value_type> *tmp = src_cur;
        src_cur = src_cur->pNext;
        if (tmp->data <= dst_cur->data) {
          if (dst_cur->pPrev) dst_cur->pPrev->pNext = tmp;
          tmp->pPrev = dst_cur->pPrev;
          dst_cur->pPrev = tmp;
          tmp->pNext = dst_cur;
          if (!tmp->pPrev) this->head = tmp;
          another.head = src_cur;
        }
      }
      src_cur = another.head;
      dst_cur = dst_cur->pNext;
    }
    if (src_cur) {
      dst_cur = this->tail;
      dst_cur->pNext = src_cur;
      src_cur->pPrev = dst_cur;
      this->tail = another.tail;
    }
    this->Size += another.Size;
  }
  another.Size = 0;
  another.head = another.tail = nullptr;
}

/*Appends new elements to the end of the container.*/
template <typename Type>
template <typename... Args>
void S21_List<Type>::insert_many_back(Args &&...args) {
  ((this->push_back(args)), ...);
}

/*Appends new elements to the top of the container.*/
template <typename Type>
template <typename... Args>
void S21_List<Type>::insert_many_front(Args &&...args) {
  if (this->Size && sizeof...(args)) {
    S21_List<value_type> tmp;
    ((tmp.push_back(args)), ...);
    S21_List<value_type>::iterator it = this->begin();
    this->splice(it, tmp);
  } else {
    ((this->push_back(args)), ...);
  }
}

/*Inserts new elements into the container directly before pos.*/
template <typename Type>
template <typename... Args>
typename S21_List<Type>::iterator S21_List<Type>::insert_many(
    const_iterator pos, Args &&...args) {
  S21_List<value_type>::iterator new_pos = this->begin();
  if (this->Size == 0) {
    ((this->push_back(args)), ...);
    new_pos = this->begin();
  } else if (sizeof...(args)) {
    S21_List<value_type>::const_iterator c_tmp_it = this->cbegin();
    while (c_tmp_it != pos) {
      ++new_pos;
      ++c_tmp_it;
    };
    S21_List<value_type> tmp;
    ((tmp.push_back(args)), ...);
    bool is_first = true;
    S21_List<value_type>::iterator tmp_iter = new_pos;
    if (pos != this->cbegin()) {
      is_first = false;
      --new_pos;
    }
    this->splice(tmp_iter, tmp);
    if (!is_first)
      ++new_pos;
    else
      new_pos = this->begin();
  }
  return new_pos;
}

};  // namespace s21