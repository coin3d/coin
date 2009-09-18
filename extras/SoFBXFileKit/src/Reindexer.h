#ifndef REINDEXER_H_
#define REINDEXER_H_

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <unordered_map>
#include <functional>
#include <vector>

/*!
  Helper container for the Reindexer.
  Use this when the size of the reindexed elements is not known at compile-time.
*/
template <typename T>
struct Tuple {
  void push_back(const T &val) { v.push_back(val); }

  bool operator==(const Tuple &other) const {
    return (this->v == other.v);
  }
  const T & operator[](std::size_t i) const {
    return this->v[i];
  }

  std::size_t hash_value() const {
    std::size_t hashval = 0;
    typename std::vector<T>::const_iterator iter;
    int size = sizeof(T)/sizeof(std::size_t);
    for (iter = this->v.begin();
         iter != this->v.end();
         iter++) {
      const T &baseval = *iter;
      const std::size_t * ptr = reinterpret_cast<const std::size_t *>(&baseval);
      for (int i = 0; i < size; i++) {
        hashval ^= ptr[i];
      }
    }
    return hashval;
  }

  std::vector<T> v;
};

// Helper function for Boost's hash function. Called automatically from Boost.
template <typename T>
std::size_t hash_value(const Tuple<T> &val) {
  return val.hash_value();
}

/*!
  Reindexes a collection of elements of type T.
  Typically used to compress an element array by creating and reusing indexes to 
  a new array or to merge two index tables to two arrays into a common index.
  The latter is necessary for VBO's or for unifying texture coordinate indices to
  multiple texture coordinate arrays.
*/
template<typename T>
class Reindexer
{
public:
  /*!
    Looks up a value. Will insert the value if it doesn't already exist.
    Returns the new index. */
  int lookup(const T &val) {
    typename std::tr1::unordered_map<T, int>::const_iterator iter = this->map.find(val);
    if (iter != this->map.end()) return iter->second;
    else {
      this->map.insert(std::make_pair(val, this->map.size()));
      return this->map.size() - 1;
    }
  }

  /*!
    Returns the current size of the new element array
  */
  std::size_t size() const {
    return this->map.size();
  }

  /*!
    Return the new element array.
  */
  const T *getArray() {
    this->vec.resize(this->map.size());
    typename std::tr1::unordered_map<T, int>::const_iterator iter = this->map.begin();
    while (iter != this->map.end()) {
      this->vec[iter->second] = iter->first;
      iter++;
    }
    return &this->vec[0];
  }

private:
  std::tr1::unordered_map<T, int> map;
  std::vector<T> vec;
};


/*
Example:


struct BaseType {
  BaseType() {  }
  BaseType(float a, float b, float c) { setValue(a,b,c); }
  void setValue(float a, float b, float c) { v[0]=a;v[1]=b;v[2]=c; }
  float v[3];

  bool operator==(const BaseType &other) const {
    return (this->v[0] == other.v[0] &&
            this->v[1] == other.v[1] &&
            this->v[2] == other.v[2]);
  }
};

int main()
{
  Reindexer<Tuple<BaseType> > map;

  BaseType t[10];
  t[0].setValue(0,9,0.0);
  t[1].setValue(1,8,0.9);
  t[2].setValue(2,7,0.8);
  t[3].setValue(3,6,0.7);
  t[4].setValue(4,5,0.6);
  t[5].setValue(5,4,0.5);
  t[6].setValue(6,3,0.4);
  t[7].setValue(0,9,0.0);
  t[8].setValue(8,1,0.2);
  t[9].setValue(9,0,0.1);
  Tuple<BaseType> my[10];

  for (int i=0;i<10;i++) {
    my[i].push_back(t[i]);
  }

  for (int i=0;i<10;i++) {
    int idx = map.lookup(my[i]);
    printf("Lookup: %d %d (hash=%d)\n", i, idx, hash_value(my[i]));
  }

  const Tuple<BaseType> *ptr = map.getArray();
  for (int i=0;i<map.size();i++) {
    printf("%d: (%f %f %f)\n", i, ptr[i].v[0].v[0], ptr[i].v[0].v[1], ptr[i].v[0].v[2]);
  }

}
*/

#endif
