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

#include <Inventor/C/base/memalloc.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>

/* internal struct used to store a linked list of free'ed items */
struct cc_memalloc_free {
  struct cc_memalloc_free * next;
};

typedef struct cc_memalloc_free cc_memalloc_free;

/* internal struct used to organize a block of allocated memory */
struct cc_memalloc_memnode {
  struct cc_memalloc_memnode * next;
  unsigned char * block;
  unsigned int currpos;
  unsigned int size;
};

typedef struct cc_memalloc_memnode cc_memalloc_memnode;

/* allocator struct */
struct cc_memalloc {

  cc_memalloc_free * free;
  cc_memalloc_memnode * memnode;

  unsigned int requested_size;
  unsigned int allocated_size;

  unsigned int num_allocated_units;
  cc_memalloc_strategy_cb * strategy;
};

/*
 * allocate 'numbytes' bytes from 'memnode'. Returns NULL if
 * the memory node is full.
 */
static void *
node_alloc(struct cc_memalloc_memnode * memnode, const int numbytes)
{
  unsigned char * ret = NULL;
  if (memnode->currpos + numbytes <= memnode->size) {
    ret = memnode->block + memnode->currpos;
    memnode->currpos += numbytes;
  }
  return ret;
}

/*
 * creates a new memory node for the allocator. Sets the next
 * pointer to the current memnode in allocator.
 */
static struct cc_memalloc_memnode *
create_memnode(cc_memalloc * allocator)
{
  int numbytes;
  cc_memalloc_memnode * node =
    (cc_memalloc_memnode*) malloc(sizeof(cc_memalloc_memnode));
  numbytes = allocator->allocated_size * allocator->strategy(allocator->num_allocated_units);
  
  node->next = allocator->memnode;
  node->block = (unsigned char*) malloc(numbytes);
  node->currpos = 0;
  node->size = numbytes;

  return node;
}

/*
 * Allocate memory from the allocator's memnode. If the memnode is
 * full, a new memnode is created for the allocator.
*/
static void *
alloc_from_memnode(cc_memalloc * allocator)
{
  int unitsize;
  void * ret = NULL;

  unitsize = allocator->allocated_size;

  if (allocator->memnode) ret = node_alloc(allocator->memnode, unitsize);
  if (ret == NULL) {
    allocator->memnode = create_memnode(allocator);
    ret = node_alloc(allocator->memnode, unitsize);
    assert(ret);
  }
  return ret;
}

/*!
  Construct a memory allocator. Each allocated unit will be \a unitsize
  bytes.
*/
cc_memalloc *
cc_memalloc_construct(const unsigned int unitsize)
{
  cc_memalloc * allocator = (cc_memalloc*)
    malloc(sizeof(cc_memalloc));

  allocator->allocated_size = unitsize;
  if (unitsize < sizeof(cc_memalloc_free)) {
    allocator->allocated_size = sizeof(cc_memalloc_free);
  }
  allocator->requested_size = unitsize;
  allocator->free = NULL;
  allocator->memnode = NULL;
  allocator->num_allocated_units = 0;

  cc_memalloc_set_strategy(allocator, NULL); /* will insert default handler */

  return allocator;
}

/*!
  Destruct \a allocator, freeing all memory used.
*/
void
cc_memalloc_destruct(cc_memalloc * allocator)
{
  cc_memalloc_clear(allocator);
  free(allocator);
}

/*!
  Allocate a memory unit from \a allocator.
*/
void *
cc_memalloc_allocate(cc_memalloc * allocator)
{
  allocator->num_allocated_units++;
  if (allocator->free) {
    void * storage = allocator->free;
    allocator->free = allocator->free->next;
    return storage;
  }
  return alloc_from_memnode(allocator);
}

/*!
  Deallocate a memory unit. \a ptr must have been allocated using
  cc_memalloc_allocate(), of course.
*/
void
cc_memalloc_deallocate(cc_memalloc * allocator, void * ptr)
{
  cc_memalloc_free * newfree = (cc_memalloc_free*) ptr;
  allocator->num_allocated_units--;
  newfree->next = allocator->free;
  allocator->free = newfree;
}

/*!
  Free all memory allocated by \a allocator.
*/
void
cc_memalloc_clear(cc_memalloc * allocator)
{
  cc_memalloc_memnode * tmp;
  cc_memalloc_memnode * node = allocator->memnode;
  while (node) {
    tmp = node->next;
    free(node->block);
    free(node);
    node = tmp;
  }
  allocator->free = NULL;
  allocator->memnode = NULL;
}

/* default strategy cb */
static int
default_strategy(const int numunits_allocated)
{
  if (numunits_allocated < 64) return 64;
  return numunits_allocated;
}

/*!
  Sets the allocator strategy callback. \cb should be a function that
  returns the number of units to allocated in a block, bases on the
  number of units currently allocated. The default strategy is to
  just return the number of units allocated, unless the number of
  units allocated is less than 64, then 64 is returned.
*/
void
cc_memalloc_set_strategy(cc_memalloc * allocator, cc_memalloc_strategy_cb * cb)
{
  if (cb == NULL) allocator->strategy = default_strategy;
  else allocator->strategy = cb;
}
