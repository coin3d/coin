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

#include <Inventor/C/threads/fifo.h>
#include <Inventor/C/threads/fifop.h>
#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/mutexp.h>
#include <Inventor/C/threads/condvar.h>
#include <Inventor/C/threads/condvarp.h>

#include <Inventor/C/errors/debugerror.h>
#include <stdlib.h>
#include <assert.h>

/*!
  \struct cc_fifo Inventor/C/threads/fifo.h
  \brief The cc_fifo is a C datatype for managing a pointer first-in, first-out queue.
  \ingroup threads
*/

/* ********************************************************************** */

static cc_fifo_item * cc_fifo_item_new(void);
static void cc_fifo_item_delete(cc_fifo_item * item);

static cc_fifo_item * i_get_free_item(cc_fifo * fifo);
static void i_append(cc_fifo * fifo, cc_fifo_item * item);
static cc_fifo_item * i_unlink_head(cc_fifo * fifo);

/* ********************************************************************** */

/*!
*/

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
} /* cc_fifo_struct_init() */

/*!
*/

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
} /* cc_fifo_struct_clean() */

/* ********************************************************************** */

/*!
  constructor.
*/

cc_fifo *
cc_fifo_new(void)
{
  cc_fifo * fifo;
  fifo = (cc_fifo*) malloc(sizeof(cc_fifo));
  cc_fifo_struct_init(fifo);
  return fifo;
} /* cc_fifo_construct() */

/*!
  destructor.
*/

void
cc_fifo_delete(cc_fifo * fifo)
{
  cc_fifo_struct_clean(fifo);
  free(fifo);
} /* cc_fifo_destruct() */

/* ********************************************************************** */

/*!
  puts pointer \a ptr of type \a type into the fifo.
*/

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
} /* cc_fifo_assign() */

/*!
  reads a pointer from the fifo.  blocks until a pointer is available for
  reading.
  NULL can be given as the \a type argument if type data is uninteresting.
*/

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
} /* cc_fifo_retrieve() */

/*!
  tries to read a pointer from the fifo.  if no data can be read, FALSE is
  returned, and TRUE otherwise.  the function does not block.
  NULL can be given as the \a type argument if type data is uninteresting.
*/

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
} /* cc_fifo_try_retrieve() */

/* ********************************************************************** */

/*!
  returns the number of pointers put on the fifo.
*/

unsigned int
cc_fifo_size(cc_fifo * fifo)
{
  assert(fifo != NULL);
  return fifo->elements;
} /* cc_fifo_size() */

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
} /* cc_fifo_item_new() */

void
cc_fifo_item_delete(cc_fifo_item * item) /* static */
{
  assert(item != NULL);
  free(item);
} /* cc_fifo_item_delete() */

/* ********************************************************************** */

/*!
  Blocks until the fifo can be locked.
*/

void
cc_fifo_lock(cc_fifo * fifo)
{
  assert(fifo != NULL);
  cc_mutex_lock(&fifo->access);
} /* cc_fifo_lock() */

/*!
  Returns TRUE if locking the fifo immediately was successful, and
  FALSE otherwise.
*/

SbBool
cc_fifo_try_lock(cc_fifo * fifo)
{
  assert(fifo != NULL);
  return cc_mutex_try_lock(&fifo->access);
} /* cc_fifo_try_lock() */

/*!
  Unlocks the fifo.
*/

void
cc_fifo_unlock(cc_fifo * fifo)
{
  assert(fifo != NULL);
  cc_mutex_unlock(&fifo->access);
} /* cc_fifo_unlock() */

/* ********************************************************************** */

/*!
  Peeks at the head item of the fifo without removing it.
  In the case where the fifo is empty, FALSE is returned.

  The fifo must be locked in advance with cc_fifo_lock() before using
  this function.
*/

SbBool
cc_fifo_peek(cc_fifo * fifo, void ** item, uint32_t * type)
{
  assert(fifo != NULL);
  if ( fifo->head == NULL ) return FALSE;
  *item = fifo->head->item;
  if ( type != NULL ) *type = fifo->head->type;
  return TRUE;
} /* cc_fifo_peek() */

/*!
  returns TRUE of FALSE on whether the item is in the fifo queue.

  The fifo must be locked in advance with cc_fifo_lock() before using
  this function.
*/

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
} /* cc_fifo_contains() */

/*!
  this function removes the item from the fifo queue.
  returns TRUE of FALSE on whether the item was in the fifo queue in the
  first place.

  The fifo must be locked in advance with cc_fifo_lock() before using
  this function.
*/

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
} /* cc_fifo_reclaim() */

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
} /* i_get_free_item() */

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
} /* i_append() */

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
} /* i_unlink_head() */

/* ********************************************************************** */
