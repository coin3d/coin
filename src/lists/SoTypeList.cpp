/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#include <Inventor/lists/SoTypeList.h>

/*!
  \class SoTypeList SoTypeList.h Inventor/lists/SoTypeList.h
  \brief The SoTypeList class is a container class for arrays of SoType objects.
  \ingroup general

  \sa SbList
*/

/*!
  \fn SoTypeList::SoTypeList(void)

  Default constructor.
*/

/*!
  \fn SoTypeList::SoTypeList(const int sizehint)

  This constructor initializes the internal allocated size for the
  list to \a sizehint. Note that the list will still initially contain
  zero items.

  \sa SbList::SbList(const int sizehint)
*/

/*!
  \fn SoTypeList::SoTypeList(const SoTypeList & l)

  Copy constructor.

  \sa SbList::SbList(const SbList<Type> & l)
*/

/*!
  Overridden from parent to accept an SoType argument.
*/
void
SoTypeList::append(const SoType type)
{
  // need a temporary variable since not all compilers will let you
  // cast directly from an int16 to a void *.
  unsigned long tmp = (unsigned long) type.getKey();
  SbPList::append((void*) tmp);
}

/*!
  Overridden from parent to accept an SoType argument.
*/
int
SoTypeList::find(const SoType type) const
{
  // need a temporary variable since not all compilers will let you
  // cast directly from an int16 to a void *.
  unsigned long tmp = (unsigned long) type.getKey();
  return SbPList::find((void*) tmp);
}

/*!
  Overridden from parent to accept an SoType argument.
*/
void
SoTypeList::insert(const SoType type, const int insertbefore)
{
  // need a temporary variable since not all compilers will let you
  // cast directly from an int16 to a void *.
  unsigned long tmp = (unsigned long) type.getKey();
  SbPList::insert((void*) tmp, insertbefore);
}

/*!
  Overridden from parent to return an SoType instance..
*/
SoType
SoTypeList::operator[](const int idx) const
{
  // need a temporary variable since not all compilers will let you
  // cast directly from a pointer to an int16.
  unsigned long tmp = (unsigned long) SbPList::operator[](idx);

  return SoType::fromKey((int16_t) tmp);
}

/*!
  This method sets the element at \a index to \a item. Overridden
  to accept an SoType argument.
*/
void
SoTypeList::set(const int index, const SoType item)
{
  // need a temporary variable since not all compilers will let you
  // cast directly from an int16 to a void *.
  unsigned long tmp = (unsigned long) item.getKey();
  SbPList::set(index, (void*) tmp);
}
