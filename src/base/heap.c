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

#include <Inventor/C/base/heap.h>
#include <Inventor/C/base/heapp.h>
#include <Inventor/C/base/hash.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/* ********************************************************************** */
/* private functions */

#define HEAP_PARENT(i) (((i)+1) / 2 - 1)
#define HEAP_LEFT(i) ((i) * 2 + 1)
#define HEAP_RIGHT(i) ((i) * 2 + 2)

static void
heap_resize(cc_heap * h, unsigned int newsize)
{
  /* Never shrink the heap */
  if (h->size >= newsize)
    return;

  h->array = (void **) realloc(h->array, newsize * sizeof(void *));
  assert(h->array);
  h->size = newsize;
}

static void
heap_heapify(cc_heap * h, unsigned int i)
{
  unsigned int left = HEAP_LEFT(i);
  unsigned int right = HEAP_RIGHT(i);
  unsigned int largest;

  /* Check which node is larger of i and its two children; if any
   * of them is larger swap it with i and recurse down on the child
   */
  if (left < h->elements && h->compare(h->array[left], h->array[i]) > 0)
    largest = left;
  else
    largest = i;

  if (right < h->elements && h->compare(h->array[right], h->array[largest]) > 0)
    largest = right;

  if (largest != i) {
    void * tmp = h->array[largest];
    h->array[largest] = h->array[i];
    h->array[i] = tmp;

    if (h->support_remove) {
      cc_hash_put(h->hash, (unsigned long)h->array[i], (void*)i);
      cc_hash_put(h->hash, (unsigned long)h->array[largest], (void*)largest);
    }

    heap_heapify(h, largest);
  }
}

/* ********************************************************************** */
/* public api */

/*!

  Construct a heap. \a size is the initial array size.

  \a comparecb should return a negative value if the first element
  is less than the second, zero if they are equal and a positive value
  if the first element is greater than the second.

  \a support_remove specifies if the heap should support removal of
  elements (other than the top element) after they are added; this
  requires use of a hash table to be efficent, but as a slight runtime
  overhead will be incurred for the add and extract_top functions the
  support can be disabled if you don't need it.

*/

cc_heap *
cc_heap_construct(unsigned int size,
                  cc_heap_compare_cb * comparecb,
                  SbBool support_remove)
{
  cc_heap * h = (cc_heap *) malloc(sizeof(cc_heap));
  assert(h);

  h->size = size;
  h->elements = 0;
  h->array = (void **) malloc(size * sizeof(void *));
  assert(h->array);
  h->compare = comparecb;
  h->support_remove = support_remove;
  h->hash = NULL;
  if (support_remove) {
    h->hash = cc_hash_construct(size, 0.0f);
  }
  return h;
}

/*!
  Destruct the heap \a h.
*/
void
cc_heap_destruct(cc_heap * h)
{
  cc_heap_clear(h);
  free(h->array);
  if (h->hash) cc_hash_destruct(h->hash);
  free(h);
}

/*!
  Clear/remove all elements in the heap \a h.
*/
void cc_heap_clear(cc_heap * h)
{
  h->elements = 0;
  if (h->hash) cc_hash_clear(h->hash);
}

/*!
  Add the element \a o to the heap \a h.
*/
void
cc_heap_add(cc_heap * h, void * o)
{
  unsigned int i;

  /* Resize the heap if it is full or the threshold is exceeded */
  if (h->elements == h->size) {
    heap_resize(h, h->size * 2);
  }

  i = h->elements++;

  /* If o is greater than its parent, swap them and check again */
  while (i > 0 && h->compare(o, h->array[HEAP_PARENT(i)]) > 0) {
    h->array[i] = h->array[HEAP_PARENT(i)];

    if (h->support_remove) {
      cc_hash_put(h->hash, (unsigned long)h->array[i], (void*)i);
    }

    i = HEAP_PARENT(i);
  }
  h->array[i] = o;

  if (h->support_remove) {
    cc_hash_put(h->hash, (unsigned long)o, (void*)i);
  }
}

/*!
  Returns the top element from the heap \a h. If the heap is empty,
  NULL is returned.
*/
void *
cc_heap_get_top(cc_heap * h)
{
  if (h->elements == 0) return NULL;
  return h->array[0];
}

/*!
  Returns and removes the top element from the heap \a h. If the
  heap is empty, NULL is returned.
*/
void *
cc_heap_extract_top(cc_heap * h)
{
  void * top;
  if (h->elements == 0) return NULL;

  top = h->array[0];
  h->array[0] = h->array[--h->elements];

  if (h->support_remove) {
    cc_hash_put(h->hash, (unsigned long)h->array[0], (void*)0);
    cc_hash_remove(h->hash, (unsigned long)top);
  }

  heap_heapify(h, 0);

  return top;
}

/*!

  Remove \a o from the heap \a h; if present TRUE is returned,
  otherwise FALSE.  Please note that the heap must have been created
  with support_remove.

*/
int
cc_heap_remove(cc_heap * h, void * o)
{
  unsigned int i;
  void * tmp;

  if (!h->support_remove) return FALSE;

  if (!cc_hash_get(h->hash, (unsigned long)o, &tmp))
    return FALSE;

  i = (unsigned int) tmp;
  assert(i < h->elements);
  assert(h->array[i] == o);

  h->array[i] = h->array[--h->elements];
  if (h->support_remove) {
    cc_hash_put(h->hash, (unsigned long) h->array[i], (void*) i);
  }
  heap_heapify(h, i);

  cc_hash_remove(h->hash, (unsigned long)o);

  return TRUE;
}

/*!
  Returns the number of elements in the heap \a h.
*/
unsigned int
cc_heap_elements(cc_heap * h)
{
  return h->elements;
}

/*!
  Returns TRUE of the heap \a h is empty; otherwise FALSE.
*/
SbBool
cc_heap_empty(cc_heap * h)
{
  return h->elements == 0 ? TRUE : FALSE;
}

#undef HEAP_LEFT
#undef HEAP_PARENT
#undef HEAP_RIGHT
