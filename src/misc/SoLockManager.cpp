/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
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
