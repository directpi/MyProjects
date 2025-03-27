namespace s21 {

/*map iterator constructor*/
template <typename Key, typename Type>
S21_Map<Key, Type>::MapIterator::MapIterator(
    Node<key_type, mapped_type>* node) {
  this->node = node;
}

/*map iterator operator *
returns pair <Key, Value>*/
template <typename Key, typename Type>
typename S21_Map<Key, Type>::value_type
S21_Map<Key, Type>::MapIterator::operator*() {
  return {node->first, node->second

  };
}

/*map iterator operator ->
gives direct access to the node*/
template <typename Key, typename Type>
typename S21_Map<Key, Type>::template Node<Key, Type>*
S21_Map<Key, Type>::MapIterator::operator->() {
  return node;
}

/*map iterator increment*/
template <typename Key, typename Type>
typename S21_Map<Key, Type>::iterator
S21_Map<Key, Type>::MapIterator::operator++() {
  if (node->pRight) {
    node = node->pRight;
    while (node->pLeft) node = node->pLeft;
  } else {
    while (node->Parent && node == node->Parent->pRight) {
      node = node->Parent;
    }
    node = node->Parent;
  }
  return *this;
}

/*map iterator decrement*/
template <typename Key, typename Type>
typename S21_Map<Key, Type>::iterator
S21_Map<Key, Type>::MapIterator::operator--() {
  if (node->pLeft) {
    node = node->pLeft;
    while (node->pRight) node = node->pRight;
  } else {
    while (node->Parent && node == node->Parent->pLeft) {
      node = node->Parent;
    }
    node = node->Parent;
  }
  return *this;
}

/*map operator ==
compares two iterators*/
template <typename Key, typename Type>
bool S21_Map<Key, Type>::MapIterator::operator==(
    const iterator& iterator) const {
  return this->node == iterator.node;
}

/*map operator !=
compares two iterators*/
template <typename Key, typename Type>
bool S21_Map<Key, Type>::MapIterator::operator!=(
    const iterator& iterator) const {
  return this->node != iterator.node;
}

/*returns node*/
template <typename Key, typename Type>
typename S21_Map<Key, Type>::template Node<Key, Type>*
S21_Map<Key, Type>::MapIterator::get_node() {
  return this->node;
}

/*map const iterator constructor*/
template <typename Key, typename Type>
S21_Map<Key, Type>::MapConstIterator::MapConstIterator(
    const Node<key_type, mapped_type>* node) {
  this->node = node;
}

/*const map const iterator operator *
returns pair <Key, Value>*/
template <typename Key, typename Type>
typename S21_Map<Key, Type>::value_type
S21_Map<Key, Type>::MapConstIterator::operator*() {
  return {node->first, node->second};
}

/*map const iterator increment*/
template <typename Key, typename Type>
typename S21_Map<Key, Type>::const_iterator
S21_Map<Key, Type>::MapConstIterator::operator++() {
  if (node->pRight) {
    node = node->pRight;
    while (node->pLeft) node = node->pLeft;
  } else {
    while (node->Parent && node == node->Parent->pRight) {
      node = node->Parent;
    }
    node = node->Parent;
  }
  return *this;
}

/*map iterator decrement*/
template <typename Key, typename Type>
typename S21_Map<Key, Type>::const_iterator
S21_Map<Key, Type>::MapConstIterator::operator--() {
  if (node->pLeft) {
    node = node->pLeft;
    while (node->pRight) node = node->pRight;
  } else {
    while (node->Parent && node == node->Parent->pLeft) {
      node = node->Parent;
    }
    node = node->Parent;
  }
  return *this;
}

/*map operator ==
compares two iterators*/
template <typename Key, typename Type>
bool S21_Map<Key, Type>::MapConstIterator::operator==(
    const const_iterator& iterator) const {
  return this->node == iterator.node;
}

/*map operator !=
compares two iterators*/
template <typename Key, typename Type>
bool S21_Map<Key, Type>::MapConstIterator::operator!=(
    const const_iterator& iterator) const {
  return this->node != iterator.node;
}

template <typename Key, typename Type>
S21_Map<Key, Type>::S21_Map() {
  root = nullptr;
  Size = 0U;
}

template <typename Key, typename Type>
S21_Map<Key, Type>::S21_Map(std::initializer_list<value_type> const& items) {
  root = nullptr;
  Size = 0U;
  if (items.size()) {
    auto it = items.begin();
    while (it != items.end()) {
      value_type pair = *it++;
      this->insert(pair.first, pair.second);
    }
  }
}

/*copy constructor*/
template <typename Key, typename Type>
S21_Map<Key, Type>::S21_Map(const S21_Map& another) {
  if (another.root == nullptr)
    throw invalid_argument("cannot copy from an empty map");
  root = nullptr;
  Size = 0U;
  Node<key_type, mapped_type>* node = another.root;
  this->copy(node);
}

/*move constructor*/
template <typename Key, typename Type>
S21_Map<Key, Type>::S21_Map(S21_Map&& another) {
  this->move(std::move(another));
}

/*destructor*/
template <typename Key, typename Type>
S21_Map<Key, Type>::~S21_Map() {
  this->clear();
}

/*assignment operator overload for moving object*/
template <typename Key, typename Type>
void S21_Map<Key, Type>::operator=(S21_Map&& another) {
  this->move(std::move(another));
}

/*moves from another map*/
template <typename Key, typename Type>
void S21_Map<Key, Type>::move(S21_Map&& another) {
  if (another.root == nullptr)
    throw invalid_argument("cannot move from an empty map");
  this->root = another.root;
  this->Size = another.Size;
  another.root = nullptr;
  another.Size = 0U;
}

/*make copy from another map*/
template <typename Key, typename Type>
void S21_Map<Key, Type>::copy(const Node<Key, Type>* node) {
  this->insert(node->first, node->second);
  // const Node<key_type, mapped_type>* tmp_node = node;
  if (node->pLeft) this->copy(node->pLeft);
  if (node->pRight) this->copy(node->pRight);
}

/*clears the contents*/
template <typename Key, typename Type>
void S21_Map<Key, Type>::clear() {
  if (this->root != nullptr) {
    Node<key_type, mapped_type>* node = this->root;
    recursive_clear(node);
    this->root = nullptr;
    this->Size = 0U;
  }
}

template <typename Key, typename Type>
void S21_Map<Key, Type>::recursive_clear(const Node<Key, Type>* node) {
  if (node->pLeft) recursive_clear(node->pLeft);
  if (node->pRight) recursive_clear(node->pRight);
  delete node;
}

/*access specified element with bounds checking*/
template <typename Key, typename Type>
Type& S21_Map<Key, Type>::at(const Key& key) {
  if (!contains(key)) throw std::out_of_range("key does not exist");
  iterator iter_node = this->root;
  std::pair<iterator, bool> pair(this->root, false);
  pair = recursive_search(iter_node, key);
  return pair.first->second;
}

/*access or insert specified element*/
template <typename Key, typename Type>
Type& S21_Map<Key, Type>::operator[](const Key& key) {
  iterator iter_node = this->root;
  std::pair<iterator, bool> pair(this->root, false);
  if (!contains(key)) {
    pair = insert(key, 0);
  } else {
    pair = recursive_search(iter_node, key);
  }
  return pair.first->second;
}

/*recursively searches for element by key, returns iterator*/
template <typename Key, typename Type>
std::pair<typename S21_Map<Key, Type>::iterator, bool>
S21_Map<Key, Type>::recursive_search(iterator iter_node, const Key& key) {
  // bool found = (iter_node->first == key) ? true : false;
  std::pair<iterator, bool> pair(iter_node,
                                 (iter_node->first == key) ? true : false);

  if (!pair.second && iter_node->pLeft)
    pair = recursive_search(iter_node->pLeft, key);
  if (!pair.second && iter_node->pRight)
    pair = recursive_search(iter_node->pRight, key);
  return pair;
}

/*returns an iterator to the beginning*/
template <typename Key, typename Type>
typename S21_Map<Key, Type>::iterator S21_Map<Key, Type>::begin() {
  Node<key_type, mapped_type>* node = this->root;
  if (node)
    while (node->pLeft != nullptr) node = node->pLeft;

  return node;
}

/*returns an iterator to the end*/
template <typename Key, typename Type>
typename S21_Map<Key, Type>::iterator S21_Map<Key, Type>::end() {
  return nullptr;
}

/*checks whether the container is empty*/
template <typename Key, typename Type>
bool S21_Map<Key, Type>::empty() {
  return root == nullptr ? true : false;
}

/*returns the number of elements*/
template <typename Key, typename Type>
typename S21_Map<Key, Type>::size_type S21_Map<Key, Type>::size() {
  return this->Size;
}

/*returns the maximum possible number of elements*/
template <typename Key, typename Type>
typename S21_Map<Key, Type>::size_type S21_Map<Key, Type>::max_size() {
  return std::numeric_limits<std::size_t>::max() /
         sizeof(std::pair<const Key, Type>);
}

/*inserts value by key and returns iterator to where the element is in the
 * container and bool denoting whether the insertion took place*/
template <typename Key, typename Type>
std::pair<typename S21_Map<Key, Type>::iterator, bool>
S21_Map<Key, Type>::insert(const key_type& key, const Type& value) {
  std::pair<iterator, bool> result(nullptr, false);
  if (this->root == nullptr) {
    this->root = new Node<key_type, mapped_type>({key, value});
    this->root->color = BLACK;
    result.first = this->root;
    result.second = true;
  } else {
    Node<key_type, mapped_type>* current = this->root;
    result = recursive_insert(current, key, value);
  }
  if (result.second) ++Size;
  return result;
}

/*recurcively insert pair to map and balance the RBtree*/
template <typename Key, typename Type>
std::pair<typename S21_Map<Key, Type>::iterator, bool>
S21_Map<Key, Type>::recursive_insert(Node<Key, Type>* node, const key_type& key,
                                     const Type& value) {
  std::pair<iterator, bool> result(nullptr, false);
  if (key < node->first) {
    if (node->pLeft)
      result = recursive_insert(node->pLeft, key, value);
    else {
      node->pLeft = new Node<key_type, mapped_type>({key, value});
      node->pLeft->Parent = node;
      result = {node->pLeft, true};
    }
  } else if (key > node->first) {
    if (node->pRight)
      result = recursive_insert(node->pRight, key, value);
    else {
      node->pRight = new Node<key_type, mapped_type>({key, value});
      node->pRight->Parent = node;
      result = {node->pRight, true};
    }
  }
  balance_tree(node);
  return result;
}

/*balances RBtree according to current node*/
template <typename Key, typename Type>
void S21_Map<Key, Type>::balance_tree(Node<Key, Type>* node) {
  // LEFT TURN
  if (node->pRight) {
    if (node->pRight->color == RED &&
        (!node->pLeft || node->pLeft->color == BLACK)) {
      if (node->Parent && node->Parent->pLeft == node)
        node->Parent->pLeft = node->pRight;
      if (node->Parent && node->Parent->pRight == node)
        node->Parent->pRight = node->pRight;
      Node<key_type, mapped_type>* tmp = node->pRight->pLeft;
      node->pRight->Parent = node->Parent;
      node->Parent = node->pRight;
      node->pRight->pLeft = node;
      node->pRight = tmp;
      if (tmp) tmp->Parent = node;
      if (node == root) root = node->Parent;
      node->color = RED;
      node->Parent->color = BLACK;
    }
  }
  // RIGHT TURN
  if (node->pLeft) {
    if (node->pLeft->color == RED &&
        (node->pLeft->pLeft && node->pLeft->pLeft->color == RED)) {
      if (node->Parent && node->Parent->pLeft == node)
        node->Parent->pLeft = node->pLeft;
      if (node->Parent && node->Parent->pRight == node)
        node->Parent->pRight = node->pLeft;
      Node<key_type, mapped_type>* tmp = node->pLeft->pRight;
      node->pLeft->Parent = node->Parent;
      node->Parent = node->pLeft;
      node->pLeft->pRight = node;
      node->pLeft = tmp;
      if (node == root) root = node->Parent;
      node->color = BLACK;
      if (node->Parent == root) node->Parent->color = BLACK;
      node->Parent->pLeft->color = BLACK;
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

/*checks if there is an element with key equivalent to key in the container*/
template <typename Key, typename Type>
bool S21_Map<Key, Type>::contains(const Key& key) {
  bool result = false;
  if (this->root != nullptr) {
    Node<key_type, mapped_type>* node = this->root;
    result = recursive_contains(key, node);
  }
  return result;
}

template <typename Key, typename Type>
bool S21_Map<Key, Type>::recursive_contains(const Key& key,
                                            const Node<Key, Type>* node) {
  bool found = (node->first == key) ? true : false;
  if (!found && node->pLeft) found = recursive_contains(key, node->pLeft);
  if (!found && node->pRight) found = recursive_contains(key, node->pRight);
  return found;
}

/*inserts node and returns iterator to where the element is in the container and
 * bool denoting whether the insertion took place*/
template <typename Key, typename Type>
std::pair<typename S21_Map<Key, Type>::iterator, bool>
S21_Map<Key, Type>::insert(const value_type& value) {
  return this->insert(value.first, value.second);
}

/*inserts an element or assigns to the current element if the key already
 * exists*/
template <typename Key, typename Type>
std::pair<typename S21_Map<Key, Type>::iterator, bool>
S21_Map<Key, Type>::insert_or_assign(const Key& key, const Type& value) {
  std::pair<iterator, bool> result(nullptr, false);
  if (contains(key)) {
    result = recursive_search(this->root, key);
    result.first->second = value;
  } else {
    result = insert(key, value);
  }
  return result;
}

/*erases element at pos*/
template <typename Key, typename Type>
void S21_Map<Key, Type>::erase(iterator pos) {
  if (this->root == nullptr) throw out_of_range("erase error: map is empty");
  Node<key_type, mapped_type>* node = pos.get_node();
  Node<key_type, mapped_type>* swap_node = node;

  if (node->pLeft && node->pRight) {
    swap_node = node->pRight;
    while (swap_node->pLeft != nullptr) swap_node = swap_node->pLeft;

    value_type pair_tmp(node->first, node->second);
    node->first = swap_node->first;
    node->second = swap_node->second;
    swap_node->first = pair_tmp.first;
    swap_node->second = pair_tmp.second;
  }

  if (swap_node->color == RED) {
    if (!swap_node->pLeft && !swap_node->pRight) {
      if (swap_node->Parent->pLeft == swap_node)
        swap_node->Parent->pLeft = nullptr;
      if (swap_node->Parent->pRight == swap_node)
        swap_node->Parent->pRight = nullptr;
      delete swap_node;
    }
  } else {
    if (swap_node->pLeft) {
      swap_node->first = swap_node->pLeft->first;
      swap_node->second = swap_node->pLeft->second;
      delete swap_node->pLeft;
      swap_node->pLeft = nullptr;
    } else if (swap_node->pRight) {
      swap_node->first = swap_node->pRight->first;
      swap_node->second = swap_node->pRight->second;
      delete swap_node->pRight;
      swap_node->pRight = nullptr;
    } else {
      if (swap_node->Parent && swap_node->Parent->pLeft == swap_node)
        swap_node->Parent->pLeft = nullptr;
      if (swap_node->Parent && swap_node->Parent->pRight == swap_node)
        swap_node->Parent->pRight = nullptr;
      if (swap_node->Parent)
        balance_tree(swap_node->Parent);
      else
        this->root = nullptr;
      delete swap_node;
    }
  }
  this->Size--;
}

/*swaps the contents*/
template <typename Key, typename Type>
void S21_Map<Key, Type>::swap(S21_Map& another) {
  Node<Key, Type>* tmp_root = this->root;
  size_type tmp_size = this->Size;

  this->root = another.root;
  this->Size = another.Size;

  another.root = tmp_root;
  another.Size = tmp_size;
}

/*splices nodes from another container*/
template <typename Key, typename Type>
void S21_Map<Key, Type>::merge(S21_Map& another) {
  iterator iter = another.begin();

  while (iter != another.end()) {
    std::pair<Key, Type> p = *iter;
    if (!this->contains(p.first)) {
      this->insert(p);
      another.erase(iter);
      iter = another.begin();
    } else
      ++iter;
  }
}

/*Inserts new elements into the container*/
template <typename Key, typename Type>
template <typename... Args>
S21_Vector<std::pair<typename S21_Map<Key, Type>::iterator, bool>>
S21_Map<Key, Type>::insert_many(Args&&... args) {
  S21_Vector<std::pair<iterator, bool>> vct;
  if (sizeof...(args))
    (vct.push_back(this->insert(args)), ...);
  else
    vct.push_back(make_pair(nullptr, false));
  return vct;
}

};  // namespace s21