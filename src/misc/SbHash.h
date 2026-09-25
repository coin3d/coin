#ifndef COIN_SBHASH_H
#define COIN_SBHASH_H

/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

// *************************************************************************
// This class (SbHash<Key, Type>) is internal and must not be exposed
// in the Coin API.

/**
   This function object is an extension of the STL concept "binary function".
   It is a goal to be as similar as possible to our STL counterpart.
*/

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

// *************************************************************************

//Create an uint of an arbitrary length datatype
template <class T>
inline unsigned int toUint(T in) noexcept {
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
inline unsigned int SbHashFunc(unsigned long long key) noexcept { return toUint<unsigned long long>(key); }
#endif //COIN_INTERNAL
#endif

/* Windows 64-bit uses the LLP64 type model, where int and long
 * are 32-bit and a pointer is 64-bit. Most Unices use the LP64
 * where int is 32-bit and long and pointer are 64-bit. */
/* FIXME: the following solution is a kludge. 20081001 tamer. */
#if defined(_WIN64)
inline unsigned int SbHashFunc(unsigned long long key) noexcept { return toUint<unsigned long long>(key); }
#else
//The identity hash function
inline unsigned int SbHashFunc(unsigned int key) noexcept { return key; }

//Some implementation of other basetypes
inline unsigned int SbHashFunc(int key) noexcept { return static_cast<unsigned int>(key); }

inline unsigned int SbHashFunc(unsigned long key) noexcept { return toUint<unsigned long>(key); }
#endif

// Preserve the historical content hash for interned C strings without
// allocating an SbString temporary. Mutable character buffers are used as
// pointer-identity keys by SoField and keep that distinct behavior.
inline unsigned int SbHashFunc(const char * key) noexcept {
  if (key == NULL) return 0;
  const unsigned char * str = reinterpret_cast<const unsigned char *>(key);
  unsigned long hash = 0;
  while (*str) {
    hash = (*str++) + (hash << 6) + (hash << 16) - hash;
  }
  return static_cast<unsigned int>(hash);
}

inline unsigned int SbHashFunc(char * key) noexcept {
  return SbHashFunc(reinterpret_cast<size_t>(key));
}

//String has its own implementation
class SbString;
unsigned int SbHashFunc(const SbString & key) noexcept;

/*
  Some implementations of pointers, all functions are per writing only reinterpret_casts to size_t
*/
//FIXME: Don't hold these definitions here, but where they are used - BFG 20080729
class SoBase;
class SoOutput;
class SoSensor;
unsigned int SbHashFunc(const SoBase * key) noexcept;
unsigned int SbHashFunc(const SoOutput * key) noexcept;
unsigned int SbHashFunc(const SoSensor * key) noexcept;

template <class Key, class Type>
class SbHash {
 public:

  class SbHashEntry {
  public:

    void * operator new(size_t COIN_UNUSED_ARG(size), cc_memalloc * memhandler) {
      SbHashEntry * entry = static_cast<SbHashEntry *>(cc_memalloc_allocate(memhandler));
      entry->memhandler = memhandler;
      return static_cast<void *>(entry);
    }
    void operator delete(void * ptr, cc_memalloc * memhandler) {
      cc_memalloc_deallocate(memhandler, ptr);
    }
    void operator delete(void * ptr) {
      SbHashEntry * entry = static_cast<SbHashEntry *>( ptr);
      cc_memalloc_deallocate(entry->memhandler, ptr);
    }
  SbHashEntry(const Key & key, const Type & obj) : key(key), obj(obj) {}
  SbHashEntry(const Key & key, const Type & obj, cc_memalloc *memhandler) 
		: key(key), obj(obj), memhandler(memhandler) {}

    Key key;
    Type obj;
    SbHashEntry * next;
    cc_memalloc * memhandler;
  };

  class iterator {
  public:
    iterator(const iterator & iter) {
      this->master = iter.master;
      this->index  = iter.index;
      this->elem  = iter.elem;
    }
    SbHashEntry & operator*() {
      return *this->elem;
    }
    SbHashEntry * operator->() {
      return this->elem;
    }
    bool operator==(const iterator & rhs) const {
      return rhs.elem == this->elem;
    }
    bool operator!=(const iterator & rhs) const {
      return !((*this)==rhs);
    }
    iterator & operator++() {
      setNext();
      return *this;
    }
  private:
  iterator(const SbHash<Key, Type> * master_in) :
    master(master_in) {
      this->index=0;
      setNextUsedBucket();
    }
    iterator() {
      this->elem = NULL;
    }

    inline void setNextUsedBucket() {
      for (; this->index < this->master->size; ++this->index) {
        if (this->master->buckets[this->index]) {
          this->elem = this->master->buckets[this->index];
          return;
        }
      }
      this->elem = NULL;
    }

    inline void setNext(){
      if (this->elem->next) {
        this->elem = this->elem->next;
        return;
      }
      if (this->index<this->master->size)
        ++this->index;
      setNextUsedBucket();
    }

    const SbHash<Key, Type> * master;
    unsigned int index;
    SbHashEntry * elem;
    friend class SbHash<Key, Type>;
  };

  class const_iterator {
  public:
    const_iterator(const iterator & iter) {
      this->master = iter.master;
      this->index  = iter.index;
      this->elem  = iter.elem;
    }
    const_iterator(const const_iterator & iter) {
      this->master = iter.master;
      this->index  = iter.index;
      this->elem  = iter.elem;
    }
    const SbHashEntry & operator*() {
      return *this->elem;
    }
    const SbHashEntry * operator->() {
      return this->elem;
    }
    bool operator==(const const_iterator & rhs) const {
      return rhs.elem == this->elem;
    }
    bool operator!=(const const_iterator & rhs) const {
      return !((*this)==rhs);
    }
    const_iterator & operator++() {
      setNext();
      return *this;
    }
  private:
  const_iterator(const SbHash<Key, Type> * master_in) :
    master(master_in) {
      this->index=0;
      setNextUsedBucket();
    }
    const_iterator() {
      this->elem = NULL;
    }

    inline void setNextUsedBucket() {
      for (; this->index < this->master->size; ++this->index) {
        if (this->master->buckets[this->index]) {
          this->elem = this->master->buckets[this->index];
          return;
        }
      }
      this->elem = NULL;
    }

    inline void setNext(){
      if (this->elem->next) {
        this->elem = this->elem->next;
        return;
      }
      if (this->index<this->master->size)
        ++this->index;
      setNextUsedBucket();
    }

    const SbHash<Key, Type> * master;
    unsigned int index;
    const SbHashEntry * elem;
    friend class SbHash<Key, Type>;
  };

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
    if (this == &from) return *this;
    this->clear();
    unsigned int i;
    SbHashEntry * elem;
    for (i = 0; i < from.size; ++i) {
      elem = from.buckets[i];
      while (elem) {
        this->put(elem->key, elem->obj);
        elem = elem->next;
      }
    }
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
        SbHashEntry * entry = this->buckets[i];
        this->buckets[i] = entry->next;
        delete entry;
      }
    }
    memset(this->buckets, 0, this->size * sizeof(SbHashEntry *));
    this->elements = 0;
  }

  iterator begin() const {
    return iterator(this);
  }

  iterator end() const {
    return iterator();
  }

  const_iterator const_begin() const {
    return const_iterator (this);
  }

  const_iterator const_end() const {
    return const_iterator();
  }

  Type & operator[](const Key & key) {
    Type * obj = NULL;
    if (!getP(key,obj)) {
      Type dummy = Type();
      if (!put(key,dummy)) assert(false);
      if (!getP(key,obj)) assert(false);
    }
    return *obj;
  }
  
  size_t erase(const Key & key)
  {
    unsigned int i = this->getIndex(key);
    SbHashEntry * entry = this->buckets[i], * next, * prev = NULL;
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
        return 1;
      }
      prev = entry;
      entry = next;
    }
    return 0;
  }

  void makeKeyList(SbList<Key> & l) const
  {
    unsigned int i;
    SbHashEntry * elem;
    for (i = 0; i < this->size; ++i) {
      elem = this->buckets[i];
      while (elem) {
        l.append(elem->key);
        elem = elem->next;
      }
    }
  }

  unsigned int getNumElements(void) const { return this->elements; }

  const_iterator find(const Key & key) const
  {
    const_iterator iter(this);
    iter.index = this->getIndex(key);
    
    iter.elem = this->buckets[iter.index];
    while (iter.elem) {
      if (iter.elem->key == key) {
        return iter;
      }
      iter.elem = iter.elem->next;
    }
    return const_end();
  }


