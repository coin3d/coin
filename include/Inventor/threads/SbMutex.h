#ifndef COIN_SBMUTEX_H
#define COIN_SBMUTEX_H

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

#include <Inventor/SbBasic.h>
#include <Inventor/C/threads/mutex.h>

class SbMutex {
public:
  SbMutex(void) { this->mutex = cc_mutex_construct(); }
  ~SbMutex(void) { cc_mutex_destruct(this->mutex); }

  int lock(void) {
    return cc_mutex_lock(this->mutex) == CC_OK ? 0 : 1;
  }
  SbBool tryLock(void) {
    return cc_mutex_try_lock(this->mutex) == CC_OK;
  }
  int unlock(void) {
    return cc_mutex_unlock(this->mutex) == CC_OK ? 0 : 1;
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
