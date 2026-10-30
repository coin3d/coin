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

/*!
  \class SbDict SbDict.h Inventor/SbDict.h
  \brief The SbDict class organizes a dictionary of keys and values.

  \ingroup coin_base

  It uses hashing to quickly insert and find entries in the dictionary.
  An entry consists of an unique key and a generic pointer.
*/

/*!
  \typedef uintptr_t SbDictKeyType

  The type definition for a dictionary key.
*/

/*!
  \typedef void SbDictApplyFunc(SbDictKeyType key, void * value)

  The type definition of the function to be applied to each entry.
*/

/*!
  \typedef void SbDictApplyDataFunc(SbDictKeyType key, void * value, void * data)

  The type definition of the function with associated data that is to be
  applied to each entry.
*/

/*!
  \typedef SbDictKeyType SbDictHashingFunc(const SbDictKeyType key)

  The type definition of a dictionary hashing function.
*/

// *************************************************************************

#define COIN_ALLOW_SBDICT
#include <Inventor/SbDict.h>
#undef COIN_ALLOW_SBDICT

#include <cassert>

#include "base/dict.h"
#include "base/oomp.h"
#include <Inventor/lists/SbPList.h>

// Keep the historical private handle type in the installed header. The
// stored pointer is opaque and is only dereferenced as its actual backend.
static cc_dict *
sbdict_backend(cc_hash * handle)
{
  return reinterpret_cast<cc_dict *>(handle);
}

// *************************************************************************

/*!
  Constructor with \a entries specifying the initial number of buckets
  in the hash list -- so it need to be larger than 0. Other than this,
  no special care needs to be taken in choosing the value since it is
  rounded up to a suitable prime number.
*/
SbDict::SbDict(const int entries)
{
  assert(entries > 0);
  this->hashtable = reinterpret_cast<cc_hash *>(cc_dict_construct(entries, 0.75f));
  if (this->hashtable == NULL) coin_oom_abort("SbDict constructor");
}

/*!
  Makes a shallow copy of the entries, using the default hash function.
  The custom hash function of the source is not copied.
*/
SbDict::SbDict(const SbDict & from)
{
  this->hashtable = NULL;
  this->operator=(from);
}

/*!
  Destructor.
*/
SbDict::~SbDict()
{
  cc_dict_destruct(sbdict_backend(this->hashtable));
}

extern "C" {

/*
  Callback for copying values from one SbDict to another.
*/
static
void
copyval(SbDictKeyType key, void * value, void * data)
{
  SbDict * thisp = static_cast<SbDict *>(data);
  thisp->enter(key, value);
}

} // extern "C"

/*!
  Make a shallow copy of the contents of dictionary \a from into this
  dictionary. The values remain non-owning pointers. As with the copy
  constructor, the default hash function is used. Self-assignment preserves
  the entries and the current hash function without rebuilding the table.
*/
SbDict &
SbDict::operator=(const SbDict & from)
{
  if (this == &from) return *this;
  if (this->hashtable) {
    // clear old values
    this->clear();
    cc_dict_destruct(sbdict_backend(this->hashtable));
  }
  this->hashtable = reinterpret_cast<cc_hash *>(cc_dict_construct(
    cc_dict_get_num_elements(sbdict_backend(from.hashtable)), 0.75f));
  if (this->hashtable == NULL) coin_oom_abort("SbDict assignment");
  from.applyToAll(copyval, this);
  return *this;
}

/*!
  Clear all entries in the dictionary.
*/
void
SbDict::clear(void)
{
  cc_dict_clear(sbdict_backend(this->hashtable));
}

/*!
  Inserts a new entry into the dictionary. \a key should be
  a unique number, and \a value is the generic user data.

  If \a key does not exist in the dictionary, a new entry
  is created and \c TRUE is returned. Otherwise, the generic user
  data is changed to \a value, and \c FALSE is returned. Failure to allocate
  a mandatory entry terminates with a diagnostic. Optional growth failure
  preserves the newly inserted entry. If a custom hash throws during growth,
  the exception propagates with that entry still present in the valid table.
*/
SbBool
SbDict::enter(const Key key, void * const value)
{
  const cc_dict_put_result result =
    cc_dict_try_put(sbdict_backend(this->hashtable), key, value);
  if (result == CC_DICT_PUT_FAILED) coin_oom_abort("SbDict::enter");
  return result == CC_DICT_PUT_INSERTED;
}

