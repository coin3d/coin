/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/C/threads/fifo.h>

#include <stdlib.h>
#include <assert.h>

#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/condvar.h>
#include <Inventor/C/errors/debugerror.h>

#include "threads/fifop.h"
#include "threads/mutexp.h"
#include "threads/condvarp.h"

/*
  The cc_fifo is a C datatype for managing a pointer first-in,
  first-out queue.
*/

/* ********************************************************************** */

static cc_fifo_item * cc_fifo_item_new(void);
static void cc_fifo_item_delete(cc_fifo_item * item);

static cc_fifo_item * i_get_free_item(cc_fifo * fifo);
static void i_append(cc_fifo * fifo, cc_fifo_item * item);
static cc_fifo_item * i_unlink_head(cc_fifo * fifo);

/* ********************************************************************** */

void
cc_fifo_struct_init(cc_fifo * fifo)
{
  assert(fifo != NULL);
  cc_mutex_struct_init(&fifo->access);
  fifo->head = NULL;
  fifo->tail = NULL;
  fifo->free = NULL;
  fifo->elements = 0;
  cc_condvar_struct_init(&fifo->sleep);
}

void
cc_fifo_struct_clean(cc_fifo * fifo)
{
  cc_fifo_item * item, * next;
  assert(fifo != NULL);
  cc_mutex_struct_clean(&fifo->access);
  /* free fifo list */
  item = fifo->head;
  while ( item != NULL ) {
    next = item->next;
    cc_fifo_item_delete(item);
    item = next;
  }
  /* free free list */
  item = fifo->free;
  while ( item != NULL ) {
    next = item->next;
    cc_fifo_item_delete(item);
    item = next;
  }
  cc_condvar_struct_clean(&fifo->sleep);
}

/* ********************************************************************** */

cc_fifo *
cc_fifo_new(void)
{
  cc_fifo * fifo;
  fifo = (cc_fifo*) malloc(sizeof(cc_fifo));
  cc_fifo_struct_init(fifo);
  return fifo;
}

void
cc_fifo_delete(cc_fifo * fifo)
{
  cc_fifo_struct_clean(fifo);
  free(fifo);
}

/* ********************************************************************** */

void
cc_fifo_assign(cc_fifo * fifo, void * ptr, uint32_t type)
{
  cc_fifo_item * item;
  assert(fifo != NULL);
  cc_mutex_lock(&fifo->access);
  item = i_get_free_item(fifo);
  item->item = ptr;
  item->type = type;
  i_append(fifo, item);
  cc_condvar_wake_one(&fifo->sleep);
  cc_mutex_unlock(&fifo->access);
}

void
cc_fifo_retrieve(cc_fifo * fifo, void ** ptr, uint32_t * type)
{
  cc_fifo_item * item;
  assert(fifo != NULL && ptr != NULL);
  cc_mutex_lock(&fifo->access);
  while ( TRUE ) {
    if ( fifo->elements == 0 ) {
      cc_condvar_wait(&fifo->sleep, &fifo->access);
    } else {
      item = i_unlink_head(fifo);
      *ptr = item->item;
      if ( type != NULL ) *type = item->type;
      item->next = fifo->free;
      fifo->free = item;
      cc_mutex_unlock(&fifo->access);
      cc_condvar_wake_one(&fifo->sleep);
      return;
    }
  }
}

SbBool
cc_fifo_try_retrieve(cc_fifo * fifo, void ** ptr, uint32_t * type)
{
  cc_fifo_item * item;
  assert(fifo != NULL && ptr != NULL);
  /* FIXME: consider cc_mutex_try_lock()? to escape even a failed lock */
  if ( ! cc_mutex_try_lock(&fifo->access) ) {
    return FALSE;
  }
  if ( fifo->elements == 0 ) {
    cc_mutex_unlock(&fifo->access);
    return FALSE;
  }
  item = i_unlink_head(fifo);
  *ptr = item->item;
  if ( type != NULL ) *type = item->type;
  cc_fifo_item_delete(item);
  cc_mutex_unlock(&fifo->access);
  cc_condvar_wake_one(&fifo->sleep);
  return TRUE;
}

/* ********************************************************************** */

unsigned int
cc_fifo_size(cc_fifo * fifo)
{
  assert(fifo != NULL);
  return fifo->elements;
}

/* ********************************************************************** */

cc_fifo_item *
cc_fifo_item_new(void) /* static */
{
  cc_fifo_item * item;
  item = (cc_fifo_item*) malloc(sizeof(cc_fifo_item));
  assert(item != NULL);
  item->next = NULL;
  item->item = NULL;
  item->type = 0;
  return item;
}

void
cc_fifo_item_delete(cc_fifo_item * item) /* static */
{
  assert(item != NULL);
  free(item);
}

/* ********************************************************************** */

void
cc_fifo_lock(cc_fifo * fifo)
{
  assert(fifo != NULL);
  cc_mutex_lock(&fifo->access);
}

