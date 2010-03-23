#ifndef COIN_SBMUTEX_H
#define COIN_SBMUTEX_H

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

#include <Inventor/SbBasic.h>
#include <Inventor/C/threads/mutex.h>

class SbMutex {
public:
  SbMutex(void) { this->mutex = cc_mutex_construct(); }
  ~SbMutex() { cc_mutex_destruct(this->mutex); }

  int lock(void) {
    cc_mutex_lock(this->mutex);
    /* Useless return, here just for compatibility with TGS'
       SbThreadMutex API: */
    return 0;
  }

  SbBool tryLock(void) {
    return cc_mutex_try_lock(this->mutex) == CC_OK;
  }

  int unlock(void) {
    cc_mutex_unlock(this->mutex);
    /* Useless return, here just for compatibility with TGS'
       SbThreadMutex API: */
    return 0;
  }

private:
  // FIXME: we need access to C mutex structur. Should we use friend,
  // or should we add a new public method to get to this structure?
  // pederb, 2002-06-26
  friend class SbCondVar;
  cc_mutex * mutex;
};


#ifndef COIN_INTERNAL
// For TGS Inventor compile-time compatibility.
#include <Inventor/threads/SbThreadAutoLock.h>
#endif // COIN_INTERNAL

#endif // !COIN_SBMUTEX_H
