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

// *************************************************************************

/*!
  Constructor with \a entries specifying the number of buckets
  in the hash list -- so it need to be larger than 0. For best
  performance during dictionary look-ups, \a entries should be a prime.
*/
SbDict::SbDict(const int entries)
{
  assert(entries > 0);
  this->hashtable = cc_hash_construct(entries, 0.75f);
}

/*!
  Copy constructor.
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
  cc_hash_destruct(this->hashtable);
}

/*!
  Make a deep copy of the contents of dictionary \a from into this dictionary.
*/
SbDict &
SbDict::operator=(const SbDict & from)
{
  if (this->hashtable) {
    // clear old values
    this->clear();
    cc_hash_destruct(this->hashtable);
  }
  this->hashtable = cc_hash_construct(cc_hash_get_num_elements(from.hashtable), 0.75f);
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
  cc_hash_clear(this->hashtable);
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
  return cc_hash_put(this->hashtable, key, value);
}

/*!
  Searches for \a key in the dictionary. If an entry with this
  key exists, \c TRUE is returned and the entry value is returned
  in \a value. Otherwise, \c FALSE is returned.
*/
SbBool
SbDict::find(const unsigned long key, void *& value) const
{
  return cc_hash_get(this->hashtable, key, &value);
}

/*!
  Removes the entry with key \a key. \c TRUE is returned if an entry
  with this key was present, \c FALSE otherwise.
*/
SbBool
SbDict::remove(const unsigned long key)
{
  return cc_hash_remove(this->hashtable, key);
}


// needed to support the extra applyToAll function. The actual
// function pointer is supplied as the closure pointer, and we just
// call that function from our dummy callback. This is needed since
// cc_hash only supports one apply function type.
typedef void sbdict_dummy_apply_func(unsigned long, void *);

static void
sbdict_dummy_apply(unsigned long key, void * value, void * closure)
{
  sbdict_dummy_apply_func * func = (sbdict_dummy_apply_func*) closure;
  func(key, value);
}

/*!
  Applies \a rtn to all entries in the dictionary.
*/
void
SbDict::applyToAll(void (* rtn)(unsigned long key, void * value)) const
{
  cc_hash_apply(this->hashtable, sbdict_dummy_apply, (void*) rtn);
}

/*!
  \overload
*/
void
SbDict::applyToAll(void (* rtn)(unsigned long, void *, void *),
                   void * data) const
{
  cc_hash_apply(this->hashtable, rtn, data);
}

typedef struct {
  SbPList * keys;
  SbPList * values;
} sbdict_makeplist_data;

static void
sbdict_makeplist_cb(unsigned long key, void * value, void * closure)
{
  sbdict_makeplist_data * data = (sbdict_makeplist_data*) closure;
  data->keys->append((void*)key);
  data->values->append(value);
}

/*!
  Creates lists with all entries in the dictionary.
*/
void
SbDict::makePList(SbPList & keys, SbPList & values)
{
  sbdict_makeplist_data applydata;
  applydata.keys = &keys;
  applydata.values = &values;

  cc_hash_apply(this->hashtable, sbdict_makeplist_cb, &applydata);
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
  cc_hash_set_hash_func(this->hashtable, func);
}

