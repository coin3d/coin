/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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

#include <Inventor/C/base/hash.h>
#include <Inventor/C/base/hashp.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

/* ********************************************************************** */
/* private functions */

static unsigned long
hash_default_hashfunc(unsigned long key)
{
  return key;
}

static unsigned int
hash_get_index(cc_hash * ht, unsigned long key)
{
  key = ht->hashfunc(key);
  key -= (key << 7); /* i.e. key = key * -127; */
  return key & (ht->size-1);
}

static void
hash_resize(cc_hash * ht, unsigned int newsize)
{
  cc_hash_entry ** oldbuckets = ht->buckets;
  unsigned int oldsize = ht->size, i;
  
  assert((newsize & -newsize) == newsize); /* must be power of 2 */
  
  /* Never shrink the table */
  if (ht->size > newsize)
    return;
  
  ht->size = newsize;
  ht->elements = 0;
  ht->threshold = (unsigned int) (newsize * ht->loadfactor);
  ht->buckets = (cc_hash_entry **) calloc(newsize, sizeof(cc_hash_entry));
  
  /* Transfer all mappings */
  for (i = 0; i < oldsize; i++) {
    cc_hash_entry * he = oldbuckets[i];
    while (he) {
      cc_hash_put(ht, he->key, he->val);
      he = he->next;
    }
  }
  free(oldbuckets);
}

/* ********************************************************************** */
/* public api */

/*!

  Construct a hash table. \a size is the initial bucket size, \a
  loadfactor is the percentage the table should be filled before
  resizing, and should be a number from 0 to 1. It is of course
  possible to specify a number bigger than 1, but then there will be
  greater chance of having many elements on the same bucket (linear
  search for an element). If you supply a number <= 0 for loadfactor,
  the default value 0.75 will be used.

*/
cc_hash *
cc_hash_construct(unsigned int size, float loadfactor)
{
  cc_hash * ht = (cc_hash *) malloc(sizeof(cc_hash));
  
  /* Size must be a power of two */
  unsigned int s = 1;
  while (s < size)
    s <<= 1;

  if (loadfactor <= 0.0f) loadfactor = 0.75f;

  ht->size = s;
  ht->elements = 0;
  ht->threshold = (unsigned int) (s * loadfactor);
  ht->loadfactor = loadfactor;
  ht->buckets = (cc_hash_entry **) calloc(s, sizeof(cc_hash_entry));
  ht->hashfunc = hash_default_hashfunc;
  return ht;
}

/*!
  Destruct the hash table \a ht.
*/
void
cc_hash_destruct(cc_hash * ht)
{
  cc_hash_clear(ht);
  free(ht->buckets);
  free(ht);
}

/*!
  Clear/remove all elements in the hash table \a ht.
*/
void
cc_hash_clear(cc_hash * ht)
{
  unsigned int i;
  for (i = 0; i < ht->size; i++) {
    cc_hash_entry * he = ht->buckets[i];
    while (he) {
      cc_hash_entry * next = he->next;
      free(he);
      he = next;
    }
  }
  memset(ht->buckets, 0, ht->size * sizeof(cc_hash_entry));
  ht->elements = 0;
}

/*!

  Insert a new element in the hash table \a ht. \a key is the key used
  to identify the element, while \a val is the element value. If \a
  key is already used by another element, the element value will be
  overwritten, and \e TRUE is returned. Otherwise a new element is
  created and \e FALSE is returned.

 */
SbBool
cc_hash_put(cc_hash * ht, unsigned long key, void * val)
{
  unsigned int i = hash_get_index(ht, key);
  cc_hash_entry * he = ht->buckets[i];

  while (he) {
    if (he->key == key) {
      /* Replace the old value */
      he->val = val;
      return TRUE;
    }
    he = he->next;
  }
  
  /* Key not already in the hash table; insert a new
   * entry as the first element in the bucket
   */
  he = (cc_hash_entry *) malloc(sizeof(cc_hash_entry));
  he->key = key;
  he->val = val;
  he->next = ht->buckets[i];
  ht->buckets[i] = he;
  
  if (ht->elements++ >= ht->threshold) {
    hash_resize(ht, ht->size * 2);
  }
  return FALSE;
}

/*!

  Find the element with key value \a key. If found, the value is written to
  \a val, and TRUE is returned. Otherwise FALSE is returned and \a val
  is not changed.

*/
SbBool
cc_hash_get(cc_hash * ht, unsigned long key, void ** val)
{
  cc_hash_entry * he;
  unsigned int i = hash_get_index(ht, key);
  he = ht->buckets[i];
  while (he) {
    if (he->key == key) {
      *val = he->val;
      return TRUE;
    }
    he = he->next;
  }
  return FALSE;
}

/*!
  Attempt to remove the element with key value \a key. Returns
  TRUE if found, FALSE otherwise.
*/
int
cc_hash_remove(cc_hash * ht, unsigned long key)
{
  cc_hash_entry * he, *next, * prev;
  unsigned int i = hash_get_index(ht, key);

  he = ht->buckets[i];
  prev = NULL;
  while (he) {
    next = he->next;
    if (he->key == key) {
      ht->elements--;
      if (prev == NULL) {
        ht->buckets[i] = next;
      }
      else {
        prev->next = next;
      }
      free(he);
      return TRUE;
    }
    prev = he;
    he = next;
  }
  return FALSE;
}

/*!
  Return the number of elements in the hash table.
*/
unsigned int
cc_hash_get_num_elements(cc_hash * ht)
{
  return ht->elements;
}

/*!
  Set the hash func that is used to map key values into 
  a bucket index.
*/
void
cc_hash_set_hash_func(cc_hash * ht, cc_hash_func * func)
{
  ht->hashfunc = func;
}

/*!
  For debugging only. Prints some information on stdout.
*/
void
cc_hash_print_stat(cc_hash * ht)
{
  unsigned int i, used_buckets = 0, max_chain_l = 0;
  for (i = 0; i < ht->size; i++) {
    if (ht->buckets[i]) {
      unsigned int chain_l = 0;
      cc_hash_entry * he = ht->buckets[i];
      used_buckets++;
      while (he) {
        chain_l++;
        he = he->next;
      }
      if (chain_l > max_chain_l) max_chain_l = chain_l;
    }
  }
  printf("Used buckets %u of %u (%u elements), avg chain length: %.2f, max chain length: %u\n", 
         used_buckets, ht->size, ht->elements, (float)ht->elements / used_buckets, max_chain_l);
}
