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
  \class SbIntList SbIntList.h Inventor/lists/SbIntList.h
  \brief The SbIntList class is a container for integer list arrays.
  \ingroup base

  \sa SbList
*/


/*!
  \fn SbIntList::SbIntList(void)

  Default constructor.
*/

/*!
  \fn SbIntList::SbIntList(const int sizehint)

  This constructor initializes the internal allocated size for the
  list to \a sizehint. Note that the list will still initially contain
  zero items.

  \sa SbList::SbList(const int sizehint)
*/

/*!
  \fn void SbIntList::append(const int item)

  Overridden from parent to accept an integer argument.
*/

/*!
  \fn int SbIntList::find(const int item)

  Overridden from parent to accept an integer argument.
*/

/*!
  \fn void SbIntList::insert(const int item, const int addbefore)

  Overridden from parent to accept an integer argument.
*/

/*!
  \fn int & SbIntList::operator[](const int idx) const

  Overridden from parent to return an integer.
*/
