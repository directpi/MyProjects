namespace s21 {

/*default constructor, creates empty stack*/
template <typename Type>
S21_Stack<Type>::S21_Stack() {
  this->head = nullptr;
  this->Size = 0U;
}

/*initializer list constructor, creates stack initizialized using
 * std::initializer_list*/
template <typename Type>
S21_Stack<Type>::S21_Stack(std::initializer_list<value_type> const& items) {
  this->head = nullptr;
  this->Size = 0U;
  if (items.size()) {
    auto it = items.begin();
    while (it != items.end()) this->push(*it++);
  }
}

/*copy constructor*/
template <typename Type>
S21_Stack<Type>::S21_Stack(const S21_Stack& another) {
  if (another.head == nullptr)
    throw std::invalid_argument("copy error: cannot copy from an empty stack");
  this->head = nullptr;
  this->Size = 0U;
  const Node<value_type>* current = another.head;
  size_type counter = another.Size;
  while (counter) {
    for (size_type i = 0; i < counter - 1; ++i) current = current->pPrev;
    this->push(current->data);
    current = another.head;
    --counter;
  }
}

/*move constructor*/
template <typename Type>
S21_Stack<Type>::S21_Stack(S21_Stack&& another) {
  this->move(std::move(another));
}

/*assignment operator overload for moving object*/
template <typename Type>
void S21_Stack<Type>::operator=(S21_Stack&& another) {
  this->move(std::move(another));
}

/*move function*/
template <typename Type>
void S21_Stack<Type>::move(S21_Stack&& another) {
  if (another.head == nullptr)
    throw std::invalid_argument("move error: cannot move from an empty stack");
  this->head = nullptr;
  this->Size = 0U;
  const Node<value_type>* current = another.head;
  size_type counter = another.Size;
  while (counter) {
    for (size_type i = 0; i < counter - 1; ++i) current = current->pPrev;
    this->push(current->data);
    const Node<value_type>* n_toremove = current;
    delete n_toremove;
    current = another.head;
    --counter;
  }
  another.Size = 0U;
  another.head = nullptr;
}

/*destructor*/
template <typename Type>
S21_Stack<Type>::~S21_Stack() {
  this->clear();
}

/*removes all elements from the stack*/
template <typename Type>
void S21_Stack<Type>::clear() {
  while (this->head != nullptr) {
    const Node<value_type>* current = this->head;
    this->head = current->pPrev;
    delete current;
    this->Size--;
  }
}

/*accesses the top element*/
template <typename Type>
typename S21_Stack<Type>::const_reference S21_Stack<Type>::top() {
  // const Node<value_type>& top = this->head;
  return this->head->data;
}

/*inserts element at the top*/
template <typename Type>
void S21_Stack<Type>::push(value_type data) {
  if (this->head == nullptr) {
    this->head = new Node<value_type>(data);
  } else {
    const Node<value_type>* new_node = new Node<value_type>(data, this->head);
    this->head = new_node;
  }
  this->Size++;
}

/*removes the top element*/
template <typename Type>
void S21_Stack<Type>::pop() {
  if (this->head == nullptr)
    throw std::out_of_range("pop error: stack is empty");

  const Node<value_type>* current = this->head;
  this->head = current->pPrev;
  delete current;
  this->Size--;
}

/*checks whether the container is empty*/
template <typename Type>
bool S21_Stack<Type>::empty() {
  return this->Size ? false : true;
}

/*returns the number of elements*/
template <typename Type>
typename S21_Stack<Type>::size_type S21_Stack<Type>::size() {
  return this->Size;
}

/*swaps the contents*/
template <typename Type>
void S21_Stack<Type>::swap(S21_Stack& another) {
  S21_Stack<value_type> tmp(std::move(another));
  another.move(std::move(*this));
  this->move(std::move(tmp));
}

/*Appends new elements to the top of the container*/
template <typename Type>
template <typename... Args>
void S21_Stack<Type>::insert_many_front(Args&&... args) {
  ((this->push(args)), ...);
}

}  // namespace s21