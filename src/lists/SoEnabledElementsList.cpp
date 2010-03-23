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

#ifdef COIN_THREADSAFE
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

#define PRIVATE(obj) ((obj)->pimpl)

/*!
  Constructor.
*/
SoEnabledElementsList::SoEnabledElementsList(SoEnabledElementsList * const parentlist)
{
  PRIVATE(this) = new SoEnabledElementsListP;

  PRIVATE(this)->prevmerge = 0;
  PRIVATE(this)->parent = parentlist;
}

/*!
  Destructor.
*/
SoEnabledElementsList::~SoEnabledElementsList()
{
  delete PRIVATE(this);
}

/*!
  Return the list of enabled elements.
*/
const SoTypeList &
SoEnabledElementsList::getElements(void) const
{
  PRIVATE(this)->lock();
  // check if we need a new merge
  if (PRIVATE(this)->prevmerge != enable_counter) {
    int storedcounter = enable_counter;
    SoEnabledElementsList * plist = (SoEnabledElementsList*) PRIVATE(this)->parent;
    while (plist) {
      ((SoEnabledElementsList*)this)->merge(*plist);
      plist = plist->pimpl->parent;
    }
    // use and restore old counter since it might change during merge
    ((SoEnabledElementsList*)this)->pimpl->prevmerge =
      enable_counter = storedcounter;
  }
  PRIVATE(this)->unlock();
  return PRIVATE(this)->elements;
}

/*!
  Add an \a elementtype to the list of enabled elements if it's not
  enabled already.
*/
void
SoEnabledElementsList::enable(const SoType elementtype, const int stackindex)
{
  while (stackindex >= PRIVATE(this)->elements.getLength())
    PRIVATE(this)->elements.append(SoType::badType());

  SoType currtype = PRIVATE(this)->elements[stackindex];
  if (currtype.isBad() ||
      (elementtype != currtype && elementtype.isDerivedFrom(currtype))) {
    PRIVATE(this)->elements.set(stackindex, elementtype);
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

#undef PRIVATE