protected:
  unsigned int getIndex(const Key & key) const noexcept {
    return this->getIndex(key, this->size);
  }

  unsigned int getNumBuckets(void) const noexcept {
    return this->size;
  }

  unsigned int getResizeThreshold(void) const noexcept {
    return this->threshold;
  }

  void resize(unsigned int newsize) {
    /* we don't shrink the table */
    if (this->size >= newsize) return;

    SbHashEntry ** newbuckets = new SbHashEntry * [newsize];
    memset(newbuckets, 0, newsize * sizeof(SbHashEntry *));

    /* Relink all mappings without copying keys or values. Allocation happens
       before any existing node is touched, and hashing is required to be
       non-throwing, so the table remains unchanged if allocation fails. */
    unsigned int i;
    for (i = 0; i < this->size; i++) {
      SbHashEntry * entry = this->buckets[i];
      while (entry) {
        SbHashEntry * next = entry->next;
        const unsigned int newindex = this->getIndex(entry->key, newsize);
        entry->next = newbuckets[newindex];
        newbuckets[newindex] = entry;
        entry = next;
      }
    }

    delete [] this->buckets;
    this->buckets = newbuckets;
    this->size = newsize;
    this->threshold = static_cast<unsigned int> (newsize * this->loadfactor);
  }

  //FIXME: Make this private when SbHash goes public: BFG 20090430
