/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

/*!
  \class SoCallbackList Inventor/lists/SoCallbackList.h
  \brief The SoCallbackList is a container class for arrays of callbacks.

  FIXME: write doc.
*/

#include <Inventor/lists/SoCallbackList.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  A constructor (default)
*/

SoCallbackList::SoCallbackList(void)
{
}

/*!
  The destructor.
*/

SoCallbackList::~SoCallbackList(void)
{
  this->clearCallbacks();
}

/*!
  FIXME: write doc.
*/

void
SoCallbackList::addCallback(SoCallbackListCB * f, void * userData)
{
  this->funclist.append((void *)f);
  this->datalist.append(userData);
}

/*!
  FIXME: write doc.
*/

void
SoCallbackList::removeCallback(SoCallbackListCB * f, void * userData)
{
  int idx = this->getNumCallbacks() - 1;

  while (idx != -1) {
    if ((this->funclist[idx] == f) && (this->datalist[idx] == userData)) break;
    idx--;
  }

#if COIN_DEBUG
  if (idx == -1) {
    SoDebugError::post("SoCallbackList::removeCallback",
		       "Tried to remove non-existant callback function.");
    return;
  }
#endif // COIN_DEBUG

  this->funclist.remove(idx);
  this->datalist.remove(idx);
}

/*!
  FIXME: write doc.
*/

void
SoCallbackList::clearCallbacks(void)
{
  this->funclist.truncate(0);
  this->datalist.truncate(0);
}

/*!
  FIXME: write doc.
*/

int
SoCallbackList::getNumCallbacks(void) const
{
  return this->funclist.getLength();
}

/*!
  FIXME: write doc.
*/

void
SoCallbackList::invokeCallbacks(void * cbdata)
{
  int idx = 0;
  while (idx < this->getNumCallbacks()) {
    SoCallbackListCB * func = (SoCallbackListCB *)(this->funclist[idx]);
    func(this->datalist[idx], cbdata);
    idx++;
  }
}

