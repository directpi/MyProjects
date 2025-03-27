#ifndef S21_SET_TPP
#define S21_SET_TPP

namespace s21 {

/// @brief make copy from another set
/// @tparam Key
/// @param node
template <typename Key>
inline void S21_Set<Key>::copy(const Node<Key> *node) {
  this->insert(node->data);
  if (node->pLeft) this->copy(node->pLeft);
  if (node->pRight) this->copy(node->pRight);
}

/// @brief moves from another set
/// @tparam Key
/// @param another
template <typename Key>
inline void S21_Set<Key>::move(S21_Set &&another) {
  if (another.root == nullptr) {
    throw std::invalid_argument("cannot move from an empty set");
  }
  root = nullptr;
  Size = 0U;
  Node<Key> *node = another.root;
  this->copy(node);
  another.clear();
}

/// @brief default constructor, creates empty set
/// @tparam Key
template <typename Key>
inline S21_Set<Key>::S21_Set() {
  root = nullptr;
  Size = 0U;
}

/// @brief initializer list constructor, creates the set initizialized using
/// std::initializer_list
/// @tparam Key
/// @param items
template <typename Key>
inline S21_Set<Key>::S21_Set(std::initializer_list<value_type> const &items) {
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
inline S21_Set<Key>::S21_Set(const S21_Set &another) {
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
inline S21_Set<Key>::S21_Set(S21_Set &&another) {
  this->move(std::move(another));
}

/// @brief destructor
/// @tparam Key
template <typename Key>
inline S21_Set<Key>::~S21_Set() {
  this->clear();
}

/// @brief assignment operator overload for moving object
/// @tparam Key
/// @param another
template <typename Key>
inline void S21_Set<Key>::operator=(S21_Set<Key> &&another) {
  this->move(std::move(another));
}

/// @brief returns an iterator to the beginning
/// @tparam Key
/// @return
template <typename Key>
inline typename S21_Set<Key>::iterator S21_Set<Key>::begin() {
  Node<Key> *node = this->root;
  if (node)
    while (node->pLeft != nullptr) node = node->pLeft;
  return node;
}

/// @brief returns an iterator to the end
/// @tparam Key
/// @return
template <typename Key>
inline typename S21_Set<Key>::iterator S21_Set<Key>::end() {
  return nullptr;
}

/// @brief checks whether the container is empty
/// @tparam Key
/// @return
template <typename Key>
inline bool S21_Set<Key>::empty() {
  return root == nullptr ? true : false;
}

/// @brief returns the number of elements
/// @tparam Key
/// @return
template <typename Key>
inline typename S21_Set<Key>::size_type S21_Set<Key>::size() {
  return this->Size;
}

/// @brief returns the maximum possible number of elements
/// @tparam Key
/// @return
template <typename Key>
inline typename S21_Set<Key>::size_type S21_Set<Key>::max_size() {
  return std::numeric_limits<std::size_t>::max() / sizeof(Key);
}

/// @brief clears the contents
/// @tparam Key
template <typename Key>
inline void S21_Set<Key>::clear() {
  if (this->root != nullptr) {
    Node<Key> *node = this->root;
    recursive_clear(node);
    this->root = nullptr;
    this->Size = 0U;
  }
}

/// @brief inserts node and returns iterator to where the element is in the
/// container and bool denoting whether the insertion took place
/// @tparam Key
/// @param value
/// @return
template <typename Key>
inline std::pair<typename S21_Set<Key>::iterator, bool> S21_Set<Key>::insert(
    const value_type &value) {
  std::pair<iterator, bool> result(nullptr, false);
  if (this->root == nullptr) {
    this->root = new Node<Key>(value);
    this->root->color = BLACK;
    result.first = this->root;
    result.second = true;
  } else {
    Node<Key> *current = this->root;
    result = recursive_insert(current, value);
  }
  if (result.second) ++Size;
  return result;
}
/// @brief recursively inserts node and returns iterator to where the element is
/// in the container and bool denoting whether the insertion took place
/// @tparam Key
/// @param node
/// @param value
/// @return
template <typename Key>
inline std::pair<typename S21_Set<Key>::iterator, bool>
S21_Set<Key>::recursive_insert(Node<Key> *node, const value_type &value) {
  std::pair<iterator, bool> result(nullptr, false);
  if (value < node->data) {
    if (node->pLeft)
      result = recursive_insert(node->pLeft, value);
    else {
      node->pLeft = new Node<Key>(value);
      node->pLeft->parent = node;
      result = {node->pLeft, true};
    }
  } else if (value > node->data) {
    if (node->pRight)
      result = recursive_insert(node->pRight, value);
    else {
      node->pRight = new Node<Key>(value);
      node->pRight->parent = node;
      result = {node->pRight, true};
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
bool S21_Set<Key>::contains(const Key &key) {
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
inline bool S21_Set<Key>::recursive_contains(const Key &key,
                                             const Node<Key> *node) {
  bool found = (node->data == key) ? true : false;
  if (!found && node->pLeft) found = recursive_contains(key, node->pLeft);
  if (!found && node->pRight) found = recursive_contains(key, node->pRight);
  return found;
}

/// @brief finds element with specific key
/// @tparam Key
/// @param key
/// @return
template <typename Key>
typename S21_Set<Key>::iterator S21_Set<Key>::find(const Key &key) {
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
std::pair<typename S21_Set<Key>::iterator, bool> S21_Set<Key>::recursive_search(
    iterator iter_node, const Key &key) {
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
inline void S21_Set<Key>::erase(iterator pos) {
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
      swap_node->data = swap_node->pLeft->data;
      delete swap_node->pLeft;
      swap_node->pLeft = nullptr;
    } else if (swap_node->pRight) {
      swap_node->data = swap_node->pRight->data;
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
inline void S21_Set<Key>::swap(S21_Set &another) {
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
inline void S21_Set<Key>::merge(S21_Set &another) {
  iterator iter = another.begin();
  while (iter != another.end()) {
    Key p = *iter;
    if (!this->contains(p)) {
      this->insert(p);
      another.erase(iter);
      iter = another.begin();
    } else
      ++iter;
  }
}

/// @brief balances RBtree according to current node
/// @tparam Key
/// @param node
template <typename Key>
inline void S21_Set<Key>::balance_tree(Node<Key> *node) {
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
inline void S21_Set<Key>::recursive_clear(const Node<Key> *node) {
  if (node->pLeft) recursive_clear(node->pLeft);
  if (node->pRight) recursive_clear(node->pRight);
  delete node;
}

/// @brief set iterator operator -> gives direct access to the node
/// @tparam Key
/// @return
template <typename Key>
typename S21_Set<Key>::template Node<Key>
    *S21_Set<Key>::SetIterator::operator->() {
  return node;
}

/// @brief iterator operator* returns Value
/// @tparam Key
/// @return
template <typename Key>
inline typename S21_Set<Key>::value_type
S21_Set<Key>::SetIterator::operator*() {
  return this->node->data;
}

/// @brief Set iterator increment
/// @tparam Key
/// @return
template <typename Key>
typename S21_Set<Key>::iterator S21_Set<Key>::SetIterator::operator++() {
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
/// @brief Set iterator decrement
/// @tparam Key
/// @return
template <typename Key>
typename S21_Set<Key>::iterator S21_Set<Key>::SetIterator::operator--() {
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
/// @brief Set operator == compares two iterators

/// @tparam Key
/// @param iterator
/// @return
template <typename Key>
bool S21_Set<Key>::SetIterator::operator==(const iterator &iterator) const {
  return this->node == iterator.node;
}
/// @brief Set operator != compares two iterators
/// @tparam Key
/// @param iterator
/// @return
template <typename Key>
bool S21_Set<Key>::SetIterator::operator!=(const iterator &iterator) const {
  return this->node != iterator.node;
}
/// @brief returns node
/// @tparam Key
/// @return
template <typename Key>
typename S21_Set<Key>::template Node<Key>
    *S21_Set<Key>::SetIterator::get_node() {
  return this->node;
}

/// @brief const set const iterator operator
/// @tparam Key
/// @return
template <typename Key>
inline typename S21_Set<Key>::value_type
S21_Set<Key>::SetConstIterator::operator*() {
  return this->node->data;
}

/// @brief set const iterator increment
/// @tparam Key
/// @return
template <typename Key>
typename S21_Set<Key>::const_iterator
S21_Set<Key>::SetConstIterator::operator++() {
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
/// @brief set const iterator decrement
/// @tparam Key
/// @return
template <typename Key>
typename S21_Set<Key>::const_iterator
S21_Set<Key>::SetConstIterator::operator--() {
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
/// @brief set operator == compares two iterators
/// @tparam Key
/// @param iterator
/// @return
template <typename Key>
bool S21_Set<Key>::SetConstIterator::operator==(
    const const_iterator &iterator) const {
  return this->node == iterator.node;
}
/// @brief set operator != compares two iterators
/// @tparam Key
/// @param iterator
/// @return
template <typename Key>
bool S21_Set<Key>::SetConstIterator::operator!=(
    const const_iterator &iterator) const {
  return this->node != iterator.node;
}
/// @brief Inserts new elements into the container
/// @tparam Key
/// @param ...args
/// @return
template <typename Key>
template <typename... Args>
S21_Vector<std::pair<typename S21_Set<Key>::iterator, bool>>
S21_Set<Key>::insert_many(Args &&...args) {
  S21_Vector<std::pair<iterator, bool>> vct;
  if (sizeof...(args))
    (vct.push_back(this->insert(args)), ...);
  else
    vct.push_back(std::make_pair(nullptr, false));
  return vct;
}

};  // namespace s21
#endif