/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#include <Inventor/SbDict.h>
#include <Inventor/lists/SbPList.h>
#include <Inventor/C/base/memalloc.h>
#include <assert.h>

/*!
  \class SbDict SbDict.h Inventor/SbDict.h
  \brief The SbDict class organizes a dictionary of keys and values.
  \ingroup base

  It uses hashing to quickly insert and find entries in the dictionary.
  An entry consists of an unique key and a generic pointer.
*/

//
// internal class
//
class SbDictEntry
{
private:
  SbDictEntry(const unsigned long key, void * const value) {
    this->key = key;
    this->value = value;
  }
private:
  void * operator new(size_t, cc_memalloc * allocator) {
    return cc_memalloc_allocate(allocator);
  }

  unsigned long key;
  void * value;
  SbDictEntry * next;

  friend class SbDict;
};

//
// default hashing function will just return the key
//
static unsigned long
default_hashfunc(const unsigned long key)
{
  return key;
}

// Macro to return the memory allocator for an instance. See FIXME in
// constructor and copy operator.
#define GET_MEMALLOC(instance) \
  (cc_memalloc*) ((instance)->buckets[instance->tablesize])

// *************************************************************************

/*!
  Constructor with \a entries specifying the number of buckets
  in the hash list -- so it need to be larger than 0. For best
  performance during dictionary look-ups, \a entries should be a prime.
*/
SbDict::SbDict(const int entries)
{
  assert(entries > 0);
  this->tablesize = entries;
  // allocate one extra item to store the memory allocator 
  // 
  // FIXME: when it's safe to change the ABI, create a private member
  // to store the cc_memalloc pointer instead of using the buckets
  // array. pederb, 2002-01-30
  this->buckets = new SbDictEntry *[this->tablesize+1];
  this->buckets[this->tablesize] = (SbDictEntry*)
    cc_memalloc_construct(sizeof(SbDictEntry));
  this->hashfunc = default_hashfunc;
  for (int i = 0; i < this->tablesize; i++) this->buckets[i] = NULL;
}

/*!
  Copy constructor.
*/
SbDict::SbDict(const SbDict & from)
{
  this->operator=(from);
}

/*!
  Destructor.
*/
SbDict::~SbDict()
{
  this->clear();
  cc_memalloc_destruct(GET_MEMALLOC(this));
  delete[] buckets;
}

/*!
  Make a deep copy of the contents of dictionary \a from into this dictionary.
*/
SbDict &
SbDict::operator=(const SbDict & from)
{
  this->tablesize = from.tablesize;
  this->hashfunc = from.hashfunc;
  // allocate one extra to store the memory allocator
  // 
  // FIXME: when it's safe to change the ABI, create a private member
  // to store the cc_memalloc pointer instead of using the buckets
  // array. pederb, 2002-01-30
  this->buckets = new SbDictEntry *[this->tablesize+1];
  this->buckets[this->tablesize] = (SbDictEntry*)
    cc_memalloc_construct(sizeof(SbDictEntry));
  for (int i = 0; i < this->tablesize; i++) this->buckets[i] = NULL;
  from.applyToAll(copyval, this);
  return *this;
}

/*!
  \COININTERNAL
  Callback for copying values from one SbDict to another.
*/
void
SbDict::copyval(unsigned long key, void * value, void * data)
{
  SbDict * thisp = (SbDict *)data;
  thisp->enter(key, value);
}

/*!
  Clear all entries in the dictionary.
*/
void
SbDict::clear(void)
{
  int i;
  SbDictEntry * entry, * nextEntry;
  
  cc_memalloc * allocator = GET_MEMALLOC(this);

  for (i = 0; i < this->tablesize; i++) {
    for (entry = buckets[i]; entry != NULL; entry = nextEntry) {
      nextEntry = entry->next;
      cc_memalloc_deallocate(allocator, (void*) entry);
    }
    buckets[i] = NULL;
  }
  // FIXME: consider calling cc_memalloc_clear(). But, it will be
  // faster not to do this, of course. pederb, 2002-01-30
}

/*!
  Inserts a new entry into the dictionary. \a key should be
  a unique number, and \a value is the generic user data.

  \e If \a key does not exist in the dictionary, a new entry
  is created and \c TRUE is returned. Otherwise, the generic user
  data is changed to \a value, and \c FALSE is returned.
*/
SbBool
SbDict::enter(const unsigned long key, void * const value)
{
  const unsigned long bucketnum = this->hashfunc(key) % this->tablesize;
  SbDictEntry *entry = findEntry(key, bucketnum);

  if (entry == NULL) {
    entry = new (GET_MEMALLOC(this)) SbDictEntry(key, value);
    entry->next = this->buckets[bucketnum];
    this->buckets[bucketnum] = entry;
    return TRUE;
  }
  else {
    entry->value = value;
    return FALSE;
  }
}

