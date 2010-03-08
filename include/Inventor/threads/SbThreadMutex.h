#ifndef COIN_SBTHREADMUTEX_H
#define COIN_SBTHREADMUTEX_H

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
#include <Inventor/C/threads/recmutex.h>

class SbThreadMutex {
public:
  SbThreadMutex(void) { this->mutex = cc_recmutex_construct(); }
  ~SbThreadMutex() { cc_recmutex_destruct(this->mutex); }

  int lock(void) {
    return cc_recmutex_lock(this->mutex);
  }

  SbBool tryLock(void) {
    return cc_recmutex_try_lock(this->mutex) == CC_OK;
  }
  
  int unlock(void) {
    return cc_recmutex_unlock(this->mutex);
  }

private:
  // FIXME: we need access to C mutex structur. Should we use friend,
  // or should we add a new public method to get to this structure?
  // pederb, 2002-06-26
  friend class SbCondVar;
  cc_recmutex * mutex;
};


#ifndef COIN_INTERNAL
// For TGS Inventor compile-time compatibility.
#include <Inventor/threads/SbThreadAutoLock.h>
#endif // COIN_INTERNAL

#endif // !COIN_SBTHREADMUTEX_H