/*!
  Searches for \a key in the dictionary. If an entry with this
  key exists, \c TRUE is returned and the entry value is returned
  in \a value. Otherwise, \c FALSE is returned and \a value is unchanged.
*/
SbBool
SbDict::find(const Key key, void *& value) const
{
  return cc_dict_get(sbdict_backend(this->hashtable), key, &value);
}

/*!
  Removes the entry with key \a key. \c TRUE is returned if an entry
  with this key was present, \c FALSE otherwise.
*/
SbBool
SbDict::remove(const Key key)
{
  return cc_dict_remove(sbdict_backend(this->hashtable), key);
}


// Bridge the no-data overload through a stack-local closure. No function
// pointer is converted to an object pointer; nested traversals are independent.
struct SbDictApplyClosure {
  SbDictApplyFunc * callback;
};
extern "C" {
static void
sbdict_dummy_apply(SbDict::Key key, void * value, void * closure)
{
  SbDictApplyClosure * data = static_cast<SbDictApplyClosure *>(closure);
  data->callback(key, value);
}
}
/*!
  Applies \a rtn to all entries in the dictionary, without a guaranteed
  order. The callback may read entries and remove its current entry; it must
  not otherwise mutate or destroy this dictionary. Exceptions propagate.
*/
void
SbDict::applyToAll(SbDictApplyFunc * rtn) const
{
  SbDictApplyClosure closure = { rtn };
  cc_dict_apply(sbdict_backend(this->hashtable), sbdict_dummy_apply, &closure);
}

/*!
  \overload
*/
void
SbDict::applyToAll(SbDictApplyDataFunc * rtn, void * data) const
{
  cc_dict_apply(sbdict_backend(this->hashtable), static_cast<cc_dict_apply_func *>(rtn), data);
}

typedef struct {
  SbPList * keys;
  SbPList * values;
} sbdict_makeplist_data;

extern "C" {

static void
sbdict_makeplist_cb(SbDict::Key key, void * value, void * closure)
{
  sbdict_makeplist_data * data = static_cast<sbdict_makeplist_data *>(closure);
  data->keys->append(reinterpret_cast<void *>(key));
  data->values->append(value);
}

} // extern "C"

/*!
  Appends all entries to \a keys and \a values, preserving their existing
  contents. Appended keys and values correspond by position, without a
  guaranteed order. Keys are represented as pointer-sized values. If an
  append throws, both lists regain their original lengths and contents;
  their allocated capacities may have grown.
*/
void
SbDict::makePList(SbPList & keys, SbPList & values)
{
  sbdict_makeplist_data applydata;
  applydata.keys = &keys;
  applydata.values = &values;

  const int keylength = keys.getLength();
  const int valuelength = values.getLength();
  try {
    cc_dict_apply(sbdict_backend(this->hashtable), sbdict_makeplist_cb, &applydata);
  }
  catch (...) {
    keys.truncate(keylength);
    values.truncate(valuelength);
    throw;
  }
}

/*!
  Sets a new hashing function for this dictionary. Default
  hashing function just returns the key. Passing NULL restores it.

  Existing entries remain accessible after changing the hash function.
  If allocating replacement storage fails, the previous function and
  entries remain unchanged. A hash exception also preserves the previous
  function and chains and propagates. Hash callbacks must be stable and
  must not mutate this dictionary.

  If you find that items entered into the dictionary seems to make
  clusters in only a few buckets, you should try setting a hashing
  function. If you're for instance using strings, you could use the
  static SbString::hash() function (you'd need to make a static function
  that will cast from SbDict::Key to char * of course).

  This function is not part of the OIV API.
*/
void
SbDict::setHashingFunction(SbDictHashingFunc * func)
{
  cc_dict_set_hash_func(sbdict_backend(this->hashtable), static_cast<cc_dict_hash_func *>(func));
}