public:
  SbBool put(const Key & key, const Type & obj)
  {
    unsigned int i = this->getIndex(key);
    SbHashEntry * entry = this->buckets[i];
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
    entry = new (this->memhandler) SbHashEntry(key, obj, this->memhandler);
    entry->next = this->buckets[i];
    this->buckets[i] = entry;

    if (this->elements++ >= this->threshold) {
      this->resize(static_cast<unsigned int>( coin_geq_prime_number(this->size + 1)));
    }
    return TRUE;
  }

  SbBool get(const Key & key, Type & obj) const
  {
    SbHashEntry * entry;
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

 private:
  static unsigned int getIndex(const Key & key, unsigned int bucketsize) noexcept
  {
    static_assert(noexcept(SbHashFunc(key)),
                  "SbHashFunc(key) and implicit key conversions must be noexcept");
    return SbHashFunc(key) % bucketsize;
  }

  SbBool getP(const Key & key, Type *& obj) const
  {
    SbHashEntry * entry;
    unsigned int i = this->getIndex(key);
    entry = this->buckets[i];
    while (entry) {
      if (entry->key == key) {
        obj = &entry->obj;
        return TRUE;
      }
      entry = entry->next;
    }
    return FALSE;
  }


  void commonConstructor(unsigned int sizearg, float loadfactorarg)
  {
    if (loadfactorarg <= 0.0f) { loadfactorarg = 0.75f; }
    unsigned int s = coin_geq_prime_number(sizearg);
    this->memhandler = cc_memalloc_construct(sizeof(SbHashEntry));
    this->size = s;
    this->elements = 0;
    this->threshold = static_cast<unsigned int> (s * loadfactorarg);
    this->loadfactor = loadfactorarg;
    this->buckets = new SbHashEntry * [this->size];
    memset(this->buckets, 0, this->size * sizeof(SbHashEntry *));
  }

 protected:
  void getStats(int & buckets_used, int & buckets, int & elements, float & chain_length_avg, int & chain_length_max)
  {
    unsigned int i;
    buckets_used = 0, chain_length_max = 0;
    for (i = 0; i < this->size; i++) {
      if (this->buckets[i]) {
        int chain_l = 0;
        SbHashEntry * entry = this->buckets[i];
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
    chain_length_avg = buckets_used == 0 ? 0.0f :
      static_cast<float>(this->elements) / static_cast<float>(buckets_used);
  }

 private:
  float loadfactor;
  unsigned int size;
  unsigned int elements;
  unsigned int threshold;

  SbHashEntry ** buckets;
  cc_memalloc * memhandler;
};

#endif // !COIN_SBHASH_H
