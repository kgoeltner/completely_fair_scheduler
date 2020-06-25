//
// Karl Goeltner
// 917006087
// ECS 36C - 05/22/2020
//
// multimap.h - Implementation of the multimap ADT using a LLRB Tree
// Public API: Size, Get, Contains, Max, Min,
//             Insert, Remove, Print
// Iterative Helper: Get
// Recursive Helpers: Min, Insert, Remove, Print
// Self-Balancing Helpers: IsRed, FlipColors, RotateRight, RotateLeft,
//                         FixUp, MoveRedRight, MoveRedLeft, DeleteMin
//

#ifndef MULTIMAP_H_
#define MULTIMAP_H_

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <deque>

template <typename K, typename V>
class Multimap {
 public:
  // Return size of tree
  unsigned int Size();
  // Return value associated to @key
  const V& Get(const K& key);
  // Return whether @key is found in tree
  bool Contains(const K& key);
  // Return max key in tree
  const K& Max();
  // Return min key in tree
  const K& Min();
  // Insert @key in tree
  void Insert(const K &key, const V &value);
  // Remove @key from tree
  void Remove(const K &key);
  // Print tree in-order
  void Print();

 private:
  enum Color { RED, BLACK };

  // Node updated to contain @key with a list of @values
  // Order of variables also changed for initialization
  struct Node {
    bool color;
    K key;
    std::deque<V> values;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
  };
  std::unique_ptr<Node> root;
  unsigned int cur_size = 0;

  // Iterative helper methods
  Node* Get(Node *n, const K &key);

  // Recursive helper methods
  Node* Min(Node *n);
  void Insert(std::unique_ptr<Node> &n, const K &key, const V &value);
  void Remove(std::unique_ptr<Node> &n, const K &key);
  void Print(Node *n);

  // Helper methods for the self-balancing
  bool IsRed(Node *n);
  void FlipColors(Node *n);
  void RotateRight(std::unique_ptr<Node> &prt);
  void RotateLeft(std::unique_ptr<Node> &prt);
  void FixUp(std::unique_ptr<Node> &n);
  void MoveRedRight(std::unique_ptr<Node> &n);
  void MoveRedLeft(std::unique_ptr<Node> &n);
  void DeleteMin(std::unique_ptr<Node> &n);
};

// Size - return current size of multimap
template <typename K, typename V>
unsigned int Multimap<K, V>::Size() {
  return cur_size;
}

// Get - call helper method to attain @value stored @key;
template <typename K, typename V>
const V& Multimap<K, V>::Get(const K &key) {
  // Start at root node and begin binary traversal with helper
  Node *n = Get(root.get(), key);
  // Ensure that key is found
  if (!n)
    throw std::runtime_error("Error: cannot find key");
  // return the first element
  return n->values.front();
}

// HELPER METHOD - traverse until node @key is found or not
//                 updated to return first element in values list
template <typename K, typename V>
typename Multimap<K, V>::Node* Multimap<K, V>::Get(Node *n, const K &key) {
  // Loop through using binary search for @key
  while (n) {
    // IF key matches
    if (key == n->key)
      return n;
    // Key on LEFT
    if (key < n->key)
      n = n->left.get();
    // Key on RIGHT
    else
      n = n->right.get();
  }
  return nullptr;
}

// Contains - uses get helper to check if @key is found
template <typename K, typename V>
bool Multimap<K, V>::Contains(const K &key) {
  return Get(root.get(), key) != nullptr;
}

// Max - iterative traversal right to attain max @key
template <typename K, typename V>
const K& Multimap<K, V>::Max(void) {
  Node *n = root.get();
  // Start at root and go all right
  while (n->right) n = n->right.get();
  return n->key;
}

// Min - call helper method to attain min @key
template <typename K, typename V>
const K& Multimap<K, V>::Min(void) {
  return Min(root.get())->key;
}

// HELPER METHOD - traverse all the way left for min node
template <typename K, typename V>
typename Multimap<K, V>::Node* Multimap<K, V>::Min(Node *n) {
  if (n->left)
    return Min(n->left.get());
  else
    return n;
}

// IsRed - check if current node is red
template <typename K, typename V>
bool Multimap<K, V>::IsRed(Node *n) {
  // NIL nodes are black
  if (!n) return false;
  // Regular nodes
  return (n->color == RED);
}

// FlipColors - case 2, inverting colors of current node & children
template <typename K, typename V>
void Multimap<K, V>::FlipColors(Node *n) {
  n->color = !n->color;
  n->left->color = !n->left->color;
  n->right->color = !n->right->color;
}

// RotateRight - perform standard right rotation
template <typename K, typename V>
void Multimap<K, V>::RotateRight(std::unique_ptr<Node> &prt) {
  // Obtain left child
  std::unique_ptr<Node> chd = std::move(prt->left);
  // Give original parent child's right
  prt->left = std::move(chd->right);
  chd->color = prt->color;
  prt->color = RED;
  // Child & parent invert positions
  chd->right = std::move(prt);
  prt = std::move(chd);
}

// RotateLeft - perform standard left rotation
template <typename K, typename V>
void Multimap<K, V>::RotateLeft(std::unique_ptr<Node> &prt) {
  // Obtain right child
  std::unique_ptr<Node> chd = std::move(prt->right);
  // Give original parent child's left
  prt->right = std::move(chd->left);
  chd->color = prt->color;
  prt->color = RED;
  // Child & parent invert positions
  chd->left = std::move(prt);
  prt = std::move(chd);
}

