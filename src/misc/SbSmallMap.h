#ifndef COIN_SBSMALLMAP_H
#define COIN_SBSMALLMAP_H

/**************************************************************************\
 * Copyright (c) 2026 FreeCAD contributors
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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* !COIN_INTERNAL */

#include <stddef.h>
#include <type_traits>
#include <utility>

#ifdef COIN_SMALLMAP_TESTING
#include <new>
#endif

#include <Inventor/SbBasic.h>
#include <Inventor/lists/SbList.h>

#include "coindefs.h"

/* A compact linear map for trivial keys and values in the common case of at
   most four entries. SbList keeps its first four elements inside the list
   object, so this map performs no allocation until a fifth distinct key is
   inserted. Allocation is the only operation that may throw; insertion keeps
   the map unchanged when allocation fails. Insertion past the inline limit
   and erase() may invalidate iterators and references. erase() does not
   preserve iteration order, and clear() retains spilled storage for reuse. */
template <typename Key, typename Type>
class SbSmallMap {
public:
  static_assert(std::is_trivial<Key>::value,
                "SbSmallMap keys must be trivial types");
  static_assert(std::is_trivial<Type>::value,
                "SbSmallMap values must be trivial types");
  static_assert(noexcept(std::declval<const Key &>() ==
                         std::declval<const Key &>()),
                "SbSmallMap key equality must be noexcept");

  class Entry {
  public:
    Entry(void) : key(), obj() { }
    Entry(const Key & keyarg, const Type & objarg)
      : key(keyarg), obj(objarg) { }

    Key key;
    Type obj;

#ifdef COIN_SMALLMAP_TESTING
    static void failNextAllocation(void) {
      Entry::allocationFailureFlag() = true;
    }

    static void * operator new[](size_t size) {
      if (Entry::allocationFailureFlag()) {
        Entry::allocationFailureFlag() = false;
        throw std::bad_alloc();
      }
      return ::operator new[](size);
    }

    static void operator delete[](void * ptr) noexcept {
      ::operator delete[](ptr);
    }

  private:
    static bool & allocationFailureFlag(void) {
      static bool fail = false;
      return fail;
    }
#endif
  };

  class const_iterator {
  public:
    const_iterator(const const_iterator & other)
      : master(other.master), index(other.index) { }

    const Entry & operator*(void) const {
      return this->master->entries.getArrayPtr()[this->index];
    }
    const Entry * operator->(void) const {
      return &this->master->entries.getArrayPtr()[this->index];
    }
    bool operator==(const const_iterator & other) const {
      return this->master == other.master && this->index == other.index;
    }
    bool operator!=(const const_iterator & other) const {
      return !(*this == other);
    }
    const_iterator & operator++(void) {
      ++this->index;
      return *this;
    }

  private:
    const_iterator(const SbSmallMap * masterarg, int indexarg)
      : master(masterarg), index(indexarg) { }

    const SbSmallMap * master;
    int index;
    friend class SbSmallMap<Key, Type>;
  };

  SbSmallMap(void) { }

#ifdef COIN_SMALLMAP_TESTING
  static void failNextAllocationForTesting(void) {
    Entry::failNextAllocation();
  }
#endif

  void clear(void) {
    this->entries.truncate(0);
  }

  SbBool put(const Key & key, const Type & obj) {
    const int index = this->findIndex(key);
    if (index >= 0) {
      this->entries[index].obj = obj;
      return FALSE;
    }
    this->entries.append(Entry(key, obj));
    return TRUE;
  }

  SbBool get(const Key & key, Type & obj) const {
    const int index = this->findIndex(key);
    if (index < 0) return FALSE;
    obj = this->entries.getArrayPtr()[index].obj;
    return TRUE;
  }

  Type & operator[](const Key & key) {
    int index = this->findIndex(key);
    if (index < 0) {
      this->entries.append(Entry(key, Type()));
      index = this->entries.getLength() - 1;
    }
    return this->entries[index].obj;
  }

  size_t erase(const Key & key) {
    const int index = this->findIndex(key);
    if (index < 0) return 0;
    this->entries.removeFast(index);
    return 1;
  }

  void makeKeyList(SbList<Key> & keys) const {
    for (int i = 0; i < this->entries.getLength(); ++i) {
      keys.append(this->entries.getArrayPtr()[i].key);
    }
  }

  unsigned int getNumElements(void) const {
    return static_cast<unsigned int>(this->entries.getLength());
  }

  const_iterator find(const Key & key) const {
    const int index = this->findIndex(key);
    return index < 0 ? this->const_end() : const_iterator(this, index);
  }

  const_iterator const_begin(void) const {
    return const_iterator(this, 0);
  }
  const_iterator const_end(void) const {
    return const_iterator(this, this->entries.getLength());
  }

private:
  int findIndex(const Key & key) const {
    for (int i = 0; i < this->entries.getLength(); ++i) {
      if (this->entries.getArrayPtr()[i].key == key) return i;
    }
    return -1;
  }

  SbList<Entry> entries;
};

#endif // !COIN_SBSMALLMAP_H
