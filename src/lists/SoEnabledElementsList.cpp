/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoEnabledElementsList SoEnabledElementsList.h Inventor/lists/SoEnabledElementsList.h
  \brief The SoEnabledElementsList class is a container for type info for element types that are enabled in actions.
  \ingroup actions

  This class is probably not interesting for the application
  programmer.
*/

// FIXME: doesn't handle post-initialization changes very well (fixing
// this also needs some tempering to be done with SoAction, I think.)
// 20000305 mortene.

#include <Inventor/lists/SoEnabledElementsList.h>
#include <assert.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#if COIN_THREADSAFE
#include <Inventor/threads/SbMutex.h>
#endif // COIN_THREADSAFE

#ifndef DOXYGEN_SKIP_THIS

class SoEnabledElementsListP {
public:
  int prevmerge;
  SoTypeList elements;
  SoEnabledElementsList * parent;
#ifdef COIN_THREADSAFE
  SbMutex mutex;
#endif // COIN_THREADSAFE
  void lock(void) {
#ifdef COIN_THREADSAFE
    this->mutex.lock();
#endif
  }
  void unlock(void) {
#ifdef COIN_THREADSAFE
    this->mutex.unlock();
#endif
  }
};

static int enable_counter = 0;

#endif // DOXYGEN_SKIP_THIS

#undef THIS
#define THIS this->pimpl

/*!
  Constructor.
*/
SoEnabledElementsList::SoEnabledElementsList(SoEnabledElementsList * const parentlist)
{
  THIS = new SoEnabledElementsListP;

  THIS->prevmerge = 0;
  THIS->parent = parentlist;
}

/*!
  Destructor.
*/
SoEnabledElementsList::~SoEnabledElementsList()
{
  delete THIS;
}

/*!
  Return the list of enabled elements.
*/
const SoTypeList &
SoEnabledElementsList::getElements(void) const
{
  THIS->lock();
  // check if we need a new merge
  if (THIS->prevmerge != enable_counter) {
    int storedcounter = enable_counter;
    SoEnabledElementsList * plist = (SoEnabledElementsList*) THIS->parent;
    while (plist) {
      ((SoEnabledElementsList*)this)->merge(*plist);
      plist = plist->pimpl->parent;
    }
    // use and restore old counter since it might change during merge
    ((SoEnabledElementsList*)this)->pimpl->prevmerge =
      enable_counter = storedcounter;
  }
  THIS->unlock();
  return THIS->elements;
}

/*!
  Add an \a elementtype to the list of enabled elements if it's not
  enabled already.
*/
void
SoEnabledElementsList::enable(const SoType elementtype, const int stackindex)
{
  while (stackindex >= THIS->elements.getLength())
    THIS->elements.append(SoType::badType());

  SoType currtype = THIS->elements[stackindex];
  if (currtype.isBad() ||
      (elementtype != currtype && elementtype.isDerivedFrom(currtype))) {
    THIS->elements.set(stackindex, elementtype);
    // increment to detect when a new merge is needed
    enable_counter++;
  }
}

/*!
  Enables all the elements from the \a eel list that is enabled in
  this instance.
*/
void
SoEnabledElementsList::merge(const SoEnabledElementsList & eel)
{
  SoType bad = SoType::badType();
  const int num = eel.pimpl->elements.getLength();
  for (int i = 0; i < num; i++) {
    if (eel.pimpl->elements[i] != bad) this->enable(eel.pimpl->elements[i], i);
  }
}

/*!
  Return the current setting of the global counter used to determine
  when lists are out of date.  It is incremented whenever a new
  element is added to a list.
*/
int
SoEnabledElementsList::getCounter(void)
{
  return enable_counter;
}

#undef THIS
