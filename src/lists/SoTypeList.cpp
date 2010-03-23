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
  const uintptr_t tmp = (uintptr_t)type.getKey();
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
  const uintptr_t tmp = (uintptr_t)type.getKey();
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
  const uintptr_t tmp = (uintptr_t)type.getKey();
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
  const uintptr_t tmp = (uintptr_t) SbPList::operator[](idx);

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
  const uintptr_t tmp = (uintptr_t) item.getKey();
  SbPList::set(index, (void*) tmp);
}
