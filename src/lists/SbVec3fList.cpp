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

#include <Inventor/lists/SbVec3fList.h>

/*!
  \class SbVec3fList SbVec3fList.h Inventor/lists/SbVec3fList.h
  \brief The SbVec3fList class is a container for arrays of SbVec3f pointers.
  \ingroup base

  Note that upon using the equality and inequality operators, the
  SbVec3f objects themselves are not compared, only the pointer
  values.

  Note also that all calls to append() and insert() will cause the
  list to allocate a new SbVec3f object. These objects are freed
  when the list is destructed.

  \sa SbList
*/



/*!
  \fn SbVec3fList::SbVec3fList(void)

  Default constructor.
*/

/*!
  Destructor.
*/

SbVec3fList::~SbVec3fList()
{
  for (int i = 0; i < this->getLength(); i++) {
    delete (*this)[i];
  }
}

/*!
  \fn void SbVec3fList::append(const SbVec3f * item)

  Overridden from parent to allocate a new SbVec3f instance when
  called.

  \sa SbPList::append()
 */

/*!
  \fn void SbVec3fList::insert(const SbVec3f * item, const int insertbefore)

  Overridden from parent to allocate a new SbVec3f instance when
  called.

  \sa SbPList::insert()
 */

/*!
  \fn SbVec3f * SbVec3fList::operator[](const int idx) const

  Overridden from parent to return an SbVec3f.
*/
