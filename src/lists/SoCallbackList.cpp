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

/*!
  \class SoCallbackList SoCallbackList.h Inventor/lists/SoCallbackList.h
  \brief The SoCallbackList is a container for callback function pointers.
  \ingroup general

  This list stores callback function pointers (along with
  user-specified extra data to pass to the callbacks) and provides a
  method for triggering the callback functions.
*/

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
  this->funclist.append(f);
  this->datalist.append(userdata);
}

/*!
  Remove callback \a f from the list.
*/
void
SoCallbackList::removeCallback(SoCallbackListCB * f, void * userdata)
{
  int idx = this->getNumCallbacks() - 1;

  while (idx != -1) {
    if ((this->funclist[idx] == f) && (this->datalist[idx] == userdata)) break;
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
*/
void
SoCallbackList::invokeCallbacks(void * callbackdata)
{
  for (int idx=0; idx < this->getNumCallbacks(); idx++) {
    SoCallbackListCB * func = (SoCallbackListCB *)(this->funclist[idx]);
    func(this->datalist[idx], callbackdata);
  }
}
