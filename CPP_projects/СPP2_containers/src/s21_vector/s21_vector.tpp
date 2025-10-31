namespace s21 {

/*default iterator constructor*/
template <typename Type>
VectorIterator<Type>::VectorIterator(value_type* iter) : iter(iter) {}

/*increment iterator by n*/
template <typename Type>
typename VectorIterator<Type>::iterator VectorIterator<Type>::operator+(int n) {
  return (iter + n);
}

/*increment iterator by n and assing*/
template <typename Type>
typename VectorIterator<Type>::iterator VectorIterator<Type>::operator+=(
    int n) {
  iter = iter + n;
  return iter;
}

/*decrement iterator by n*/
template <typename Type>
typename VectorIterator<Type>::iterator VectorIterator<Type>::operator-(int n) {
  return (iter - n);
}

/*decrement iterator by n and assing*/
template <typename Type>
typename VectorIterator<Type>::iterator VectorIterator<Type>::operator-=(
    int n) {
  iter = iter - n;
  return iter;
}

/*increment iterator*/
template <typename Type>
typename VectorIterator<Type>::iterator VectorIterator<Type>::operator++(int) {
  return iter++;
}

/*decrement iterator*/
template <typename Type>
typename VectorIterator<Type>::iterator VectorIterator<Type>::operator--(int) {
  return iter--;
}

/*increment iterator*/
template <typename Type>
typename VectorIterator<Type>::iterator VectorIterator<Type>::operator++() {
  return ++iter;
}

/*decrement iterator*/
template <typename Type>
typename VectorIterator<Type>::iterator VectorIterator<Type>::operator--() {
  return --iter;
}

/*unpack iterator*/
template <typename Type>
typename VectorIterator<Type>::reference VectorIterator<Type>::operator*() {
  return *iter;
}

/*substruct another from iterator*/
template <typename Type>
typename VectorIterator<Type>::size_type VectorIterator<Type>::operator-(
    iterator another) {
  return this->iter - another.iter;
}

/*sum another with iterator*/
template <typename Type>
typename VectorIterator<Type>::size_type VectorIterator<Type>::operator+(
    iterator another) {
  return this->iter + another.iter;
}

/*return true if iterator != another*/
template <typename Type>
bool VectorIterator<Type>::operator!=(const iterator& another) {
  return this->iter != another.iter;
}

/*return true if iterator == another*/
template <typename Type>
bool VectorIterator<Type>::operator==(const iterator& another) {
  return this->iter == another.iter;
}

/*return true if iterator > another*/
template <typename Type>
bool VectorIterator<Type>::operator>(const iterator& another) {
  return this->iter > another.iter;
}

/*return true if iterator >= another*/
template <typename Type>
bool VectorIterator<Type>::operator>=(const iterator& another) {
  return this->iter >= another.iter;
}

/*return true if iterator < another*/
template <typename Type>
bool VectorIterator<Type>::operator<(const iterator& another) {
  return this->iter < another.iter;
}

/*return true if iterator < another*/
template <typename Type>
bool VectorIterator<Type>::operator<=(const iterator& another) {
  return this->iter <= another.iter;
}

/*default iterator constructor*/
template <typename Type>
VectorConstIterator<Type>::VectorConstIterator(const value_type* iter)
    : iter(iter) {}

/*increment iterator by n*/
template <typename Type>
typename VectorConstIterator<Type>::const_iterator
VectorConstIterator<Type>::operator+(int n) {
  return (iter + n);
}

/*increment iterator by n and assing*/
template <typename Type>
typename VectorConstIterator<Type>::const_iterator
VectorConstIterator<Type>::operator+=(int n) {
  iter = iter + n;
  return iter;
}

/*decrement iterator by n*/
template <typename Type>
typename VectorConstIterator<Type>::const_iterator
VectorConstIterator<Type>::operator-(int n) {
  return (iter - n);
}

/*decrement iterator by n and assing*/
template <typename Type>
typename VectorConstIterator<Type>::const_iterator
VectorConstIterator<Type>::operator-=(int n) {
  iter = iter - n;
  return iter;
}

/*increment iterator*/
template <typename Type>
typename VectorConstIterator<Type>::const_iterator
VectorConstIterator<Type>::operator++(int) {
  return iter++;
}

/*decrement iterator*/
template <typename Type>
typename VectorConstIterator<Type>::const_iterator
VectorConstIterator<Type>::operator--(int) {
  return iter--;
}

/*increment iterator*/
template <typename Type>
typename VectorConstIterator<Type>::const_iterator
VectorConstIterator<Type>::operator++() {
  return ++iter;
}

/*decrement iterator*/
template <typename Type>
typename VectorConstIterator<Type>::const_iterator
VectorConstIterator<Type>::operator--() {
  return --iter;
}

/*unpack iterator*/
template <typename Type>
typename VectorConstIterator<Type>::const_reference
VectorConstIterator<Type>::operator*() const {
  return *iter;
}

/*return true if iterator != another*/
template <typename Type>
bool VectorConstIterator<Type>::operator!=(const const_iterator& another) {
  return this->iter != another.iter;
}

/*return true if iterator == another*/
template <typename Type>
bool VectorConstIterator<Type>::operator==(const const_iterator& another) {
  return this->iter == another.iter;
}

/*return true if iterator > another*/
template <typename Type>
bool VectorConstIterator<Type>::operator>(const const_iterator& another) {
  return this->iter > another.iter;
}

/*return true if iterator >= another*/
template <typename Type>
bool VectorConstIterator<Type>::operator>=(const const_iterator& another) {
  return this->iter >= another.iter;
}

/*return true if iterator < another*/
template <typename Type>
bool VectorConstIterator<Type>::operator<(const const_iterator& another) {
  return this->iter < another.iter;
}

/*return true if iterator < another*/
template <typename Type>
bool VectorConstIterator<Type>::operator<=(const const_iterator& another) {
  return this->iter <= another.iter;
}

/*default constructor, creates empty vector*/
template <typename Type>
S21_Vector<Type>::S21_Vector() : Size(0U), Capacity(0U), ptr(nullptr) {}

/*parameterized constructor, creates the vector of size n*/
template <typename Type>
S21_Vector<Type>::S21_Vector(size_type n)
    : Size(n), Capacity(n), ptr(n ? new value_type[n]{0} : nullptr) {}

/*initializer list constructor, creates vector initizialized using
 * std::initializer_list*/
template <typename Type>
S21_Vector<Type>::S21_Vector(std::initializer_list<Type> const& items) {
  ptr = nullptr;
  ptr = new Type[items.size()]{0};
  int i = 0;
  for (auto it = items.begin(); it != items.end(); it++) {
    ptr[i] = *it;
    i++;
  }
  Size = Capacity = items.size();
};

/*copy constructor*/
template <typename Type>
S21_Vector<Type>::S21_Vector(S21_Vector& another) {
  this->copy(another);
}

/*move constructor*/
template <typename Type>
S21_Vector<Type>::S21_Vector(S21_Vector&& another) {
  this->move(std::move(another));
}

/*destructor*/
template <typename Type>
S21_Vector<Type>::~S21_Vector() {
  clear();
}

template <typename Type>
void S21_Vector<Type>::copy(S21_Vector& another) {
  if (another.Size == 0)
    throw invalid_argument("copy error: unable to copy empty vector");
  this->Size = another.Size;
  this->Capacity = another.Capacity;
  this->ptr = new value_type[this->Capacity]{0};

  memcpy(this->ptr, another.ptr, this->Size * sizeof(Type));
}

template <typename Type>
void S21_Vector<Type>::move(S21_Vector&& another) {
  if (another.Size == 0)
    throw invalid_argument("move error: unable to move empty vector");
  this->Size = another.Size;
  this->Capacity = another.Capacity;
  this->ptr = new value_type[this->Capacity];
  std::move(another.ptr, another.ptr + this->Capacity, this->ptr);
  another.clear();
}

/*clears the contents*/
template <typename Type>
void S21_Vector<Type>::clear() {
  if (this->ptr != nullptr) {
    delete[] ptr;
  }
  ptr = nullptr;
  Size = Capacity = 0U;
}

/*allocate storage of size elements and copies current array elements to a
 * newely allocated array*/
template <typename Type>
void S21_Vector<Type>::reserve(size_type n) {
  if (n > Capacity) {
    value_type* buff = new value_type[n];
    if (ptr != nullptr) {
      for (size_type i = 0; i < this->Size; ++i) {
        buff[i] = ptr[i];
      }
      value_type* tmp = ptr;
      delete[] tmp;
    }
    ptr = buff;
    Capacity = n;
  }
}

/*adds an element to the end*/
template <typename Type>
void S21_Vector<Type>::push_back(const_reference element) {
  if (Size == Capacity) {
    reserve(Size ? Size * 2 : 2);
  }
  ptr[Size++] = element;
}

/*removes the last element*/
template <typename Type>
void S21_Vector<Type>::pop_back() {
  if (ptr == nullptr) throw out_of_range("vector is empty");
  if (this->Size > 1) {
    value_type* new_ptr = new value_type[Size - 1]{0};
    memcpy(new_ptr, ptr, (this->Size - 1) * sizeof(value_type));
    delete[] ptr;
    ptr = new_ptr;
    this->Size = this->Capacity = this->Size - 1;
  } else {
    this->clear();
  }
}

/*access specified element with bounds checking*/
template <typename Type>
Type& S21_Vector<Type>::at(size_type index) {
  if (ptr == nullptr) throw out_of_range("vector is empty");
  if (index > Size - 1) throw out_of_range("index out of bounds");
  return ptr[index];
}

/*access the first element*/
template <typename Type>
const Type& S21_Vector<Type>::front() {
  if (ptr == nullptr) throw out_of_range("vector is empty");
  return this->ptr[0];
}

/*access the last element*/
template <typename Type>
const Type& S21_Vector<Type>::back() {
  if (ptr == nullptr) throw out_of_range("vector is empty");
  return this->ptr[this->Size - 1];
}

/*access specified element*/
template <typename Type>
Type& S21_Vector<Type>::operator[](size_type index) {
  if (ptr == nullptr) throw out_of_range("vector is empty");
  if (index > Size - 1) throw out_of_range("index out of bounds");
  return ptr[index];
}

/*reduces memory usage by freeing unused memory*/
template <typename Type>
void S21_Vector<Type>::shrink_to_fit() {
  if (Capacity > Size) {
    value_type* shrinked = new value_type[Size];
    memcpy(shrinked, ptr, Size * sizeof(value_type));
    delete[] ptr;
    ptr = shrinked;
    Capacity = Size;
  }
}

/*erases element at pos*/
template <typename Type>
void S21_Vector<Type>::erase(iterator pos) {
  if (ptr == nullptr) throw invalid_argument("erase error: vector is empty");
  iterator p_end = this->end();
  while (pos != p_end - 1) {
    *pos = *(pos + 1);
    pos++;
  }
  Size--;
  this->shrink_to_fit();
}

/*swaps the contents*/
template <typename Type>
void S21_Vector<Type>::swap(S21_Vector& another) {
  value_type* tmp_arr = this->ptr;
  size_type tmp_size = this->Size;
  size_type tmp_capacity = this->Capacity;
  this->ptr = another.ptr;
  this->Size = another.Size;
  this->Capacity = another.Capacity;
  another.ptr = tmp_arr;
  another.Size = tmp_size;
  another.Capacity = tmp_capacity;
}

/*inserts elements into concrete pos and returns the iterator that points to the
 * new element*/
template <typename Type>
typename S21_Vector<Type>::iterator S21_Vector<Type>::insert(
    iterator pos, const_reference value) {
  size_type shift = 0;
  if (ptr != nullptr) shift = this->end() - pos;
  if (Capacity == Size) reserve(Capacity ? Capacity * 2 : 2);
  iterator new_pos(nullptr);
  if (Size == 0) {
    ptr[0] = value;
    new_pos = ptr;
  } else {
    iterator p_end = this->end();
    for (size_type i = 0; i < shift; ++i) *p_end-- = *(p_end - 1);
    *p_end = value;
    new_pos = p_end;
  }
  Size++;
  return new_pos;
}

/*returns the maximum possible number of elements*/
template <typename Type>
size_t S21_Vector<Type>::max_size() {
  auto max_allocatable =
      std::numeric_limits<std::size_t>::max() / sizeof(value_type);
  return max_allocatable;
}

/*returns an iterator to the beginning*/
template <typename Type>
typename S21_Vector<Type>::iterator S21_Vector<Type>::begin() {
  return ptr;
}

/*returns an iterator to the end*/
template <typename Type>
typename S21_Vector<Type>::iterator S21_Vector<Type>::end() {
  return (ptr + Size);
}

/*returns an iterator to the end*/
template <typename Type>
typename S21_Vector<Type>::iterator S21_Vector<Type>::rbegin() {
  return (ptr + Size - 1);
}

/*returns an iterator to the beginning*/
template <typename Type>
typename S21_Vector<Type>::iterator S21_Vector<Type>::rend() {
  Type* r_value = nullptr;
  if (ptr != nullptr) r_value = ptr - 1;
  return r_value;
}

/*returns an iterator to the beginning*/
template <typename Type>
typename S21_Vector<Type>::const_iterator S21_Vector<Type>::cbegin() {
  return ptr;
}

/*returns an iterator to the end*/
template <typename Type>
typename S21_Vector<Type>::const_iterator S21_Vector<Type>::cend() {
  return (ptr + Size);
}

/*returns an iterator to the end*/
template <typename Type>
typename S21_Vector<Type>::const_iterator S21_Vector<Type>::crbegin() {
  return (ptr + Size - 1);
}

/*returns an iterator to the beginning*/
template <typename Type>
typename S21_Vector<Type>::const_iterator S21_Vector<Type>::crend() {
  Type* r_value = nullptr;
  if (ptr != nullptr) r_value = ptr - 1;
  return r_value;
}

/*Appends new elements to the end of the container.*/
template <typename Type>
template <typename... Args>
void S21_Vector<Type>::insert_many_back(Args&&... args) {
  ((this->push_back(args)), ...);
}

/*Inserts new elements into the container directly before pos.*/
template <typename Type>
template <typename... Args>
typename S21_Vector<Type>::iterator S21_Vector<Type>::insert_many(
    const_iterator pos, Args&&... args) {
  if ((pos < this->cbegin() || pos >= this->cend()) && pos != nullptr)
    throw out_of_range("insert_many error: VectorIterator _pos out of bounds");
  size_type step = 0;
  size_type new_size = this->Size + sizeof...(args);
  if (this->ptr != nullptr) {
    iterator iter = this->begin(), tmp_iter = this->begin();
    const_iterator citer = this->cbegin();
    while (citer != pos) {
      ++citer;
      ++tmp_iter;
      ++step;
    };
    value_type* new_vector = new value_type[new_size];
    iterator new_iter(new_vector);
    while (iter != this->end()) {
      if (iter == tmp_iter) (..., (*new_iter++ = args));
      *new_iter++ = *iter++;
    }
    delete[] ptr;
    ptr = new_vector;
  } else
    ((this->push_back(args)), ...);

  this->Size = this->Capacity = new_size;
  return (this->begin() + step);
}

};  // namespace s21