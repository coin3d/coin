/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/C/threads/storage.h>
#include <Inventor/C/threads/storagep.h>
#include <Inventor/C/threads/thread.h>
#include <Inventor/C/threads/mutex.h>

#include <stdlib.h>
#include <assert.h>


/* ********************************************************************** */
/* private functions */

static cc_storage *
cc_storage_init(unsigned int size, void (*constructor)(void *), 
                void (*destructor)(void *)) 
{
  cc_storage * storage = (cc_storage *) malloc(sizeof(cc_storage));
  storage->size = size;
  storage->constructor = constructor;
  storage->destructor = destructor;
  storage->dict = cc_hash_construct(8, 0.75f);
  storage->mutex = cc_mutex_construct();

  return storage;
}

static void
cc_storage_hash_destruct_cb(unsigned long key, void * val, void * closure)
{
  cc_storage * storage = (cc_storage*) closure;
  
  if (storage->destructor) {
    storage->destructor(val);
  }
  free(val);
}

/* ********************************************************************** */
/* public api */

cc_storage *
cc_storage_construct(unsigned int size)
{
  return cc_storage_init(size, NULL, NULL);
}

cc_storage *
cc_storage_construct_etc(unsigned int size,
                         void (*constructor)(void *),
                         void (*destructor)(void *))
{
  return cc_storage_init(size, constructor, destructor);
}

/*
*/
void
cc_storage_destruct(cc_storage * storage)
{
  assert(storage != NULL);

  cc_hash_apply(storage->dict, cc_storage_hash_destruct_cb, storage);
  cc_hash_destruct(storage->dict);

  cc_mutex_destruct(storage->mutex);

  free(storage);
}

/* ********************************************************************** */

/*
*/

void *
cc_storage_get(cc_storage * storage)
{
  void * val;
  unsigned long threadid = cc_thread_id();

  cc_mutex_lock(storage->mutex);

  if (!cc_hash_get(storage->dict, threadid, &val)) {
    val = malloc(storage->size);
    if (storage->constructor) {
      storage->constructor(val);
    }
    (void) cc_hash_put(storage->dict, threadid, val);
  }
  cc_mutex_unlock(storage->mutex);

  return val;
}

/* struct needed for cc_hash wrapper callback */
typedef struct {
  cc_storage_apply_func * func;
  void * closure;
} cc_storage_hash_apply_data; 

/* callback from cc_hash_apply. will simply call the function specified
   in cc_storage_apply_to_appl */
static void 
storage_hash_apply(unsigned long key, void * val, void * closure)
{
  cc_storage_hash_apply_data * data = 
    (cc_storage_hash_apply_data*) closure;
  data->func(val, data->closure);
}

void 
cc_storage_apply_to_all(cc_storage * storage, 
                        cc_storage_apply_func * func, 
                        void * closure)
{
  /* need to set up a struct to use cc_hash_apply */
  cc_storage_hash_apply_data mydata;
  
  /* store func and closure in struct */
  mydata.func = func;
  mydata.closure = closure;

  cc_mutex_lock(storage->mutex);
  cc_hash_apply(storage->dict, storage_hash_apply, &mydata);
  cc_mutex_unlock(storage->mutex);
}


/* ********************************************************************** */

void 
cc_storage_thread_cleanup(unsigned long threadid)
{
  /* FIXME: remove and destruct all data for this thread for all storages */
}

/* ********************************************************************** */


/*!
  \class SbStorage Inventor/threads/SbStorage.h
  \brief The SbStorage class manages thread-local memory.
  \ingroup multi-threading

  This class manages thread-local memory.  When different threads access
  the memory an SbStorage object manages, they will receive different
  memory blocks back.  This is one of the most common techniques used
  to make old thread-unsafe libraries thread-safe when it is the sharing
  of read/write static data inside the library that is the problem.
*/

/*!
  \fn SbStorage::SbStorage(unsigned int size)

  Constructor.  \c size specifies the number of bytes each thread should
  have in this thread-local memory management object.
*/

/*!
  \fn SbStorage::SbStorage(unsigned int size, void (*constr)(void *), void (*destr)(void *))

  Constructor.  \c size specifies the number of bytes each thread should
  have in this thread-local memory management object.  A constructor and
  a destructor functions can be given that will be called when the actual
  memory blocks are allocated and freed.
*/

/*!
  \fn SbStorage::~SbStorage(void)

  The destructor.
*/

/*!
  \fn void * SbStorage::get(void)

  This method returns the calling thread's thread-local memory block.
*/

/*!
  \fn void SbStorage::applyToAll(cc_storage_apply_func * func, void * closure)
  
  This method will call \a func for all thread local storage data.
  \a closure will be supplied as the second parameter to the callback.
*/

/* ********************************************************************** */
