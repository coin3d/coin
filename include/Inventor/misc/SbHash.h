#ifndef COIN_SBHASH_H
#define COIN_SBHASH_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <assert.h>
#include <stddef.h> // NULL definition
#include <string.h>

// We usually implement inline functions below the class definition,
// since we think that makes the file more readable. However, this is
// not done for this class, since Microsoft Visual C++ is not too
// happy about having functions declared as inline for a template
// class.

// This class (SbHash<Type, Key>) is internal and must not be exposed
// in the API.

template <class Type, class Key>
class SbHashEntry {
public:
  Key key;
  Type * val;
  SbHashEntry<Type, Key> * next;
};

template <class Type, class Key>
class SbHash {
public:
  typedef unsigned long SbHashFunc(const Key key);
  typedef void SbHashApplyFunc(Key key, Type * obj, void * closure);

public:
  SbHash(int size = 256, float loadfactor = 0.0f) {
    if ( loadfactor <= 0.0f ) { loadfactor = 0.75f; }
    unsigned int s = 1;
    while ( s < size ) { s <<= 1; } // power-of-two size
    this->size = s;
    this->elements = 0;
    this->threshold = (unsigned int) (s * loadfactor);
    this->loadfactor = loadfactor;
    this->buckets = new SbHashEntry<Type, Key> * [size];
    this->hashfunc = default_hash_func;
  }

  ~SbHash(void) {
    this->clear();
    delete [] this->buckets;
  }

  void clear(void) {
    unsigned int i;
    for ( i = 0; i < this->size; i++ ) {
      while ( this->buckets[i] ) {
        SbHashEntry<Type, Key> * entry = this->buckets[i];
        this->buckets[i] = entry->next;
        delete entry;
      }
    }
    memset(this->buckets, 0, this->size * sizeof(SbHashEntry<Type, Key> *));
    this->elements = NULL;
  }

  int put(Key key, Type * obj) {
    unsigned int i = this->getIndex(key);
    SbHashEntry<Type, Key> * entry = this->buckets[i];
    while ( entry ) {
      if ( entry->key == key ) { // Replace the old value
        entry->val = obj;
        return FALSE;
      }
      entry = entry->next;
    }

    // Key not already in the hash table; insert a new
    // entry as the first element in the bucket
    entry = new SbHashEntry<Type, Key>;
    entry->key = key;
    entry->val = val;
    entry->next = this->buckets[i];
    this->buckets[i] = entry;

    if ( this->elements++ >= this->threshold ) { this->resize(this->size * 2); }
    return TRUE;
  }

  int get(Key key, Type *& obj) const {
    SbHashEntry<Type, Key> * entry;
    unsigned int i = this->getIndex(key);
    entry = this->buckets[i];
    while ( entry ) {
      if ( entry->key == key ) {
        *obj = entry->val;
        return TRUE;
      }
      entry = entry->next;
    }
    return FALSE;
  }

  int remove(Key key) {
    unsigned int i = this->getIndex(key);
    SbHashEntry<Type, Key> * entry = this->buckets[i], * next, * prev = NULL;
    while ( entry ) {
      next = entry->next;
      if ( entry->key == key ) {
        this->elements--;
        if ( prev == NULL) {
          this->buckets[i] = next;
        }
        else {
          prev->next = next;
        }
        delete entry;
        return TRUE;
      }
      prev = entry;
      entry = next;
    }
    return FALSE;
  }

  void apply(SbHashApplyFunc * func, void * closure) {
    unsigned int i;
    SbHashEntry<Type, Key> * elem;
    for ( i = 0; i < this->size; i++ ) {
      elem = this->buckets[i];
      while ( elem ) {
        func(elem->key, elem->val, closure);
        elem = elem->next;
      }
    }
  }

  unsigned int getnumElements(void) const { return this->elements; }

  void setHashFunc(SbHashFunc * func) { this->hashfunc = func; }

  void getStats(int & buckets_used, int & buckets, int & elements, float & chain_length_avg, int & chain_length_max) {
    unsigned int i;
    buckets_used = 0, chain_length_max = 0;
    for ( i = 0; i < this->size; i++ ) {
      if ( this->buckets[i] ) {
        unsigned int chain_l = 0;
        SbHashEntry<Type, Key> * entry = this->buckets[i];
        buckets_used++;
        while ( entry ) {
          chain_l++;
          entry = entry->next;
        }
        if ( chain_l > chain_length_max ) { chain_length_max = chain_l; }
      }
    }
    buckets = this->size;
    elements = this->elements;
    chain_length_avg = (float) this->elements / buckets_used;
  }

protected:
  static unsigned long default_hash_func(const Key key) {
    return (unsigned long) key;
  }

  unsigned int getIndex(const Key key) {
    key = this->hashfunc(key);
    key -= (key << 7); /* i.e. key = key * -127; */
    return key & (this->size - 1);
  }

  void resize(unsigned int newsize) {
    if (this->size >= newsize) return; // we don't shrink the table
    // assert(coin_is_power_of_two(newsize));

    unsigned int oldsize = this->size;
    SbHashEntry<Type, Key> ** oldbuckets = this->buckets;

    this->size = newsize;
    this->elements = 0;
    this->threshold = (unsigned int) (newsize * this->loadfactor);
    this->buckets = new SbHashEntry<Type, Key> * [newsize];

    // Transfer all mappings
    int i;
    for ( i = 0; i < oldsize; i++ ) {
      SbHashEntry<Type, Key> * entry = oldbuckets[i];
      while ( entry ) {
        this->put(entry->key, entry->val);
        entry = entry->next;
      }
    }
    delete [] oldbuckets;
  }

private:
  float loadfactor;
  unsigned int size;
  unsigned int elements;
  unsigned int threshold;
  SbHashEntry<Type, Key> ** buckets;
  SbHashFunc * hashfunc;

};

#endif // !COIN_SBHASH_H
