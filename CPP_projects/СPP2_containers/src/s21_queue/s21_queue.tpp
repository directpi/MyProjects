namespace s21 {
/*default constructor, creates empty stack*/
template <typename Type>
S21_Queue<Type>::S21_Queue() {
  this->head = nullptr;
  this->tail = nullptr;
  this->Size = 0U;
}

/*initializer list constructor, creates stack initizialized using
 * std::initializer_list*/
template <typename Type>
S21_Queue<Type>::S21_Queue(std::initializer_list<value_type> const& items) {
  this->head = nullptr;
  this->tail = nullptr;
  this->Size = 0U;
  if (items.size()) {
    auto it = items.begin();
    while (it != items.end()) this->push(*it++);
  }
}

/*copy constructor*/
template <typename Type>
S21_Queue<Type>::S21_Queue(const S21_Queue& another) {
  this->copy(another);
}

/*copies another queue to this queue*/
template <typename Type>
void S21_Queue<Type>::copy(const S21_Queue& another) {
  if (another.head == nullptr)
    throw std::invalid_argument("copy error: cannot copy from an empty queue");
  this->head = this->tail = nullptr;
  this->Size = 0U;
  Node<value_type>* current = another.tail;
  while (current != nullptr) {
    this->push(current->data);
    current = current->pPrev;
  }
}

/*move constructor*/
template <typename Type>
S21_Queue<Type>::S21_Queue(S21_Queue&& another) {
  this->move(std::move(another));
}

/*destructor*/
template <typename Type>
S21_Queue<Type>::~S21_Queue() {
  this->clear();
}

/*moves another queue to current queue*/
template <typename Type>
void S21_Queue<Type>::move(S21_Queue&& another) {
  if (another.head == nullptr)
    throw std::invalid_argument("move error: cannot move from an empty queue");
  this->head = this->tail = nullptr;
  this->Size = 0U;
  while (another.tail != nullptr) {
    this->push(another.tail->data);
    another.pop();
  }
}

/*assignment operator overload for moving object*/
template <typename Type>
void S21_Queue<Type>::operator=(S21_Queue&& another) {
  this->move(std::move(another));
}

/*removes all elements from the stack*/
template <typename Type>
void S21_Queue<Type>::clear() {
  while (this->head != nullptr) {
    this->pop();
  }
}

/*inserts element at the end*/
template <typename Type>
void S21_Queue<Type>::push(const_reference data) {
  if (this->head == nullptr) {
    this->head = this->tail = new Node<value_type>(data);
  } else {
    Node<value_type>* new_node = new Node<value_type>(data);
    new_node->pNext = this->head;
    this->head->pPrev = new_node;
    this->head = new_node;
  }
  this->Size++;
}

/*removes the first element*/
template <typename Type>
void S21_Queue<Type>::pop() {
  if (this->head == nullptr) {
    throw invalid_argument("pop error: the queue is empty");
  }
  Node<Type>* pop_node = this->tail;
  if (pop_node->pPrev != nullptr) {
    pop_node->pPrev->pNext = nullptr;
    this->tail = pop_node->pPrev;
  } else {
    this->head = this->tail = nullptr;
  }
  this->Size--;
  delete pop_node;
}

/*access the first element*/
template <typename Type>
typename S21_Queue<Type>::const_reference S21_Queue<Type>::front() {
  if (this->tail == nullptr)
    throw invalid_argument("front error: queue is empty");
  return this->tail->data;
}

/*access the last element*/
template <typename Type>
typename S21_Queue<Type>::const_reference S21_Queue<Type>::back() {
  if (this->head == nullptr)
    throw invalid_argument("back error: queue is empty");
  return this->head->data;
}

/*checks whether the container is empty*/
template <typename Type>
bool S21_Queue<Type>::empty() {
  return this->Size ? false : true;
}

/*returns the number of elements*/
template <typename Type>
typename S21_Queue<Type>::size_type S21_Queue<Type>::size() {
  return this->Size;
}

/*swaps the contents*/
template <typename Type>
void S21_Queue<Type>::swap(S21_Queue& another) {
  S21_Queue<value_type> tmp(std::move(another));
  another.move(std::move(*this));
  this->move(std::move(tmp));
}

template <typename Type>
template <typename... Args>
void S21_Queue<Type>::insert_many_back(Args&&... args) {
  ((this->push(args)), ...);
}

}  // namespace s21