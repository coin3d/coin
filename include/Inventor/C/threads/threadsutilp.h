#ifndef CC_THREADUTILP_H
#define CC_THREADUTILP_H

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

#ifndef COIN_INTERNAL
#error You have tried to use one of the private Coin header files
#endif /* ! COIN_INTERNAL */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef COIN_THREADSAFE

#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/mutexp.h>
#include <Inventor/C/threads/sync.h>

#define CC_MUTEX_CONSTRUCT(_mymutex_) \
  do { \
    cc_mutex_global_lock(); \
    if (_mymutex_ == NULL) { \
      _mymutex_ = (void*) cc_mutex_construct(); \
    } \
    cc_mutex_global_unlock(); \
  } while (0)

#define CC_MUTEX_DESTRUCT(_mymutex_) \
  cc_mutex_destruct((cc_mutex*) _mymutex_)

#define CC_MUTEX_LOCK(_mymutex_) \
  cc_mutex_lock((cc_mutex*) _mymutex_)

#define CC_MUTEX_UNLOCK(_mymutex_) \
  cc_mutex_unlock((cc_mutex*) _mymutex_)

#define CC_SYNC_BEGIN(_myid_) \
  void * coin_mydummysyncptr = cc_sync_begin((void*) _myid_)

#define CC_SYNC_END(_myid_) \
  cc_sync_end(coin_mydummysyncptr)

#define CC_GLOBAL_LOCK cc_mutex_global_lock()
#define CC_GLOBAL_UNLOCK cc_mutex_global_unlock()

#else /* HAVE_THREADS */

#define CC_MUTEX_CONSTRUCT(_mymutex_)
#define CC_MUTEX_DESTRUCT(_mymutex_)
#define CC_MUTEX_LOCK(_mymutex_)
#define CC_MUTEX_UNLOCK(_mymutex_)
#define CC_SYNC_BEGIN(_myid_)
#define CC_SYNC_END(_myid_)
#define CC_GLOBAL_LOCK
#define CC_GLOBAL_UNLOCK

#endif /* ! HAVE_THREADS */

#endif /* CC_THREADUTILP_H */