SbBool
cc_fifo_try_lock(cc_fifo * fifo)
{
  assert(fifo != NULL);
  return cc_mutex_try_lock(&fifo->access);
}

void
cc_fifo_unlock(cc_fifo * fifo)
{
  assert(fifo != NULL);
  cc_mutex_unlock(&fifo->access);
}

/* ********************************************************************** */

SbBool
cc_fifo_peek(cc_fifo * fifo, void ** item, uint32_t * type)
{
  assert(fifo != NULL);
  if ( fifo->head == NULL ) return FALSE;
  *item = fifo->head->item;
  if ( type != NULL ) *type = fifo->head->type;
  return TRUE;
}

SbBool
cc_fifo_contains(cc_fifo * fifo, void * itemptr)
{
  cc_fifo_item * item;
  assert(fifo != NULL);
  item = fifo->head;
  while ( item != NULL ) {
    if ( item->item == itemptr ) return TRUE;
    item = item->next;
  }
  return FALSE;
}

SbBool
cc_fifo_reclaim(cc_fifo * fifo, void * itemptr)
{
  cc_fifo_item * item, * prev;
  assert(fifo != NULL);
  item = fifo->head;
  prev = NULL;
  while ( item != NULL ) {
    if ( item->item == itemptr ) {
      if ( prev == NULL ) fifo->head = item->next;
      else prev->next = item->next;
      if ( fifo->tail == item ) fifo->tail = prev;
      /* and reset/store the container */
      item->item = NULL;
      item->type = 0;
      item->next = fifo->free;
      fifo->free = item;
      return TRUE;
    }
    prev = item;
    item = item->next;
  }
  return FALSE;
}

/* ********************************************************************** */

/*
  get item from free list or construct a new one
*/

cc_fifo_item *
i_get_free_item(cc_fifo * fifo) /* static */
{
  cc_fifo_item * item;
  if ( fifo->free != NULL ) {
    item = fifo->free;
    fifo->free = item->next;
    item->next = NULL;
  } else {
    item = cc_fifo_item_new();
  }
  return item;
}

/*
  append item to fifo.  make sure both ::head and ::tail are correct
  after.
*/

void
i_append(cc_fifo * fifo, cc_fifo_item * item) /* static */
{
  if ( fifo->tail == NULL ) {
    fifo->head = item;
    fifo->tail = item;
  } else {
    fifo->tail->next = item;
    fifo->tail = item;
  }
  fifo->elements += 1;
}

/*
  unlink first item from fifo.  make sure both ::head and ::tail are
  correct after.
*/

cc_fifo_item *
i_unlink_head(cc_fifo * fifo) /* static */
{
  cc_fifo_item * item;
  item = fifo->head;
  fifo->head = item->next;
  if ( fifo->head == NULL )
    fifo->tail = NULL;
  fifo->elements -= 1;
  return item;
}

/* ********************************************************************** */

/*!
  \class SbFifo Inventor/threads/SbFifo.h
  \brief A class for managing a pointer first-in, first-out queue.
  \ingroup threads
*/

/*!
  \fn void SbFifo::assign(void * ptr, uint32_t type)

  Puts pointer \a ptr of type \a type into the fifo.

  The \a type argument is just meant as a user data tag, and a 0 value
  can be given as the \a type argument if type data is uninteresting.
*/

/*!
  \fn void SbFifo::retrieve(void *& ptr, uint32_t &type)

  Reads a pointer from the queue. Blocks until a pointer is available
  for reading.
*/

/*!
  \fn SbBool SbFifo::tryRetrieve(void *& ptr, uint32_t & type)

  Tries to read a pointer from the queue. If no data can be read, \c
  FALSE is returned, and \c TRUE otherwise. The function does not
  block.
*/

/*!
  \fn unsigned int SbFifo::size(void) const

  Returns number of pointers currently in the queue.
*/

/*!
  \fn void SbFifo::lock(void) const

  Blocks until the queue can be locked.
*/

/*!
  \fn void SbFifo::unlock(void) const

  Unlocks the queue.
*/

/*!
  \fn SbBool SbFifo::peek(void *& item, uint32_t & type) const

  Peeks at the head item of the queue without removing it.  In the
  case where the fifo is empty, \c FALSE is returned.

  The queue must be locked with SbFifo::lock() before using this
  function, then unlocked.
*/

/*!
  \fn SbBool SbFifo::contains(void * item) const

  Returns \c TRUE or \c FALSE depending on whether the item is in the
  queue.

  The queue must be locked with SbFifo::lock() before using this
  function, then unlocked.
*/

/*!
  \fn SbBool SbFifo::reclaim(void * item)

  This function removes the given \a item from the queue.  Returns \c
  TRUE or \c FALSE depending on whether the item was in the queue in
  the first place.

  The queue must be locked with SbFifo::lock() before using this
  function, then unlocked.
*/

/* ********************************************************************** */
