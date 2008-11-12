#ifndef COIN_SBHASH_H
#define COIN_SBHASH_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
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

// *************************************************************************
// This class (SbHash<Type, Key>) is internal and must not be exposed
// in the Coin API.

/**
   This function object is an extension of the STL concept "binary function".
   It is a goal to be as similar as possible to our STL counterpart.
*/
//FIXME Move this concept to a separate header. BFG 20081112
template < typename ARG_ONE, typename ARG_TWO, typename ARG_THREE, typename RETTYPE >
  struct trinary_function {
  typedef ARG_ONE first_argument_type;
  typedef ARG_TWO second_argument_type;
  typedef ARG_THREE third_argument_type;
  typedef RETTYPE result_type;

  //RETTYPE operator()(ARG_ONE x, ARG_TWO y, ARG_THREE z);
};


#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* ! COIN_INTERNAL */

// *************************************************************************

#include <assert.h>
#include <stddef.h> // NULL
#include <string.h> // memset()

#include <Inventor/lists/SbList.h>
#include <Inventor/C/base/memalloc.h>

#include "tidbitsp.h"
#include "coindefs.h"
#include "SbBasicP.h"

// *************************************************************************

// We usually implement inline functions below the class definition,
// since we think that makes the file more readable. However, this is
// not done for this class, since Microsoft Visual C++ is not too
// happy about having functions declared as inline for a template
// class.

// *************************************************************************

template <class Type, class Key>
class SbHashEntry {
public:

  void * operator new(size_t COIN_UNUSED(size), cc_memalloc * memhandler) {
    SbHashEntry<Type, Key> * entry = static_cast<SbHashEntry<Type, Key> *>(
      cc_memalloc_allocate(memhandler));
    entry->memhandler = memhandler;
    return static_cast<void *>(entry);
  }
  void operator delete(void * ptr, cc_memalloc * memhandler) {
    cc_memalloc_deallocate(memhandler, ptr);
  }
  void operator delete(void * ptr) {
    SbHashEntry<Type, Key> * entry = static_cast<SbHashEntry<Type, Key> *>( ptr);
    cc_memalloc_deallocate(entry->memhandler, ptr);
  }
  SbHashEntry(const Key & key, const Type & obj) : key(key), obj(obj) {}

  Key key;
  Type obj;
  SbHashEntry<Type, Key> * next;
  cc_memalloc * memhandler;
};

// *************************************************************************

//Create an uint of an arbitrary length datatype
template <class T>
inline unsigned int toUint(T in) {
  if (sizeof(T)>sizeof(unsigned int)) {
    T retVal=in;
    for (size_t i = sizeof(T)/sizeof(unsigned int)-1; i>0; i--) {
      retVal^=in>>(i * 8 * sizeof(unsigned int));
    }
    return static_cast<unsigned int>(retVal);
  }
  else {
    return static_cast<unsigned int>(in);
  }
}

#if !defined(_MSC_VER) || (_MSC_VER >= 1300) // 'long long' not in vc6
#ifndef COIN_INTERNAL //Not available for internal use, as this is not
                    //available on all platforms.
inline unsigned int SbHashFunc(unsigned long long key) { return toUint<unsigned long long>(key); }
#endif //COIN_INTERNAL
#endif

/* Windows 64-bit uses the LLP64 type model, where int and long
 * are 32-bit and a pointer is 64-bit. Most Unices use the LP64
 * where int is 32-bit and long and pointer are 64-bit. */
/* FIXME: the following solution is a kludge. 20081001 tamer. */
#if defined(_WIN64)
inline unsigned int SbHashFunc(unsigned long long key) { return toUint<unsigned long long>(key); }
#else
//The identity hash function
inline unsigned int SbHashFunc(unsigned int key) { return key; }

//Some implementation of other basetypes
inline unsigned int SbHashFunc(int key) { return static_cast<unsigned int>(key); }

inline unsigned int SbHashFunc(unsigned long key) { return toUint<unsigned long>(key); }
#endif

//String has its own implementation
class SbString;
unsigned int SbHashFunc(const SbString & key);

/*
  Some implementations of pointers, all functions are per writing only reinterpret_casts to size_t
*/
//FIXME: Don't hold these definitions here, but where they are used - BFG 20080729
class SoBase;
class SoOutput;
class SoSensor;
unsigned int SbHashFunc(const SoBase * key);
unsigned int SbHashFunc(const SoOutput * key);
unsigned int SbHashFunc(const SoSensor * key);

template <class Type, class Key>
class SbHash {
 public:
  template <typename DType>
    struct ApplyFunctor : public trinary_function<Key,Type,DType, void> {
    virtual void operator()(Key & key, Type & obj, DType closure) = 0;
  };

 public:

  SbHash(unsigned int sizearg = 256, float loadfactorarg = 0.0f)
  {
    this->commonConstructor(sizearg, loadfactorarg);
  }

  SbHash(const SbHash & from)
  {
    this->commonConstructor(from.size, from.loadfactor);
    this->operator=(from);
  }

  SbHash & operator=(const SbHash & from)
  {
    this->clear();
    copy_data functor;
    from.apply(functor, this);
    return *this;
  }

  ~SbHash()
  {
    this->clear();
    cc_memalloc_destruct(this->memhandler);
    delete [] this->buckets;
  }

