#ifndef COIN_SBRWMUTEX_H
#define COIN_SBRWMUTEX_H

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

#include <Inventor/C/threads/rwmutex.h>

class SbRWMutex {
public:
  enum Precedence {
    READ_PRECEDENCE,
    WRITE_PRECEDENCE
  };

  SbRWMutex(Precedence policy) {
    this->rwmutex = cc_rwmutex_construct_etc(
      (policy == WRITE_PRECEDENCE)? CC_WRITE_PRECEDENCE : CC_READ_PRECEDENCE);
  }
  ~SbRWMutex(void) { cc_rwmutex_destruct(this->rwmutex); }

  int writeLock(void) { return cc_rwmutex_write_lock(this->rwmutex); }
  int tryWriteLock(void) { return cc_rwmutex_write_try_lock(this->rwmutex); }
  int writeUnlock(void) { return cc_rwmutex_write_unlock(this->rwmutex); }

  int readLock(void) { return cc_rwmutex_read_lock(this->rwmutex); }
  int tryReadLock(void) { return cc_rwmutex_read_try_lock(this->rwmutex); }
  int readUnlock(void) { return cc_rwmutex_read_unlock(this->rwmutex); }

private:
  cc_rwmutex * rwmutex;
};

#endif // !COIN_SBRWMUTEX_H
