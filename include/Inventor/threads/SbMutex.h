#ifndef COIN_SBMUTEX_H
#define COIN_SBMUTEX_H

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
#include <Inventor/C/threads/mutex.h>

// *************************************************************************

class SbMutex {
public:
  SbMutex(void) { this->mutex = cc_mutex_construct(); }
  ~SbMutex(void) { cc_mutex_destruct(this->mutex); }
  
  SbBool lock(void) { return cc_mutex_lock(this->mutex); }
  SbBool tryLock(void) { return cc_mutex_try_lock(this->mutex); }
  SbBool unlock(void) { return cc_mutex_unlock(this->mutex); }

private:
  cc_mutex * mutex;
};

// *************************************************************************

#endif // ! COIN_SBMUTEX_H