  void clear(void)
  {
    unsigned int i;
    for (i = 0; i < this->size; i++) {
      while (this->buckets[i]) {
        SbHashEntry<Type, Key> * entry = this->buckets[i];
        this->buckets[i] = entry->next;
        delete entry;
      }
    }
    memset(this->buckets, 0, this->size * sizeof(SbHashEntry<Type, Key> *));
    this->elements = 0;
  }

  SbBool put(const Key & key, const Type & obj)
  {
    unsigned int i = this->getIndex(key);
    SbHashEntry<Type, Key> * entry = this->buckets[i];
    while (entry) {
      if (entry->key == key) {
        /* Replace the old value */
        entry->obj = obj;
        return FALSE;
      }
      entry = entry->next;
    }

    /* Key not already in the hash table; insert a new
     * entry as the first element in the bucket
     */
    entry = new (this->memhandler) SbHashEntry<Type, Key>(key, obj);
    entry->next = this->buckets[i];
    this->buckets[i] = entry;

    if (this->elements++ >= this->threshold) {
      this->resize(static_cast<unsigned int>( coin_geq_prime_number(this->size + 1)));
    }
    return TRUE;
  }

  SbBool get(const Key & key, Type & obj) const
  {
    SbHashEntry<Type, Key> * entry;
    unsigned int i = this->getIndex(key);
    entry = this->buckets[i];
    while (entry) {
      if (entry->key == key) {
        obj = entry->obj;
        return TRUE;
      }
      entry = entry->next;
    }
    return FALSE;
  }

  SbBool remove(const Key & key)
  {
    unsigned int i = this->getIndex(key);
    SbHashEntry<Type, Key> * entry = this->buckets[i], * next, * prev = NULL;
    while (entry) {
      next = entry->next;
      if (entry->key == key) {
        this->elements--;
        if (prev == NULL) {
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

  template<typename PointerType>
  void apply( ApplyFunctor<PointerType> & func,
             PointerType closure = NULL) const
  {
#ifdef COIN_DEPOINTER_AVAILABLE
    COIN_CT_ASSERT(coin_depointer<PointerType>::valid);
#endif
    unsigned int i;
    SbHashEntry<Type, Key> * elem;
    for (i = 0; i < this->size; i++) {
      elem = this->buckets[i];
      while (elem) {
        func(elem->key, elem->obj, closure);
        elem = elem->next;
      }
    }
  }

  void makeKeyList(SbList<Key> & l) const
  {
    add_to_list functor;
    this->apply(functor, &l);
  }

  unsigned int getNumElements(void) const { return this->elements; }

protected:
  unsigned int getIndex(const Key & key) const {
    unsigned int idx = SbHashFunc(key);
    return (idx % this->size);
  }

  void resize(unsigned int newsize) {
    /* we don't shrink the table */
    if (this->size >= newsize) return;

    unsigned int oldsize = this->size;
    SbHashEntry<Type, Key> ** oldbuckets = this->buckets;

    this->size = newsize;
    this->elements = 0;
    this->threshold = static_cast<unsigned int> (newsize * this->loadfactor);
    this->buckets = new SbHashEntry<Type, Key> * [newsize];
    memset(this->buckets, 0, this->size * sizeof(SbHashEntry<Type, Key> *));

    /* Transfer all mappings */
    unsigned int i;
    for (i = 0; i < oldsize; i++) {
      SbHashEntry<Type, Key> * entry = oldbuckets[i];
      while (entry) {
        this->put(entry->key, entry->obj);
        SbHashEntry<Type, Key> * preventry = entry;
        entry = entry->next;
        delete preventry;
      }
    }
    delete [] oldbuckets;
  }

private:
  void commonConstructor(unsigned int sizearg, float loadfactorarg)
  {
    if (loadfactorarg <= 0.0f) { loadfactorarg = 0.75f; }
    unsigned int s = coin_geq_prime_number(sizearg);
    this->memhandler = cc_memalloc_construct(sizeof(SbHashEntry<Type, Key>));
    this->size = s;
    this->elements = 0;
    this->threshold = static_cast<unsigned int> (s * loadfactorarg);
    this->loadfactor = loadfactorarg;
    this->buckets = new SbHashEntry<Type, Key> * [this->size];
    memset(this->buckets, 0, this->size * sizeof(SbHashEntry<Type, Key> *));
  }

  void getStats(int & buckets_used, int & buckets, int & elements, float & chain_length_avg, int & chain_length_max)
  {
    unsigned int i;
    buckets_used = 0, chain_length_max = 0;
    for (i = 0; i < this->size; i++) {
      if (this->buckets[i]) {
        unsigned int chain_l = 0;
        SbHashEntry<Type, Key> * entry = this->buckets[i];
        buckets_used++;
        while (entry) {
          chain_l++;
          entry = entry->next;
        }
        if (chain_l > chain_length_max) { chain_length_max = chain_l; }
      }
    }
    buckets = this->size;
    elements = this->elements;
    chain_length_avg = static_cast<float>( this->elements / buckets_used);
  }

  struct copy_data : public ApplyFunctor<SbHash *> {
    void operator()(Key & key, Type & obj, SbHash * thisp) {
      thisp->put(key, obj);
    }
  };

  struct add_to_list : public ApplyFunctor<SbList<Key> *> {
    void operator()(Key & key, Type & obj, SbList<Key> * list) {
      list->append(key);
    }
  };

  float loadfactor;
  unsigned int size;
  unsigned int elements;
  unsigned int threshold;

  SbHashEntry<Type, Key> ** buckets;
  cc_memalloc * memhandler;
};

#endif // !COIN_SBHASH_H
