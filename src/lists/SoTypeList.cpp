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
  SbPList::append((void*) type.getKey());
}

/*!
  Overridden from parent to accept an SoType argument.
*/
int
SoTypeList::find(const SoType type) const
{
  return SbPList::find((void*) type.getKey());
}

/*!
  Overridden from parent to accept an SoType argument.
*/
void
SoTypeList::insert(const SoType type, const int insertbefore)
{
  SbPList::insert((void*) type.getKey(), insertbefore);
}

/*!
  Overridden from parent to return an SoType instance..
*/
SoType
SoTypeList::operator[](const int idx) const
{
  return SoType::fromKey((int16_t) SbPList::operator[](idx));
}

/*!
  This method sets the element at \a index to \a item. Overridden
  to accept an SoType argument.
*/
void
SoTypeList::set(const int index, const SoType item)
{
  SbPList::set(index, (void*) item.getKey());
}
