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

#include <Inventor/C/base/memalloc.h>

#include <cstdlib>
#include <cstddef>
#include <cassert>
#include <climits>
#include <cstdio>

#include "coindefs.h"

#ifndef COIN_WORKAROUND_NO_USING_STD_FUNCS
using std::malloc;
using std::free;
#endif // !COIN_WORKAROUND_NO_USING_STD_FUNCS

/* ********************************************************************** */

/*!
  \struct cc_memalloc memalloc.h Inventor/C/base/memalloc.h

  The allocator structure for memory.
*/

/*!
  \typedef struct cc_memalloc cc_memalloc

  A type definition for the memory allocator structure.
*/

/*!
  \typedef int cc_memalloc_strategy_cb(const int numunits_allocated)

  The type definition for the memory allocator strategy callback function.
*/

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

  unsigned int chunksize;

  unsigned int num_allocated_units;
  cc_memalloc_strategy_cb * strategy;
};

/*
 * allocate 'numbytes' bytes from 'memnode'. Returns NULL if
 * the memory node is full.
 */
static void *
node_alloc(struct cc_memalloc_memnode * memnode, const unsigned int numbytes)
{
  unsigned char * ret = NULL;
  if (memnode->currpos <= memnode->size &&
      numbytes <= memnode->size - memnode->currpos) {
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
  const int chunkmultiplier = allocator->strategy(allocator->num_allocated_units);
  if (chunkmultiplier <= 0 ||
      static_cast<unsigned int>(chunkmultiplier) >
        UINT_MAX / allocator->chunksize) return NULL;

  const unsigned int numbytes =
    allocator->chunksize * static_cast<unsigned int>(chunkmultiplier);
  cc_memalloc_memnode * node =
    (cc_memalloc_memnode*) malloc(sizeof(cc_memalloc_memnode));
  if (node == NULL) return NULL;

  node->block = (unsigned char*) malloc(numbytes);
  if (node->block == NULL) {
    free(node);
    return NULL;
  }
  node->next = allocator->memnode;
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
  void * ret = NULL;

  if (allocator->memnode) ret = node_alloc(allocator->memnode, allocator->chunksize);
  if (ret == NULL) {
    cc_memalloc_memnode * node = create_memnode(allocator);
    if (node == NULL) return NULL;
    allocator->memnode = node;
    ret = node_alloc(node, allocator->chunksize);
    assert(ret != NULL);
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
  const size_t alignment = alignof(cc_memalloc_free);
  size_t chunksize = unitsize;
  if (chunksize < sizeof(cc_memalloc_free)) {
    chunksize = sizeof(cc_memalloc_free);
  }
  const size_t remainder = chunksize % alignment;
  if (remainder != 0) {
    const size_t padding = alignment - remainder;
    if (chunksize > UINT_MAX - padding) return NULL;
    chunksize += padding;
  }
  if (chunksize > UINT_MAX) return NULL;

  cc_memalloc * allocator = (cc_memalloc*) malloc(sizeof(cc_memalloc));
  if (allocator == NULL) return NULL;
  allocator->chunksize = static_cast<unsigned int>(chunksize);
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
  if (allocator->num_allocated_units >= INT_MAX) return NULL;
  allocator->num_allocated_units++;
  if (allocator->free) {
    void * storage = allocator->free;
    allocator->free = allocator->free->next;
    return storage;
  }
  void * storage = alloc_from_memnode(allocator);
  if (storage == NULL) allocator->num_allocated_units--;
  return storage;
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
  allocator->num_allocated_units = 0;
}

extern "C" {

/* default strategy cb */
static int
default_strategy(const int numunits_allocated)
{
  if (numunits_allocated < 64) return 64;
  return numunits_allocated;
}

} // extern "C"

/*!
  Sets the allocator strategy callback. \c cb should be a function that
  returns the number of units to allocated in a block, based on the
  number of units currently allocated.

  The default strategy is to just return the number of units allocated
  (which will successively double the internal memory chunk sizes),
  unless the number of units allocated is less than 64, then 64 is
  returned.
*/
void
cc_memalloc_set_strategy(cc_memalloc * allocator, cc_memalloc_strategy_cb * cb)
{
  if (cb == NULL) allocator->strategy = default_strategy;
  else allocator->strategy = cb;
}

#ifdef COIN_TEST_SUITE

#include <cstdint>
#include <climits>

static int memalloc_strategy_input = -1;

static int
memalloc_single_unit_strategy(const int numunits_allocated)
{
  memalloc_strategy_input = numunits_allocated;
  return 1;
}

BOOST_AUTO_TEST_CASE(cc_memalloc_aligns_and_reuses_units)
{
  cc_memalloc * allocator = cc_memalloc_construct(9);
  void * first = cc_memalloc_allocate(allocator);
  void * second = cc_memalloc_allocate(allocator);

  BOOST_CHECK_EQUAL(reinterpret_cast<uintptr_t>(first) %
                    alignof(void *), 0U);
  BOOST_CHECK_EQUAL(reinterpret_cast<uintptr_t>(second) %
                    alignof(void *), 0U);

  cc_memalloc_deallocate(allocator, first);
  cc_memalloc_deallocate(allocator, second);
  BOOST_CHECK(cc_memalloc_allocate(allocator) == second);
  BOOST_CHECK(cc_memalloc_allocate(allocator) == first);

  cc_memalloc_destruct(allocator);
}

BOOST_AUTO_TEST_CASE(cc_memalloc_clear_resets_strategy_count)
{
  cc_memalloc * allocator = cc_memalloc_construct(sizeof(void *));
  cc_memalloc_set_strategy(allocator, memalloc_single_unit_strategy);

  memalloc_strategy_input = -1;
  cc_memalloc_allocate(allocator);
  BOOST_CHECK_EQUAL(memalloc_strategy_input, 1);
  cc_memalloc_allocate(allocator);
  BOOST_CHECK_EQUAL(memalloc_strategy_input, 2);

  cc_memalloc_clear(allocator);
  cc_memalloc_allocate(allocator);
  BOOST_CHECK_EQUAL(memalloc_strategy_input, 1);

  cc_memalloc_destruct(allocator);
}

static int memalloc_test_multiplier;

static int
memalloc_test_strategy(const int numunits_allocated)
{
  memalloc_strategy_input = numunits_allocated;
  return memalloc_test_multiplier;
}

BOOST_AUTO_TEST_CASE(cc_memalloc_rejects_overflow_and_invalid_strategy)
{
  BOOST_CHECK(cc_memalloc_construct(UINT_MAX) == NULL);

  cc_memalloc * allocator = cc_memalloc_construct(64);
  BOOST_REQUIRE(allocator != NULL);
  cc_memalloc_set_strategy(allocator, memalloc_test_strategy);

  memalloc_test_multiplier = 0;
  BOOST_CHECK(cc_memalloc_allocate(allocator) == NULL);
  memalloc_test_multiplier = -1;
  BOOST_CHECK(cc_memalloc_allocate(allocator) == NULL);
  memalloc_test_multiplier = INT_MAX;
  BOOST_CHECK(cc_memalloc_allocate(allocator) == NULL);

  memalloc_test_multiplier = 1;
  void * value = cc_memalloc_allocate(allocator);
  BOOST_REQUIRE(value != NULL);
  BOOST_CHECK_EQUAL(memalloc_strategy_input, 1);
  cc_memalloc_deallocate(allocator, value);
  cc_memalloc_destruct(allocator);
}

#endif // COIN_TEST_SUITE
