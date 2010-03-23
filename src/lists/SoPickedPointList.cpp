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

#include <Inventor/lists/SoPickedPointList.h>
#include <Inventor/SoPickedPoint.h>

/*!
  \class SoPickedPointList SoPickedPointList.h Inventor/lists/SoPickedPointList.h
  \brief The SoPickedPointList class is a container for pointers to SoPickedPoint objects.
  \ingroup general

  This list class will delete the picked points when
  destructed/truncated, or when a picked point in the list is replaced
  by another picked point The caller is responsible for allocating the
  picked points passed to the list, but should not deallocate them since
  this will be handled by the list.

  \sa SbPList
*/

/*!
  \fn SoPickedPointList::SoPickedPointList(void)

  Default constructor.
*/

/*!
  \fn SoPickedPointList::SoPickedPointList(const int sizehint)

  This constructor initializes the internal allocated size for the
  list to \a sizehint. Note that the list will still initially contain
  zero items.

  \sa SbList::SbList(const int sizehint)
*/

/*!
  \fn SoPickedPoint * SoPickedPointList::operator[](const int idx) const

  Returns element at \a idx.

  Will automatically expand the size of the internal array if \a idx
  is outside the current bounds of the list. The values of any
  additional pointers are then set to \c NULL.
*/


/*!
  Copy constructor. Will copy picked points, not just pointers.

  \sa SbList::SbList(const SbList<Type> & l)
*/
SoPickedPointList::SoPickedPointList(const SoPickedPointList & l)
  : SbPList(l.getLength())
  
{
  for (int i = 0; i < l.getLength(); i++) {
    this->append(l[i]->copy());
  }
}

/*!
  Overridden to delete truncated items.
*/
void 
SoPickedPointList::truncate(const int start, const int fit) 
{
  int oldlen = this->getLength();
  
  for (int i = start; i < oldlen; i++) {
    delete (*this)[i];
  }
  SbPList::truncate(start, fit);
}

/*!
  Overridden to destruct the replaced item.
*/
void 
SoPickedPointList::set(const int idx, SoPickedPoint * pp)
{
  if (idx < this->getLength()) delete (*this)[idx];
  SbPList::operator[](idx) = (void*) pp;
}
