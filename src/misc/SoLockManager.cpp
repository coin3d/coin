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

#include <string.h>

#include <Inventor/SbString.h>

#include "tidbitsp.h"

class SoLockManager_pimpl {
public:
  SoLockManager_pimpl(void) { this->unlockstr = NULL; }
  ~SoLockManager_pimpl() { delete this->unlockstr; }

  char * unlockstr;
};

static SoLockManager_pimpl * solockmanager_pimpl = NULL;

static void solockmanager_cleanup(void)
{
  delete solockmanager_pimpl;
  solockmanager_pimpl = NULL;
}

/*!
  A void method provided just for sourcecode compatibility in client
  applications with TGS Inventor.

  It just stores the \a unlockstr argument internally to be able to
  provide it upon calls to GetUnlockString().
*/
void
SoLockManager::SetUnlockString(char * unlockstr)
{
  if (!solockmanager_pimpl) {
    solockmanager_pimpl = new SoLockManager_pimpl;
    coin_atexit((coin_atexit_f*)solockmanager_cleanup, CC_ATEXIT_NORMAL);
  }
  delete solockmanager_pimpl->unlockstr;
  solockmanager_pimpl->unlockstr = new char[strlen(unlockstr) + 1];
  (void)strcpy(solockmanager_pimpl->unlockstr, unlockstr);
}

/*!
  A void method provided just for sourcecode compatibility in client
  applications with TGS Inventor.

  Returns the string set through SetUnlockString().
*/
char *
SoLockManager::GetUnlockString(void)
{
  return solockmanager_pimpl ? solockmanager_pimpl->unlockstr : NULL;
}
