#ifndef S21_ARRAY_TPP
#define S21_ARRAY_TPP

namespace s21 {
/// @brief default constructor, creates empty array
/// @tparam Type
/// @tparam N
template <typename Type, std::size_t N>
inline S21_array<Type, N>::S21_array() : size_(N), data_() {}

/// @brief initializer list constructor, creates array initizialized using
/// @tparam Type
/// @tparam N
/// @param items
template <typename Type, size_t N>
inline S21_array<Type, N>::S21_array(std::initializer_list<Type> const &items)
    : size_(items.size()) {
  size_t i = 0;
  for (const auto &item : items) {
    data_[i++] = item;
  }
}

/// @brief copy constructor
/// @tparam Type
/// @tparam N
/// @param a
template <typename Type, size_t N>
inline S21_array<Type, N>::S21_array(const S21_array &a) : size_(a.size_) {
  std::copy(a.data_, a.data_ + N, data_);
}

/// @brief move constructor
/// @tparam Type
/// @tparam N
/// @param a
template <typename Type, size_t N>
inline S21_array<Type, N>::S21_array(S21_array &&a) : size_(a.size_) {
  std::move(a.data_, a.data_ + N, data_);
  a.size_ = 0;
}

/// @brief
/// @tparam Type
/// @tparam N
template <typename Type, size_t N>
inline S21_array<Type, N>::~S21_array() {}

/// @brief assignment operator overload for moving object
/// @param a
template <typename Type, size_t N>
inline S21_array<Type, N> S21_array<Type, N>::operator=(S21_array &&a) {
  if (this != &a) {
    for (size_t i = 0; i < N; ++i) {
      data_[i] = std::move(a.data_[i]);
    }
  }
  a.size_ = 0;
  return *this;
}

/// @brief access specified element with bounds checking
/// @param pos
/// @return
template <typename Type, size_t N>
inline typename S21_array<Type, N>::reference S21_array<Type, N>::at(
    size_type pos) {
  if (pos >= N) {
    throw std::out_of_range("OutOfRangeEror: Index out of array range");
  }
  return data_[pos];
}

/// @brief access specified element
/// @param pos
/// @return
template <typename Type, size_t N>
inline typename S21_array<Type, N>::reference S21_array<Type, N>::operator[](
    size_type pos) {
  if (pos >= N) {
    throw std::out_of_range("OutOfRangeEror: Index out of array range");
  }
  return data_[pos];
}

/// @brief access the first element
/// @return
template <typename Type, size_t N>
inline typename S21_array<Type, N>::const_reference
S21_array<Type, N>::front() {
  return data_[0];
}

/// @brief access the last element
/// @return
template <typename Type, size_t N>
inline typename S21_array<Type, N>::const_reference S21_array<Type, N>::back() {
  return data_[N - 1];
}

/// @brief direct access to the underlying array
/// @return
template <typename Type, size_t N>
inline typename S21_array<Type, N>::iterator S21_array<Type, N>::data() {
  return data_;
}

/// @brief returns an iterator to the beginning
/// @return
template <typename Type, size_t N>
inline typename S21_array<Type, N>::iterator S21_array<Type, N>::begin() {
  return &data_[0];
}

/// @brief returns an iterator to the end
/// @return
template <typename Type, size_t N>
inline typename S21_array<Type, N>::iterator S21_array<Type, N>::end() {
  return &data_[size_];
}

/// @brief checks whether the container is empty
/// @return
template <typename Type, size_t N>
inline bool S21_array<Type, N>::empty() {
  return !size_;
}

/// @brief returns the number of elements
/// @return
template <typename Type, size_t N>
inline typename S21_array<Type, N>::size_type S21_array<Type, N>::size() {
  return size_;
}

/// @brief returns the maximum possible number of elements
/// @return
template <typename Type, size_t N>
inline typename S21_array<Type, N>::size_type S21_array<Type, N>::max_size() {
  return N;
}

/// @brief swaps the contents
/// @param other
template <typename Type, size_t N>
inline void S21_array<Type, N>::swap(S21_array &other) {
  for (size_t i = 0; i < N; ++i) {
    std::swap(data_[i], other.data_[i]);
  }
}

/// @brief assigns the given value value to all elements in the container
/// @param value
template <typename Type, size_t N>
inline void S21_array<Type, N>::fill(const_reference value) {
  for (size_t i = 0; i < N; ++i) {
    data_[i] = value;
  }
}
template <typename Type>
inline ArrayIterator<Type>::ArrayIterator(value_type *iter) : iter(iter) {}

/// @brief gets the element pointed to by the iterator
/// @return
template <typename Type>
inline typename ArrayIterator<Type>::reference
ArrayIterator<Type>::operator*() {
  return *iter;
}

/// @brief moves the iterator forward to the next element
template <typename Type>
inline typename ArrayIterator<Type>::iterator
ArrayIterator<Type>::operator++() {
  return ++iter;
}

/// @brief to the next element
template <typename Type>
inline typename ArrayIterator<Type>::iterator ArrayIterator<Type>::operator++(
    int) {
  return iter++;
}

/// @brief increment iterator by n
/// @param n
template <typename Type>
inline typename ArrayIterator<Type>::iterator ArrayIterator<Type>::operator+(
    int n) {
  return (iter + n);
}

/// @brief increment iterator by n
/// @param n
template <typename Type>
inline typename ArrayIterator<Type>::iterator ArrayIterator<Type>::operator+=(
    int n) {
  return (iter + n);
}

/// @brief moves the iterator backwards to the previous element
/// @return
template <typename Type>
inline typename ArrayIterator<Type>::iterator
ArrayIterator<Type>::operator--() {
  return --iter;
}

/// @brief moves the iterator backwards to the previous element
/// @return
template <typename Type>
inline typename ArrayIterator<Type>::iterator ArrayIterator<Type>::operator--(
    int) {
  return iter--;
}

/// @brief decrement iterator by n
/// @param n
template <typename Type>
inline typename ArrayIterator<Type>::iterator ArrayIterator<Type>::operator-(
    int n) {
  return iter - n;
}

/// @brief decrement iterator by n
/// @param n
template <typename Type>
inline typename ArrayIterator<Type>::iterator ArrayIterator<Type>::operator-=(
    int n) {
  return iter - n;
}

/// @brief return true if iterator == another
/// @param another
template <typename Type>
inline bool ArrayIterator<Type>::operator==(const iterator &another) {
  return this->iter == another->iter;
}

/// @brief return true if iterator != another
/// @param another
template <typename Type>
inline bool ArrayIterator<Type>::operator!=(const iterator &another) {
  return this->iter != another->iter;
}

template <typename Type>
ArrayConstIterator<Type>::ArrayConstIterator(const value_type *iter)
    : iter(iter) {}

/// @brief gets the element pointed to by the const_iterator
/// @return
template <typename Type>
inline typename ArrayConstIterator<Type>::const_reference
ArrayConstIterator<Type>::operator*() const {
  return *iter;
}

/// @brief moves the const_iterator forward to the next element
template <typename Type>
inline typename ArrayConstIterator<Type>::const_iterator
ArrayConstIterator<Type>::operator++() {
  return ++iter;
}

/// @brief to the next element
template <typename Type>
inline typename ArrayConstIterator<Type>::const_iterator
ArrayConstIterator<Type>::operator++(int) {
  return iter++;
}

/// @brief increment const_iterator by n
/// @param n
template <typename Type>
inline typename ArrayConstIterator<Type>::const_iterator
ArrayConstIterator<Type>::operator+(int n) {
  return (iter + n);
}

/// @brief increment const_iterator by n
/// @param n
template <typename Type>
inline typename ArrayConstIterator<Type>::const_iterator
ArrayConstIterator<Type>::operator+=(int n) {
  return (iter + n);
}

/// @brief moves the const_iterator backwards to the previous element
/// @return
template <typename Type>
inline typename ArrayConstIterator<Type>::const_iterator
ArrayConstIterator<Type>::operator--() {
  return --iter;
}

/// @brief moves the const_iterator backwards to the previous element
/// @return
template <typename Type>
inline typename ArrayConstIterator<Type>::const_iterator
ArrayConstIterator<Type>::operator--(int) {
  return iter--;
}

/// @brief decrement const_iterator by n
/// @param n
template <typename Type>
inline typename ArrayConstIterator<Type>::const_iterator
ArrayConstIterator<Type>::operator-(int n) {
  return iter - n;
}

/// @brief decrement const_iterator by n
/// @param n
template <typename Type>
inline typename ArrayConstIterator<Type>::const_iterator
ArrayConstIterator<Type>::operator-=(int n) {
  return iter - n;
}

/// @brief return true if const_iterator == another
/// @param another
template <typename Type>
inline bool ArrayConstIterator<Type>::operator==(
    const const_iterator &another) {
  return this->iter == another->iter;
}

/// @brief return true if const_iterator != another
/// @param another
template <typename Type>
inline bool ArrayConstIterator<Type>::operator!=(
    const const_iterator &another) {
  return this->iter != another->iter;
}

};  // namespace s21

#endif