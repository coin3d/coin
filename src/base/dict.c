/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.  See the file LICENSE.GPL
 *  at the root directory of this source distribution for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  www.sim.no, support@sim.no, Voice: +47 22114160, Fax: +47 22207097
 *
\**************************************************************************/

#include <Inventor/SbBasic.h>

#include <Inventor/C/base/dict.h>
#include <Inventor/C/base/list.h>

#include <stdlib.h>
#include <assert.h>

/* ********************************************************************** */

typedef struct cc_dict_entry cc_dict_entry;

struct cc_dict {
  unsigned int type;
  unsigned long (*hash)(unsigned long key);
  int tablesize;
  cc_dict_entry ** buckets;
};

struct cc_dict_entry {
  unsigned long key;
  void * value;
  cc_dict_entry * next;
};

/* ********************************************************************** */

static cc_dict_entry *
cc_dictentry_construct(unsigned long key, void * value)
{
  cc_dict_entry * entry;
  entry = (cc_dict_entry *) malloc(sizeof(cc_dict_entry));
  assert(entry != NULL);
  entry->key = key;
  entry->value = value;
  entry->next = NULL;
  return entry;
}

static cc_dict_entry * cc_dict_find_entry(cc_dict *, unsigned long, unsigned long, cc_dict_entry **);

/* ********************************************************************** */

static unsigned long
default_hash(unsigned long key)
{
  return key;
}

/* ********************************************************************** */

cc_dict *
cc_dict_construct_sized(unsigned int entries)
{
  cc_dict * dict;
  int i;
  assert(entries != 0);
  dict = (cc_dict *) malloc(sizeof(cc_dict));
  dict->tablesize = entries;
  dict->buckets = malloc(sizeof(cc_dict_entry *) * entries);
  assert(dict->buckets != NULL);
  dict->hash = default_hash;
  for ( i = 0; i < entries; i++ )
    dict->buckets[i] = NULL;
  return dict;
}

cc_dict *
cc_dict_construct(void)
{
  return cc_dict_construct_sized(251);
}

void
cc_dict_destruct(cc_dict * dict)
{
  cc_dict_clear(dict);
  free(dict->buckets);
}


#if 0
/*!
  Make a deep copy of the contents of dictionary \a from into this dictionary.
*/
SbDict &
SbDict::operator=(const SbDict & from)
{
  dict->tablesize = from->tablesize;
  dict->hash = from->hash;
  dict->buckets = new SbDictEntry *[this->tablesize];
  for (int i = 0; i < this->tablesize; i++) this->buckets[i] = NULL;
  from.applyToAll(copyval, this);
  return *this;
}
#endif

#if 0
/*!
  \internal
  Callback for copying values from one SbDict to another.
*/
void
SbDict::copyval(unsigned long key, void * value, void * data)
{
  SbDict * thisp = (SbDict *)data;
  thisp->enter(key, value);
}
#endif

void
cc_dict_clear(cc_dict * dict)
{
  int i;
  cc_dict_entry * entry, * next;
  for ( i = 0; i < dict->tablesize; i++ ) {
    for ( entry = dict->buckets[i]; entry != NULL; entry = next ) {
      next = entry->next;
      free(entry);
    }
    dict->buckets[i] = NULL;
  }
} /* cc_dict_clear() */

SbBool
cc_dict_enter(cc_dict * dict, unsigned long key, void * value)
{
  unsigned long bucket;
  cc_dict_entry * entry;
  bucket = dict->hash(key) % dict->tablesize;
  entry = cc_dict_find_entry(dict, key, bucket, NULL);
  if ( entry == NULL ) {
    entry = cc_dictentry_construct(key, value);
    entry->next = dict->buckets[bucket];
    dict->buckets[bucket] = entry;
    return TRUE;
  } else {
    entry->value = value;
    return FALSE;
  }
} /* cc_dict_enter() */

SbBool
cc_dict_find(cc_dict * dict, unsigned long key, void ** value)
{
  unsigned long bucket;
  cc_dict_entry * entry;
  bucket = dict->hash(key) % dict->tablesize;
  entry = cc_dict_find_entry(dict, key, bucket, NULL);
  if ( entry == NULL ) {
    value = NULL;
    return FALSE;
  } else {
    value = entry->value;
    return TRUE;
  }
} /* cc_dict_find() */

SbBool
cc_dict_remove(cc_dict * dict, unsigned long key)
{
  unsigned long bucket;
  cc_dict_entry * prev = NULL;
  cc_dict_entry * entry;
  bucket = dict->hash(key) % dict->tablesize;
  entry = cc_dict_find_entry(dict, key, bucket, &prev);
  if ( entry != NULL ) {
    if ( prev != NULL )
      prev->next = entry->next;
    else
      dict->buckets[bucket] = entry->next;
    free(entry);
    return TRUE;
  }
  return FALSE;
} /* cc_dict_remove() */

void
cc_dict_apply(cc_dict * dict, void (*func)(unsigned long key, void * value))
{
  int i;
  cc_dict_entry * entry;
  for ( i = 0; i < dict->tablesize; i++ ) {
    entry = dict->buckets[i];
    while ( entry ) {
      func(entry->key, entry->value);
      entry = entry->next;
    }
  }
} /* cc_dict_apply() */

void
cc_dict_apply_with_closure(cc_dict * dict, void (*func)(unsigned long key, void * value, void * closure), void * closure)
{
  int i;
  cc_dict_entry * entry;
  for ( i = 0; i < dict->tablesize; i++ ) {
    entry = dict->buckets[i];
    while ( entry ) {
      func(entry->key, entry->value, closure);
      entry = entry->next;
    }
  }
} /* cc_dict_apply_with_closure() */

#if 0
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
#endif

static cc_dict_entry *
cc_dict_find_entry(cc_dict * dict, unsigned long key, unsigned long bucket, cc_dict_entry ** prev )
{
  cc_dict_entry * entry;
  if ( prev ) *prev = NULL;
  entry = dict->buckets[bucket];
  while ( entry ) {
    if ( entry->key == key ) break;
    if ( prev ) *prev = entry;
    entry = entry->next;
  }
  return entry;
} /* cc_dict_find_entry() */

void
cc_dict_set_hashing(cc_dict * dict, unsigned long (*hash)(unsigned long key))
{
  assert(dict != NULL);
  dict->hash = hash;
}


#if 0
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
#endif

#if 0
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
#endif
