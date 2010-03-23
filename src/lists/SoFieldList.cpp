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

#include <Inventor/lists/SoFieldList.h>

/*!
  \class SoFieldList SoFieldList.h Inventor/lists/SoFieldList.h
  \brief The SoFieldList class is a container for pointers to SoField objects.
  \ingroup fields

  \sa SbList
*/

/*!
  \fn SoFieldList::SoFieldList(void)

  Default constructor.
*/

/*!
  \fn SoFieldList::SoFieldList(const int sizehint)

  This constructor initializes the internal allocated size for the
  list to \a sizehint. Note that the list will still initially contain
  zero items.

  \sa SbList::SbList(const int)
*/

/*!
  \fn SoFieldList::SoFieldList(const SoFieldList & l)

  Copy constructor.

  \sa SbList::SbList(const SbList<Type> & l)
*/

/*!
  \fn void SoFieldList::set(const int index, SoField * item)

  Overridden from parent to accept an SoField pointer argument.
*/

/*!
  \fn void SoFieldList::append(SoField * field)

  Overridden from parent to accept an SoField pointer argument.
*/

/*!
  \fn void SoFieldList::insert(SoField * field, const int insertbefore)

  Overridden from parent to accept an SoField pointer argument.
*/
  
/*!
  \fn SoField * SoFieldList::operator [](const int idx) const

  Overridden from parent to return an SoField pointer.
*/

/*!
  \fn SoField * SoFieldList::get(const int idx) const

  Overridden from parent to return an SoField pointer.
*/
