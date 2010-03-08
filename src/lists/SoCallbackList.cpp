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
  \class SoCallbackList SoCallbackList.h Inventor/lists/SoCallbackList.h
  \brief The SoCallbackList is a container for callback function pointers.
  \ingroup general

  This list stores callback function pointers (along with
  user-specified extra data to pass to the callbacks) and provides a
  method for triggering the callback functions.
*/

/*! \file SoCallbackList.h */
#include <Inventor/lists/SoCallbackList.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  Default constructor.
*/
SoCallbackList::SoCallbackList(void)
{
}

/*!
  Destructor.
*/
SoCallbackList::~SoCallbackList(void)
{
}

/*!
  Append the callback function \a f to the list. It will be passed the
  \a userdata upon invocation.
*/
void
SoCallbackList::addCallback(SoCallbackListCB * f, void * userdata)
{
  // FIXME: Shouldn't we check if the callback is already in the list?
  // 20050723 kyrah.
  this->funclist.append((void*)f);
  this->datalist.append(userdata);
}

/*!
  Remove callback \a f from the list.
*/
void
SoCallbackList::removeCallback(SoCallbackListCB * f, void * userdata)
{
  int idx = this->getNumCallbacks() - 1;

  // FIXME: Why are we not using a dictionary here? (The question is
  // of course whether it should be allowed to have the same callback
  // entry in the list twice...) 20050723 kyrah.
  while (idx != -1) {
    if ((this->funclist[idx] == (void*)f) && (this->datalist[idx] == userdata)) break;
    idx--;
  }

#if COIN_DEBUG
  // FIXME: Is this warning really necessary? Shouldn't it be possible
  // to do the equivalent of setCallback(NULL,NULL) -- i.e. "remove if
  // already exists, else do nothing"? 20050723 kyrah.
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
  Remove all callbacks in the list.
*/
void
SoCallbackList::clearCallbacks(void)
{
  this->funclist.truncate(0);
  this->datalist.truncate(0);
}

/*!
  Returns number of callback functions.
*/
int
SoCallbackList::getNumCallbacks(void) const
{
  return this->funclist.getLength();
}

/*!
  Invoke all callback functions, passing the userdata and the \a
  callbackdata as the first and second argument, respectively.

  All callbacks registered when the method is invoked will be
  triggered, even though if the code in one callback removes another
  callback.

  It is safe for a callback to remove itself or any other callbacks
  during execution.
*/
void
SoCallbackList::invokeCallbacks(void * callbackdata)
{
  SbPList flcopy(this->funclist);
  SbPList dlcopy(this->datalist);

  for (int idx=0; idx < flcopy.getLength(); idx++) {
    SoCallbackListCB * func = (SoCallbackListCB*) flcopy[idx];
    func(dlcopy.operator[](idx), callbackdata);
  }
}
