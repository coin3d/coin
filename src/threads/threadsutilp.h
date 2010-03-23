#ifndef CC_THREADUTILP_H
#define CC_THREADUTILP_H

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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* ! COIN_INTERNAL */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_THREADS

#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/sync.h>

#include "threads/mutexp.h"

#define CC_MUTEX_CONSTRUCT(_mymutex_) \
  do { \
    cc_mutex_global_lock(); \
    if (_mymutex_ == NULL) { \
      _mymutex_ = static_cast<void*>(cc_mutex_construct()); \
    } \
    cc_mutex_global_unlock(); \
  } while (0)

#define CC_MUTEX_DESTRUCT(_mymutex_) \
  cc_mutex_destruct(static_cast<cc_mutex*>(_mymutex_));     \
  _mymutex_ = NULL

#define CC_MUTEX_LOCK(_mymutex_) \
  cc_mutex_lock(static_cast<cc_mutex *>(_mymutex_))

#define CC_MUTEX_UNLOCK(_mymutex_) \
  cc_mutex_unlock(static_cast<cc_mutex *>(_mymutex_))

#define CC_SYNC_BEGIN(_myid_) \
  void * coin_mydummysyncptr = cc_sync_begin((void*) _myid_)

#define CC_SYNC_END(_myid_) \
  cc_sync_end(coin_mydummysyncptr)

#define CC_GLOBAL_LOCK cc_mutex_global_lock()
#define CC_GLOBAL_UNLOCK cc_mutex_global_unlock()

#else /* ! HAVE_THREADS */

#define CC_MUTEX_CONSTRUCT(_mymutex_)  do { } while (0)
#define CC_MUTEX_DESTRUCT(_mymutex_)  do { } while (0)
#define CC_MUTEX_LOCK(_mymutex_)  do { } while (0)
#define CC_MUTEX_UNLOCK(_mymutex_)  do { } while (0)
#define CC_SYNC_BEGIN(_myid_)  do { } while (0)
#define CC_SYNC_END(_myid_)  do { } while (0)
#define CC_GLOBAL_LOCK  do { } while (0)
#define CC_GLOBAL_UNLOCK  do { } while (0)

#endif /* ! HAVE_THREADS */

#endif /* CC_THREADUTILP_H */
