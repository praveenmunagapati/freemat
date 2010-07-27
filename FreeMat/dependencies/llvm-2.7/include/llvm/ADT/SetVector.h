//===- llvm/ADT/SetVector.h - Set with insert order iteration ---*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements a set that has insertion order iteration
// characteristics. This is useful for keeping a set of things that need to be
// visited later but in a deterministic order (insertion order). The interface
// is purposefully minimal.
//
// This file defines SetVector and SmallSetVector, which performs no allocations
// if the SetVector has less than a certain number of elements.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_ADT_SETVECTOR_H
#define LLVM_ADT_SETVECTOR_H

#include "llvm/ADT/SmallSet.h"
#include <algorithm>
#include <cassert>
#include <vector>

namespace llvm {

/// This adapter class provides a way to keep a set of things that also has the
/// property of a deterministic iteration order. The order of iteration is the
/// order of insertion.
/// @brief A vector that has set insertion semantics.
template <typename T, typename Vector = std::vector<T>,
                      typename Set = SmallSet<T, 16> >
class SetVector {
public:
  typedef T value_type;
  typedef T key_type;
  typedef T& reference;
  typedef const T& const_reference;
  typedef Set set_type;
  typedef Vector vector_type;
  typedef typename vector_type::const_iterator iterator;
  typedef typename vector_type::const_iterator const_iterator;
  typedef typename vector_type::size_type size_type;

  /// @brief Construct an empty SetVector
  SetVector() {}

  /// @brief Initialize a SetVector with a range of elements
  template<typename It>
  SetVector(It Start, It End) {
    insert(Start, End);
  }

  /// @brief Determine if the SetVector is empty or not.
  bool empty() const {
    return vector_.empty();
  }

  /// @brief Determine the number of elements in the SetVector.
  size_type size() const {
    return vector_.size();
  }

  /// @brief Get an iterator to the beginning of the SetVector.
  iterator begin() {
    return vector_.begin();
  }

  /// @brief Get a const_iterator to the beginning of the SetVector.
  const_iterator begin() const {
    return vector_.begin();
  }

  /// @brief Get an iterator to the end of the SetVector.
  iterator end() {
    return vector_.end();
  }

  /// @brief Get a const_iterator to the end of the SetVector.
  const_iterator end() const {
    return vector_.end();
  }

  /// @brief Return the last element of the SetVector.
  const T &back() const {
    assert(!empty() && "Cannot call back() on empty SetVector!");
    return vector_.back();
  }

  /// @brief Index into the SetVector.
  const_reference operator[](size_type n) const {
    assert(n < vector_.size() && "SetVector access out of range!");
    return vector_[n];
  }

  /// @returns true iff the element was inserted into the SetVector.
  /// @brief Insert a new element into the SetVector.
  bool insert(const value_type &X) {
    bool result = set_.insert(X);
    if (result)
      vector_.push_back(X);
    return result;
  }

  /// @brief Insert a range of elements into the SetVector.
  template<typename It>
  void insert(It Start, It End) {
    for (; Start != End; ++Start)
      if (set_.insert(*Start))
        vector_.push_back(*Start);
  }

  /// @brief Remove an item from the set vector.
  void remove(const value_type& X) {
    if (set_.erase(X)) {
      typename vector_type::iterator I =
        std::find(vector_.begin(), vector_.end(), X);
      assert(I != vector_.end() && "Corrupted SetVector instances!");
      vector_.erase(I);
    }
  }


  /// @returns 0 if the element is not in the SetVector, 1 if it is.
  /// @brief Count the number of elements of a given key in the SetVector.
  size_type count(const key_type &key) const {
    return set_.count(key);
  }

  /// @brief Completely clear the SetVector
  void clear() {
    set_.clear();
    vector_.clear();
  }

  /// @brief Remove the last element of the SetVector.
  void pop_back() {
    assert(!empty() && "Cannot remove an element from an empty SetVector!");
    set_.erase(back());
    vector_.pop_back();
  }

private:
  set_type set_;         ///< The set.
  vector_type vector_;   ///< The vector.
};

/// SmallSetVector - A SetVector that performs no allocations if smaller than
/// a certain size.
template <typename T, unsigned N>
class SmallSetVector : public SetVector<T, SmallVector<T, N>, SmallSet<T, N> > {
public:
  SmallSetVector() {}

  /// @brief Initialize a SmallSetVector with a range of elements
  template<typename It>
  SmallSetVector(It Start, It End) {
    this->insert(Start, End);
  }
};

} // End llvm namespace

// vim: sw=2 ai
#endif
