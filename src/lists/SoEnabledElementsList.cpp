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
  \class SoEnabledElementsList Inventor/lists/SoEnabledElementsList.h
  \brief The SoEnabledElementsList class is a container class for arrays of
  SoType objects for element types that are enabled in actions.

  FIXME: write doc.
*/

#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/SoType.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <assert.h>

int SoEnabledElementsList::counter;

/*!
  A constructor.
*/

SoEnabledElementsList::SoEnabledElementsList(SoEnabledElementsList * const parentList)
  : setUpCounter(counter), parent(parentList)
{
}

#include <Inventor/SbName.h>

/*!
  This method returns the list of enabled elements.
*/

const SoTypeList &
SoEnabledElementsList::getElements(void) const
{
  if (! parent) // already merged. FIXME: can't do it like this
    return elements;

  // FIXME: use counter and setUpCounter to avoid unncessary merges
  // now we just set this->parent to NULL, but if parent changes,
  // this will not be correct.
  SoEnabledElementsList * const list = (SoEnabledElementsList *)this;
  list->merge(*this->parent);
  list->parent = NULL;
  return elements;
}

/*!
  This method adds an element to the list of enabled elements if it's not
  enabled already.
*/

void
SoEnabledElementsList::enable(const SoType elementType,
                              const int stackIndex)
{
#if 0 // debug
  SoDebugError::postInfo("SoEnabledElementsList::enable",
                         "this: %p, elements: %p, typelist length: %d\n",
                         this, &(this->elements), elements.getLength());
#endif // debug

  while (stackIndex >= this->elements.getLength())
    this->elements.append(SoType::badType());

  this->elements[stackIndex] = elementType;
}

/*!
  This method enables all the elements in \a list that is enabled in this
  instance.
*/

void
SoEnabledElementsList::merge(const SoEnabledElementsList & list)
{
  SoType bad = SoType::badType();
  const int num = list.elements.getLength();
  for (int i = 0; i < num; i++) {
    if (list.elements.get(i) != bad) this->enable(list.elements.get(i), i);
  }
}

/*!
  This method returns the current setting of the global counter used to
  determine when lists are out of date.  It is incremented whenever a new
  element is added to a list.
*/

int
SoEnabledElementsList::getCounter()
{
  return counter;
}
