/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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

/*!
  \class SoEnabledElementsList SoEnabledElementsList.h Inventor/lists/SoEnabledElementsList.h
  \brief The SoEnabledElementsList class is a container for type info for element types that are enabled in actions.
  \ingroup actions

  This class is probably not interesting for the application
  programmer.
*/

// Metadon doc
/*¡
  FIXME: doesn't handle post-initialization changes very well (fixing
  this also needs some tempering to be done with SoAction, I think.)
  20000305 mortene.
 */

#include <Inventor/lists/SoEnabledElementsList.h>
#include <assert.h>


int SoEnabledElementsList::counter = 0;


/*!
  Constructor.
*/
SoEnabledElementsList::SoEnabledElementsList(SoEnabledElementsList * const parentlist)
  : setupcounter(counter), parent(parentlist)
{
}

/*!
  Return the list of enabled elements.
*/
const SoTypeList &
SoEnabledElementsList::getElements(void) const
{
  if (!parent) // already merged. FIXME: can't do it like this
    return elements;

  // FIXME: use counter and setupcounter to avoid unncessary merges
  // now we just set this->parent to NULL, but if parent changes,
  // this will not be correct.
  SoEnabledElementsList * const eel = (SoEnabledElementsList *)this;
  eel->merge(*this->parent);
  eel->parent = NULL;
  return elements;
}

/*!
  Add an \a elementtype to the list of enabled elements if it's not
  enabled already.
*/
void
SoEnabledElementsList::enable(const SoType elementtype, const int stackindex)
{
#if 0 // debug
  SoDebugError::postInfo("SoEnabledElementsList::enable",
                         "this: %p, elements: %p, typelist length: %d\n",
                         this, &(this->elements), elements.getLength());
#endif // debug

  while (stackindex >= this->elements.getLength())
    this->elements.append(SoType::badType());

  this->elements[stackindex] = elementtype;
}

/*!
  Enables all the elements from the \a eel list that is enabled in
  this instance.
*/
void
SoEnabledElementsList::merge(const SoEnabledElementsList & eel)
{
  SoType bad = SoType::badType();
  const int num = eel.elements.getLength();
  for (int i = 0; i < num; i++) {
    if (eel.elements[i] != bad) this->enable(eel.elements[i], i);
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
  return SoEnabledElementsList::counter;
}
