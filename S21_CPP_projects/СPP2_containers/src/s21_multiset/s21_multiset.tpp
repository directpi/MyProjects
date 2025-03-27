#ifndef S21_MULTISET_TPP
#define S21_MULTISET_TPP

namespace s21 {

/// @brief make copy from another multiset
/// @tparam Key
/// @param node
template <typename Key>
inline void S21_Multiset<Key>::copy(const Node<Key> *node) {
  this->insert(node->data);
  if (node->pLeft) this->copy(node->pLeft);
  if (node->pRight) this->copy(node->pRight);
}

/// @brief moves from another multiset
/// @tparam Key
/// @param another
template <typename Key>
inline void S21_Multiset<Key>::move(S21_Multiset &&another) {
  if (another.root == nullptr) {
    throw std::invalid_argument("cannot move from an empty set");
  }
  root = nullptr;
  Size = 0U;
  Node<Key> *node = another.root;
  this->copy(node);
  another.clear();
}

/// @brief default constructor, creates empty multiset
/// @tparam Key
template <typename Key>
inline S21_Multiset<Key>::S21_Multiset() {
  root = nullptr;
  Size = 0U;
}

/// @brief initializer list constructor, creates the multiset initizialized
/// using std::initializer_list
/// @tparam Key
/// @param items
template <typename Key>
inline S21_Multiset<Key>::S21_Multiset(
    std::initializer_list<value_type> const &items) {
  root = nullptr;
  Size = 0U;
  if (items.size()) {
    auto it = items.begin();
    while (it != items.end()) {
      value_type my_val = *it++;
      this->insert(my_val);
    }
  }
}

/// @brief copy constructor
/// @tparam Key
/// @param another
template <typename Key>
inline S21_Multiset<Key>::S21_Multiset(const S21_Multiset &another) {
  if (another.root == nullptr) {
    throw std::invalid_argument("cannot copy from an empty set");
  }
  root = nullptr;
  Size = 0U;
  Node<Key> *node = another.root;
  this->copy(node);
}

/// @brief move constructor
/// @tparam Key
/// @param another
template <typename Key>
inline S21_Multiset<Key>::S21_Multiset(S21_Multiset &&another) {
  this->move(std::move(another));
}

/// @brief destructor
/// @tparam Key
template <typename Key>
inline S21_Multiset<Key>::~S21_Multiset() {
  this->clear();
}

/// @brief assignment operator overload for moving object
/// @tparam Key
/// @param another
template <typename Key>
inline void S21_Multiset<Key>::operator=(S21_Multiset<Key> &&another) {
  this->move(std::move(another));
}

/// @brief returns an iterator to the beginning
/// @tparam Key
/// @return
template <typename Key>
inline typename S21_Multiset<Key>::iterator S21_Multiset<Key>::begin() {
  Node<Key> *node = this->root;
  if (node)
    while (node->pLeft != nullptr) node = node->pLeft;
  return node;
}

/// @brief returns an iterator to the end
/// @tparam Key
/// @return
template <typename Key>
inline typename S21_Multiset<Key>::iterator S21_Multiset<Key>::end() {
  return nullptr;
}

/// @brief checks whether the container is empty
/// @tparam Key
/// @return
template <typename Key>
inline bool S21_Multiset<Key>::empty() {
  return root == nullptr ? true : false;
}

/// @brief returns the number of elements
/// @tparam Key
/// @return
template <typename Key>
inline typename S21_Multiset<Key>::size_type S21_Multiset<Key>::size() {
  return this->Size;
}

/// @brief returns the maximum possible number of elements
/// @tparam Key
/// @return
template <typename Key>
inline typename S21_Multiset<Key>::size_type S21_Multiset<Key>::max_size() {
  return std::numeric_limits<std::size_t>::max() / sizeof(Key);
}

/// @brief clears the contents
/// @tparam Key
template <typename Key>
inline void S21_Multiset<Key>::clear() {
  if (this->root != nullptr) {
    Node<Key> *node = this->root;
    recursive_clear(node);
    this->root = nullptr;
    this->Size = 0U;
  }
}

/// @brief inserts node and returns iterator to where the element is in the
/// container
/// @tparam Key
/// @param value
/// @return
template <typename Key>
inline typename S21_Multiset<Key>::iterator S21_Multiset<Key>::insert(
    const value_type &value) {
  iterator result(nullptr);
  if (this->root == nullptr) {
    this->root = new Node<Key>(value);
    this->root->color = BLACK;
    result = this->root;
  } else {
    Node<Key> *current = this->root;
    result = recursive_insert(current, value);
  }
  if (result != nullptr) ++Size;
  return result;
}
/// @brief recursively inserts node and returns iterator to where the element is
/// in the container
/// @tparam Key
/// @param node
/// @param value
/// @return
template <typename Key>
inline typename S21_Multiset<Key>::iterator S21_Multiset<Key>::recursive_insert(
    Node<Key> *node, const value_type &value) {
  iterator result(nullptr);
  if (value <= node->data) {
    if (node->pLeft)
      result = recursive_insert(node->pLeft, value);
    else {
      node->pLeft = new Node<Key>(value);
      node->pLeft->parent = node;
      result = node->pLeft;
    }
  } else {
    if (node->pRight)
      result = recursive_insert(node->pRight, value);
    else {
      node->pRight = new Node<Key>(value);
      node->pRight->parent = node;
      result = node->pRight;
    }
  }
  balance_tree(node);
  return result;
}

/// @brief checks if the container contains element with specific key
/// @tparam Key
/// @param key
/// @return
template <typename Key>
bool S21_Multiset<Key>::contains(const Key &key) {
  if (this->root == nullptr) throw std::out_of_range("Set is empty");
  Node<Key> *node = this->root;
  return recursive_contains(key, node);
}

/// @brief recursively checks if the container contains element with specific
/// key
/// @tparam Key
/// @param key
/// @param node
/// @return
template <typename Key>
inline bool S21_Multiset<Key>::recursive_contains(const Key &key,
                                                  const Node<Key> *node) {
  bool found = (node->data == key) ? true : false;
  if (!found && node->pLeft) found = recursive_contains(key, node->pLeft);
  if (!found && node->pRight) found = recursive_contains(key, node->pRight);
  return found;
}

/// @brief returns the number of elements matching specific key
/// @param key
/// @return
template <typename Key>
inline typename S21_Multiset<Key>::size_type S21_Multiset<Key>::count(
    const Key &key) {
  size_type result = 0U;
  iterator iter = this->begin();
  while (iter != this->end()) {
    if (*iter == key) ++result;
    ++iter;
  }
  return result;
}

/// @brief returns range of elements matching a specific key
/// @param key
/// @return
template <typename Key>
std::pair<typename S21_Multiset<Key>::iterator,
          typename S21_Multiset<Key>::iterator>
S21_Multiset<Key>::equal_range(const Key &key) {
  std::pair<iterator, iterator> result(nullptr, nullptr);
  iterator iter = this->begin();
  bool found = false;
  while (iter != this->end()) {
    if (!found && *iter == key) {
      result.first = iter;
      found = true;
    }
    if (found && *iter != key) {
      result.second = iter;
      found = false;
    }
    ++iter;
  }
  return result;
}

/// @brief returns an iterator to the first element not less than the given key
/// @param key
/// @return
template <typename Key>
typename S21_Multiset<Key>::iterator S21_Multiset<Key>::lower_bound(
    const Key &key) {
  std::pair<iterator, iterator> result(nullptr, nullptr);
  result = this->equal_range(key);

  return result.first;
}

/// @brief returns an iterator to the first element greater than the given key
/// @param key
/// @return
template <typename Key>
typename S21_Multiset<Key>::iterator S21_Multiset<Key>::upper_bound(
    const Key &key) {
  std::pair<iterator, iterator> result(nullptr, nullptr);
  result = this->equal_range(key);

  return result.second;
}

/// @brief finds element with specific key
/// @tparam Key
/// @param key
/// @return
template <typename Key>
typename S21_Multiset<Key>::iterator S21_Multiset<Key>::find(const Key &key) {
  if (this->root == nullptr) throw std::out_of_range("Set is empty");
  std::pair<iterator, bool> it_pair(nullptr, false);
  if (contains(key)) {
    iterator iter_node = this->root;
    it_pair = recursive_search(iter_node, key);
  }
  return it_pair.first;
}
/// @brief recursively searches for element by key, returns iterator
/// @tparam Key
/// @param iter_node
/// @param key
/// @return
template <typename Key>
std::pair<typename S21_Multiset<Key>::iterator, bool>
S21_Multiset<Key>::recursive_search(iterator iter_node, const Key &key) {
  std::pair<iterator, bool> pair(iter_node,
                                 (iter_node->data == key) ? true : false);
  if (!pair.second && iter_node->pLeft)
    pair = recursive_search(iter_node->pLeft, key);
  if (!pair.second && iter_node->pRight)
    pair = recursive_search(iter_node->pRight, key);
  return pair;
}

/// @brief erases element at pos
/// @tparam Key
/// @param pos
template <typename Key>
inline void S21_Multiset<Key>::erase(iterator pos) {
  if (this->root == nullptr)
    throw std::out_of_range("Erase error: set is empty");
  Node<Key> *node = pos.get_node();
  Node<Key> *swap_node = node;

  if (node->pLeft && node->pRight) {
    swap_node = node->pRight;
    while (swap_node->pLeft != nullptr) swap_node = swap_node->pLeft;

    value_type tmp = node->data;
    node->data = swap_node->data;
    swap_node->data = tmp;
  }

  if (swap_node->color == RED) {
    if (!swap_node->pLeft && !swap_node->pRight) {
      if (swap_node->parent->pLeft == swap_node)
        swap_node->parent->pLeft = nullptr;
      if (swap_node->parent->pRight == swap_node)
        swap_node->parent->pRight = nullptr;
      delete swap_node;
    }
  } else {
    if (swap_node->pLeft) {
      // value_type tmp = node->data;
      swap_node->data = swap_node->pLeft->data;
      delete swap_node->pLeft;
      swap_node->pLeft = nullptr;
    } else if (swap_node->pRight) {
      swap_node->data = swap_node->pRight->data;
      // swap_node->second = swap_node->pRight->second;
      delete swap_node->pRight;
      swap_node->pRight = nullptr;
    } else {
      if (swap_node->parent && swap_node->parent->pLeft == swap_node)
        swap_node->parent->pLeft = nullptr;
      if (swap_node->parent && swap_node->parent->pRight == swap_node)
        swap_node->parent->pRight = nullptr;
      if (swap_node->parent)
        balance_tree(swap_node->parent);
      else
        this->root = nullptr;
      delete swap_node;
    }
  }
  this->Size--;
}

/// @brief swaps the contents
/// @tparam Key
/// @param another
template <typename Key>
inline void S21_Multiset<Key>::swap(S21_Multiset &another) {
  Node<Key> *tmp_root = this->root;
  size_type tmp_size = this->Size;

  this->root = another.root;
  this->Size = another.Size;
  another.root = tmp_root;
  another.Size = tmp_size;
}

/// @brief splices nodes from another container
/// @tparam Key
/// @param another
template <typename Key>
inline void S21_Multiset<Key>::merge(S21_Multiset &another) {
  iterator iter = another.begin();
  while (iter != another.end()) {
    Key p = *iter;
    // if (!this->contains(p)) {
    this->insert(p);
    another.erase(iter);
    iter = another.begin();
    // } else
    // ++iter;
  }
}

/// @brief balances RBtree according to current node
/// @tparam Key
/// @param node
template <typename Key>
inline void S21_Multiset<Key>::balance_tree(Node<Key> *node) {
  // LEFT TURN
  if (node->pRight) {
    if (node->pRight->color == RED &&
        (!node->pLeft || node->pLeft->color == BLACK)) {
      if (node->parent && node->parent->pLeft == node)
        node->parent->pLeft = node->pRight;
      if (node->parent && node->parent->pRight == node)
        node->parent->pRight = node->pRight;
      Node<Key> *tmp = node->pRight->pLeft;
      node->pRight->parent = node->parent;
      node->parent = node->pRight;
      node->pRight->pLeft = node;
      node->pRight = tmp;
      if (tmp) tmp->parent = node;
      if (node == root) root = node->parent;
      node->color = RED;
      node->parent->color = BLACK;
    }
  }
  // RIGHT TURN
  if (node->pLeft) {
    if (node->pLeft->color == RED &&
        (node->pLeft->pLeft && node->pLeft->pLeft->color == RED)) {
      if (node->parent && node->parent->pLeft == node)
        node->parent->pLeft = node->pLeft;
      if (node->parent && node->parent->pRight == node)
        node->parent->pRight = node->pLeft;
      Node<Key> *tmp = node->pLeft->pRight;
      node->pLeft->parent = node->parent;
      node->parent = node->pLeft;
      node->pLeft->pRight = node;
      node->pLeft = tmp;
      if (node == root) root = node->parent;
      node->color = BLACK;
      if (node->parent == root) node->parent->color = BLACK;
      node->parent->pLeft->color = BLACK;
    }
  }
  // COLORS SWAP
  if (node->pLeft && node->pRight) {
    if (node->pLeft->color == RED && node->pRight->color == RED) {
      node->pLeft->color = node->pRight->color = BLACK;
      if (node != this->root) node->color = RED;
    }
  }
}

/// @brief recursively clears the contents
/// @tparam Key
/// @param node
template <typename Key>
inline void S21_Multiset<Key>::recursive_clear(const Node<Key> *node) {
  if (node->pLeft) recursive_clear(node->pLeft);
  if (node->pRight) recursive_clear(node->pRight);
  delete node;
}

/// @brief set iterator operator -> gives direct access to the node
/// @tparam Key
/// @return
template <typename Key>
typename S21_Multiset<Key>::template Node<Key>
    *S21_Multiset<Key>::MultisetIterator::operator->() {
  return node;
}

/// @brief iterator operator* returns Value
/// @tparam Key
/// @return
template <typename Key>
inline typename S21_Multiset<Key>::value_type
S21_Multiset<Key>::MultisetIterator::operator*() {
  return this->node->data;
}

/// @brief MultiSet iterator increment
/// @tparam Key
/// @return
template <typename Key>
typename S21_Multiset<Key>::iterator
S21_Multiset<Key>::MultisetIterator::operator++() {
  if (node->pRight) {
    node = node->pRight;
    while (node->pLeft) node = node->pLeft;
  } else {
    while (node->parent && node == node->parent->pRight) {
      node = node->parent;
    }
    node = node->parent;
  }
  return *this;
}
/// @brief MultiSet iterator decrement
/// @tparam Key
/// @return
template <typename Key>
typename S21_Multiset<Key>::iterator
S21_Multiset<Key>::MultisetIterator::operator--() {
  if (node->pLeft) {
    node = node->pLeft;
    while (node->pRight) node = node->pRight;
  } else {
    while (node->parent && node == node->parent->pLeft) {
      node = node->parent;
    }
    node = node->parent;
  }
  return *this;
}
/// @brief MultiSet operator == compares two iterators
/// @tparam Key
/// @param iterator
/// @return
template <typename Key>
bool S21_Multiset<Key>::MultisetIterator::operator==(
    const iterator &iterator) const {
  return this->node == iterator.node;
}
/// @brief MultiSet operator != compares two iterators
/// @tparam Key
/// @param iterator
/// @return
template <typename Key>
bool S21_Multiset<Key>::MultisetIterator::operator!=(
    const iterator &iterator) const {
  return this->node != iterator.node;
}
/// @brief returns node
/// @tparam Key
/// @return
template <typename Key>
typename S21_Multiset<Key>::template Node<Key>
    *S21_Multiset<Key>::MultisetIterator::get_node() {
  return this->node;
}

/// @brief const multiset const iterator operator
/// @tparam Key
/// @return
template <typename Key>
inline typename S21_Multiset<Key>::value_type
S21_Multiset<Key>::MultisetConstIterator::operator*() {
  return this->node->data;
}

/// @brief multiset const iterator increment
/// @tparam Key
/// @return
template <typename Key>
typename S21_Multiset<Key>::const_iterator
S21_Multiset<Key>::MultisetConstIterator::operator++() {
  if (node->pRight) {
    node = node->pRight;
    while (node->pLeft) node = node->pLeft;
  } else {
    while (node->parent && node == node->parent->pRight) {
      node = node->parent;
    }
    node = node->parent;
  }
  return *this;
}
/// @brief multiset const iterator decrement
/// @tparam Key
/// @return
template <typename Key>
typename S21_Multiset<Key>::const_iterator
S21_Multiset<Key>::MultisetConstIterator::operator--() {
  if (node->pLeft) {
    node = node->pLeft;
    while (node->pRight) node = node->pRight;
  } else {
    while (node->parent && node == node->parent->pLeft) {
      node = node->parent;
    }
    node = node->parent;
  }
  return *this;
}
/// @brief multiset operator == compares two iterators
/// @tparam Key
/// @param iterator
/// @return
template <typename Key>
bool S21_Multiset<Key>::MultisetConstIterator::operator==(
    const const_iterator &iterator) const {
  return this->node == iterator.node;
}
/// @brief multiset operator != compares two iterators
/// @tparam Key
/// @param iterator
/// @return
template <typename Key>
bool S21_Multiset<Key>::MultisetConstIterator::operator!=(
    const const_iterator &iterator) const {
  return this->node != iterator.node;
}
/// @brief Inserts new elements into the container
/// @tparam Key
/// @param ...args
/// @return
template <typename Key>
template <typename... Args>
S21_Vector<std::pair<typename S21_Multiset<Key>::iterator, bool>>
S21_Multiset<Key>::insert_many(Args &&...args) {
  S21_Vector<std::pair<iterator, bool>> vct;
  if (sizeof...(args))
    (vct.push_back(std::make_pair(this->insert(args), true)), ...);
  else
    vct.push_back(std::make_pair(nullptr, false));
  return vct;
}

};  // namespace s21
#endif