/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

// This is just a dummy implementation of the Open Inventor license
// locking mechanism.


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

void
SoLockManager::SetUnlockString(char * unlockstr)
{
  if (!SoLockManager::pimpl) // FIXME: deallocate on exit. 20000518 mortene.
    SoLockManager::pimpl = new SoLockManager_pimpl;
  delete SoLockManager::pimpl->unlockstr;
  SoLockManager::pimpl->unlockstr = new char[strlen(unlockstr) + 1];
  (void)strcpy(SoLockManager::pimpl->unlockstr, unlockstr);
}

char *
SoLockManager::GetUnlockString(void)
{
  return SoLockManager::pimpl ? SoLockManager::pimpl->unlockstr : NULL;
}