/*!
  Searches for \a key in the dictionary. If an entry with this
  key exists, \c TRUE is returned and the entry value is returned
  in \a value. Otherwise, \c FALSE is returned.
*/
SbBool
SbDict::find(const unsigned long key, void *& value) const
{
  const unsigned long bucketnum = this->hashfunc(key) % this->tablesize;
  SbDictEntry *entry = findEntry(key, bucketnum);
  if (entry == NULL) {
    value = NULL;
    return FALSE;
  }
  else {
    value = entry->value;
    return TRUE;
  }
}

/*!
  Removes the entry with key \a key. \c TRUE is returned if an entry
  with this key was present, \c FALSE otherwise.
*/
SbBool
SbDict::remove(const unsigned long key)
{
  const unsigned long bucketnum = this->hashfunc(key) % this->tablesize;
  SbDictEntry *prev = NULL;
  SbDictEntry *entry = findEntry(key, bucketnum, &prev);
  if (entry == NULL)
    return FALSE;
  else {
    if (prev) {
      prev->next = entry->next;
    }
    else {
      this->buckets[bucketnum] = entry->next;
    }
    cc_memalloc_deallocate(GET_MEMALLOC(this), (void*) entry);
    return TRUE;
  }
}

/*!
  Applies \a rtn to all entries in the dictionary.
*/
void
SbDict::applyToAll(void (* rtn)(unsigned long key, void * value)) const
{
  SbDictEntry * entry;
  int n = this->tablesize;
  for (int i = 0; i < n; i++) {
    entry = this->buckets[i];
    while (entry) {
      rtn(entry->key, entry->value);
      entry = entry->next;
    }
  }
}

/*!
  \overload
*/
void
SbDict::applyToAll(void (* rtn)(unsigned long key, void * value, void * data),
                   void * data) const
{
  SbDictEntry * entry;
  int n = this->tablesize;
  for (int i = 0; i < n; i++) {
    entry = this->buckets[i];
    while (entry) {
      rtn(entry->key, entry->value, data);
      entry = entry->next;
    }
  }
}

/*!
  Creates lists with all entries in the dictionary.
*/
void
SbDict::makePList(SbPList & keys, SbPList & values)
{
  SbDictEntry * entry;
  int n = this->tablesize;
  for (int i = 0; i < n; i++) {
    entry = this->buckets[i];
    while (entry) {
      keys.append((void *)entry->key);
      values.append((void *)entry->value);
      entry = entry->next;
    }
  }
}

SbDictEntry *
SbDict::findEntry(const unsigned long key,
                  const unsigned long bucketnum,
                  SbDictEntry **prev) const
{
  if (prev) *prev = NULL;
  SbDictEntry *entry = buckets[bucketnum];
  while (entry) {
    if (entry->key == key) break;
    if (prev) *prev = entry;
    entry = entry->next;
  }
  return entry;
}

/*!
  Sets a new hashing function for this dictionary. Default
  hashing function just returns the key.

  If you find that items entered into the dictionary seems to make
  clusters in only a few buckets, you should try setting a hashing
  function. If you're for instance using strings, you could use the
  static SbString::hash() function (you'd need to make a static function
  that will cast from unsigned long to char * of course).

  This function is not part of the OIV API.
*/
void
SbDict::setHashingFunction(unsigned long (*func)(const unsigned long key))
{
  this->hashfunc = func;
}



/*
  For debugging
  */
// void
// SbDict::dump(void)
// {
//   int i;
//   SbDictEntry * entry, * nextEntry;

//   for (i = 0; i < this->tablesize; i++) {
//     for (entry = buckets[i]; entry != NULL; entry = nextEntry) {
//       nextEntry = entry->next;
//       printf("entry: '%s' %p\n", entry->key, entry->value);
//     }
//   }
// }

// void SbDict::print_info()
// {
//   int i, cnt;
//   SbDictEntry * entry;

//   printf("---------- dict info ------------------\n");

//   for (i = 0; i < this->tablesize; i++) {
//     entry = buckets[i];
//     cnt = 0;
//     while (entry) {
//       entry = entry->next;
//       cnt++;
//     }
//     printf(" bucket: %d, cnt: %d\n", i, cnt);
//   }
//   printf("\n\n\n");
// }
