#ifndef COIN_SBTHREAD_H
#define COIN_SBTHREAD_H

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
#include <Inventor/C/threads/thread.h>

class SbThread {
public:
  static SbThread * create(void *(*func)(void *), void * closure) {
    return new SbThread(cc_thread_construct(func, closure));
  }
  static void destroy(SbThread * thread) {
    cc_thread_destruct(thread->thread);
    delete thread;
  }

  int join(void ** retval = 0L) {
    return cc_thread_join(this->thread, retval);
  }
  static int join(SbThread * thread, void ** retval = 0L) {
    return cc_thread_join(thread->thread, retval);
  }

protected:
  SbThread(cc_thread * thread) { this->thread = thread; }
  ~SbThread(void) {}

private:
  cc_thread * thread;
};

#endif // !COIN_SBTHREAD_H