// FixUp - recursion traversal back up the RB-Tree;
//         handle (3a) simple rot, (3b) complex rot, (2) recoloring
template <typename K, typename V>
void Multimap<K, V>::FixUp(std::unique_ptr<Node> &n) {
  // Rotate left if there is a right-leaning red node
  if (IsRed(n->right.get()) && !IsRed(n->left.get()))
    RotateLeft(n);
  // Rotate right if red-red pair of nodes on left
  if (IsRed(n->left.get()) && IsRed(n->left->left.get()))
    RotateRight(n);
  // Recoloring if both children are red
  if (IsRed(n->left.get()) && IsRed(n->right.get()))
    FlipColors(n.get());
}

// MoveRedRight - search path goes RIGHT, operate if left-left child red
template <typename K, typename V>
void Multimap<K, V>::MoveRedRight(std::unique_ptr<Node> &n) {
  // Flip current node & children colors
  FlipColors(n.get());
  // If left-left child is RED, RR, Flip
  if (IsRed(n->left->left.get())) {
    RotateRight(n);
    FlipColors(n.get());
  }
}

// MoveRedLeft - search path goes LEFT, operate if right-left child red
template <typename K, typename V>
void Multimap<K, V>::MoveRedLeft(std::unique_ptr<Node> &n) {
  // Flip current node & children colors
  FlipColors(n.get());
  // If right-left child is RED, RR, RL, Flip
  if (IsRed(n->right->left.get())) {
    RotateRight(n->right);
    RotateLeft(n);
    FlipColors(n.get());
  }
}

// DeleteMin - delete min node and recurse back up to restore RB
template <typename K, typename V>
void Multimap<K, V>::DeleteMin(std::unique_ptr<Node> &n) {
  // No left child, min is 'n'
  if (!n->left) {
    // Remove n
    n = nullptr;
    return;
  }
  // Push red link down if necessary
  if (!IsRed(n->left.get()) && !IsRed(n->left->left.get()))
    MoveRedLeft(n);
  // Continue traversing down left
  DeleteMin(n->left);
  // Fix restructuring & recoloring
  FixUp(n);
}

// Remove - call helper method to remove @key & @value pair
template <typename K, typename V>
void Multimap<K, V>::Remove(const K &key) {
  // Check to make sure multimap contains @key
  if (!Contains(key))
    return;
  // Go about with removal
  Remove(root, key);
  // Decrement Multimap size and make root black
  cur_size--;
  if (root)
    root->color = BLACK;
}

// HELPER METHOD - remove node with @key & @value at appropriate position;
//                 updated to handle a list of values
template <typename K, typename V>
void Multimap<K, V>::Remove(std::unique_ptr<Node> &n, const K &key) {
  // Key not found
  if (!n) return;

  // (1) LEFT case
  if (key < n->key) {
    // Left = BLACK, Left-Left = BLACK, search path goes LEFT
    if (!IsRed(n->left.get()) && !IsRed(n->left->left.get()))
      MoveRedLeft(n);
    // Keep recursing LEFT
    Remove(n->left, key);
  // (2) RIGHT or EQUAL case
  } else {
    // Left = RED
    if (IsRed(n->left.get()))
      RotateRight(n);
    // EQUAL - *at bottom*, delete key-value pair
    if (key == n->key && !n->right) {
      // a) Remove 1 key-value pair
      if (n->values.size() > 1)
        n->values.pop_front();
      // b) Remove entire node
      else
        n = nullptr;
      return;
    }
    // Right = BLACK, Right-Left = BLACK, search path goes RIGHT
    if (!IsRed(n->right.get()) && !IsRed(n->right->left.get()))
      MoveRedRight(n);
    // EQUAL - *not at bottom*
    if (key == n->key) {
      // a) Remove 1 key-value pair
      if (n->values.size() > 1) {
        n->values.pop_front();
      // b) Replace by copying content from min node
      } else {
        // Find MIN node in the right subtree, delete it
        Node *n_min = Min(n->right.get());
        n->key = n_min->key;
        n->values.swap(n_min->values);
        DeleteMin(n->right);
      }
    } else {
      // Keep recursing RIGHT
      Remove(n->right, key);
    }
  }
  // Recurse back up and perform additional restructuring & recoloring
  FixUp(n);
}

// Insert - call helper method to insert @key with @value
template <typename K, typename V>
void Multimap<K, V>::Insert(const K &key, const V &value) {
  Insert(root, key, value);
  // Update current size and make root black
  cur_size++;
  root->color = BLACK;
}

// HELPER METHOD - insert node with @key & @value at appropriate position;
//                 updated to handle a list of values
template <typename K, typename V>
void Multimap<K, V>::Insert(std::unique_ptr<Node> &n,
                       const K &key, const V &value) {
  // INSERT HERE -> no node present, add value to vector
  if (!n) {
    // Initialize with only color & key value, vector already created
    n = std::unique_ptr<Node>(new Node{RED, key});
    n->values.push_back(value);
  // Go LEFT -> node is smaller
  } else if (key < n->key) {
    Insert(n->left, key, value);
  // Go RIGHT -> node is greater
  } else if (key > n->key) {
    Insert(n->right, key, value);
  // @key already exists, push new value at end of list
  } else {
    n->values.push_back(value);
  }
  // Recurse back up and perform additional restructuring & recoloring
  FixUp(n);
}

// Print - call helper method to print out all @key & @value pairs in in-order
template <typename K, typename V>
void Multimap<K, V>::Print() {
  Print(root.get());
  std::cout << std::endl;
}

// HELPER METHOD - recurse LNR for in-order traversal printing of @key & @value;
//                 updated to print full list of values upon @key
template <typename K, typename V>
void Multimap<K, V>::Print(Node *n) {
  if (!n) return;
  Print(n->left.get());
  // Print out each key-value pair
  for (auto i : n->values)
    std::cout << "<" << n->key << "," << i << "> ";
  Print(n->right.get());
}

#endif  // MULTIMAP_H_
