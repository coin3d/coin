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

/*!
  \class SoLockManager SoLockMgr.h Inventor/lock/SoLockMgr.h
  \brief The SoLockManager is a defunct software license lock mechanism.
  \ingroup general

  This is just a dummy implementation of the TGS Inventor class used
  to provide a license locking mechanism. Since Coin can be used
  without any royalty fees or client-site license locks, no such
  mechanism is needed.

  If you are looking for information about using Coin in proprietary
  applications for commercial distribution, read about the Coin
  Professional Edition License on the Coin web-pages <a
  href="http://www.coin3d.org">here</a>.
*/


#include <Inventor/lock/SoLockMgr.h>
#include <Inventor/SbString.h>
#include <string.h>

class SoLockManager_pimpl {
public:
  SoLockManager_pimpl(void) { this->unlockstr = NULL; }
  ~SoLockManager_pimpl() { delete this->unlockstr; }

  char * unlockstr;
};

SoLockManager_pimpl * SoLockManager::pimpl = NULL;

/*!
  A void method provided just for sourcecode compatibility in client
  applications with TGS Inventor.

  It just stores the \a unlockstr argument internally to be able to
  provide it upon calls to GetUnlockString().
*/
void
SoLockManager::SetUnlockString(char * unlockstr)
{
  if (!SoLockManager::pimpl) // FIXME: deallocate on exit. 20000518 mortene.
    SoLockManager::pimpl = new SoLockManager_pimpl;
  delete SoLockManager::pimpl->unlockstr;
  SoLockManager::pimpl->unlockstr = new char[strlen(unlockstr) + 1];
  (void)strcpy(SoLockManager::pimpl->unlockstr, unlockstr);
}

/*!
  A void method provided just for sourcecode compatibility in client
  applications with TGS Inventor.

  Returns the string set through SetUnlockString().
*/
char *
SoLockManager::GetUnlockString(void)
{
  return SoLockManager::pimpl ? SoLockManager::pimpl->unlockstr : NULL;
}
