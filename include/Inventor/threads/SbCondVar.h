#ifndef COIN_SBCONDVAR_H
#define COIN_SBCONDVAR_H

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
#include <Inventor/SbTime.h>
#include <Inventor/C/threads/condvar.h>

class SbMutex;

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
