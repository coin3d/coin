#ifndef COIN_SBCONDVAR_H
#define COIN_SBCONDVAR_H

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
#include <Inventor/SbTime.h>
#include <Inventor/C/threads/condvar.h>
#include <Inventor/threads/SbMutex.h>

class SbCondVar {
public:
  SbCondVar(void) { this->condvar = cc_condvar_construct(); }
  ~SbCondVar(void) { cc_condvar_destruct(this->condvar); }

  SbBool wait(SbMutex & mutex) { 
    return cc_condvar_wait(this->condvar, mutex.mutex) == CC_OK; 
  }
  SbBool timedWait(SbMutex & mutex, SbTime period) {
    return cc_condvar_timed_wait(this->condvar, mutex.mutex, period.getValue()) == CC_OK;
  }
  
  void wakeOne(void) { cc_condvar_wake_one(this->condvar); }
  void wakeAll(void) { cc_condvar_wake_all(this->condvar); }

private:
  cc_condvar * condvar;
};

#endif // !COIN_SBCONDVAR_H
