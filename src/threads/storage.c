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

#include <Inventor/C/threads/storage.h>
#include <Inventor/C/threads/storagep.h>

#include <stdlib.h>
#include <assert.h>

/* ********************************************************************** */

/* the header - used for internal destructor management - can't be sure
   the cc_storage object still exists... */

typedef
struct cc_storage_env {
  void (*destructor)(void *);
  /* userdata follows here */
} cc_storage_env;

static void cc_storage_internal_destructor(void * env);

/*
 * implement user-settable constructor and destructor if possible on
 * non-pthread platforms (must be)
 */

/* ********************************************************************** */

/*
*/

void
cc_storage_struct_init(cc_storage * storage_struct, unsigned int size,
                        void (*constr)(void *), void (*destr)(void *))
{
  assert(storage_struct != NULL);

  storage_struct->size = size;
  storage_struct->constructor = constr;
  storage_struct->destructor = destr;
#ifdef USE_PTHREAD
  if (pthread_key_create(&storage_struct->pthread.key,
                         cc_storage_internal_destructor) != 0 ) {
    assert(0 && "unable to create key");
  }
#endif /* USE_PTHREAD */
}

/*
*/

void
cc_storage_struct_clean(cc_storage * storage_struct)
{
  assert(storage_struct != NULL);
}

/* ********************************************************************** */

/*
*/

cc_storage *
cc_storage_construct(unsigned int size)
{
  cc_storage * storage = (cc_storage *) malloc(sizeof(cc_storage));
  assert(storage != NULL);
  cc_storage_struct_init(storage, size, NULL, NULL);
  return storage;
}

cc_storage *
cc_storage_construct_etc(unsigned int size,
                         void (*constructor)(void *),
                         void (*destructor)(void *))
{
  cc_storage * storage = (cc_storage *) malloc(sizeof(cc_storage));
  assert(storage != NULL);
  cc_storage_struct_init(storage, size, constructor, destructor);
  return storage;
}

/*
*/
void
cc_storage_destruct(cc_storage * storage)
{
  assert(storage != NULL);

#ifdef USE_PTHREAD
  if (pthread_key_delete(storage->pthread.key) != 0) {
    assert(0 && "error while deleting key");
  }
#endif /* USE_PTHREAD */
  free(storage);
}

/* ********************************************************************** */

/*
*/

void *
cc_storage_get(cc_storage * storage)
{
  cc_storage_env * envelope;
  void * user;
  assert(storage != NULL);

#ifdef USE_PTHREAD
  envelope = (cc_storage_env *) pthread_getspecific(storage->pthread.key);
  if (envelope == NULL) {
    envelope =
      (cc_storage_env *) malloc(sizeof(cc_storage_env)+storage->size);
    assert( envelope != NULL );
    envelope->destructor = storage->destructor;
    pthread_setspecific(storage->pthread.key, envelope);
    user = (void *) (((char *) envelope) + sizeof(cc_storage_env));
    if (storage->constructor != NULL)
      (*storage->constructor)(user);
  }
#endif /* USE_PTHREAD */
  
  return (void *) (((char *) envelope) + sizeof(cc_storage_env));
}

/* ********************************************************************** */

/*
*/

void
cc_storage_set_constructor(cc_storage * storage, void (*constructor)(void *))
{
  assert(storage != NULL);
  storage->constructor = constructor;
}

/*
*/

void
cc_storage_set_destructor(cc_storage * storage,
                          void (*destructor)(void *))
{
  assert(storage != NULL);
  storage->destructor = destructor;
}

/* ********************************************************************** */

/*
*/

void
cc_storage_internal_destructor(void * env)
{
  cc_storage_env * envelope;
  void * user;
  envelope = (cc_storage_env *) env;
  assert(envelope != NULL);
  if (envelope->destructor != NULL) {
    user = (void *) (((char *) envelope) + sizeof(cc_storage_env));
    (*envelope->destructor)(user);
  }
  free(env);
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

/* ********************************************************************** */
