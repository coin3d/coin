#ifndef COIN_SBCONDVAR_H
#define COIN_SBCONDVAR_H

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

#include <Inventor/SbBasic.h>
#include <Inventor/SbTime.h>
#include <Coin/threads/condvar.h>

// *************************************************************************

class SbCondVar {
public:
  SbCondVar(void) { this->condvar = ccb_condvar_construct(); }
  ~SbCondVar(void) { ccb_condvar_destruct(this->condvar); }

  SbBool wait(void) { return ccb_condvar_wait(this->condvar); }
  SbBool timedWait(SbTime period) {
    return ccb_condvar_timed_wait(this->condvar, period.getValue());
  }

  void wakeOne(void) { ccb_condvar_wake_one(this->condvar); }
  void wakeAll(void) { ccb_condvar_wake_all(this->condvar); }

private:
  ccb_condvar * condvar;
};

// *************************************************************************

#endif // ! COIN_SBCONDVAR_H
