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

#include <Inventor/lists/SoDetailList.h>
#include <Inventor/details/SoDetail.h>

/*!
  \class SoDetailList SoDetailList.h Inventor/lists/SoDetailList.h
  \brief The SoDetailList class is a container for pointers to SoDetail objects.
  \ingroup details

  This list class will delete the details when destructed/truncated,
  or when a detail in the list is replaced by another detail. The
  caller is responsible for allocating the details passed to the list,
  but should not deallocate them since this will be handled by the
  list.
  
  \sa SbPList
*/

/*!
  \fn SoDetailList::SoDetailList(void)

  Default constructor.
*/

/*!
  \fn SoDetailList::SoDetailList(const int sizehint)

  This constructor initializes the internal allocated size for the
  list to \a sizehint. Note that the list will still initially contain
  zero items.

  \sa SbList::SbList(const int sizehint)
*/

/*!
  \fn SoDetail * SoDetailList::operator[](const int idx) const

  Returns element at \a idx.

  Will automatically expand the size of the internal array if \a idx
  is outside the current bounds of the list. The values of any
  additional pointers are then set to \c NULL.
*/


/*!
  Copy constructor.
*/
SoDetailList::SoDetailList(const SoDetailList & l)
  : SbPList(l.getLength())
{
  this->copy(l);
}

/*!
  Destructor.
*/
SoDetailList::~SoDetailList()
{
  this->truncate(0);
}

/*!
  Overridden to delete truncated items.
*/ 
void 
SoDetailList::truncate(const int length, const int fit) 
{
  int oldlen = this->getLength();
  
  for (int i = length; i < oldlen; i++) {
    delete (*this)[i];
  }
  SbPList::truncate(length, fit);
}

/*!
  Overridden to copy items, not just pointers.
*/
void 
SoDetailList::copy(const SoDetailList & l)
{
  this->truncate(0);
  for (int i = 0; i < l.getLength(); i++) {
    this->append(l[i]->copy());
  }
}

/*!
  Overridden to delete old item.
*/
void
SoDetailList::set(const int idx, SoDetail * detail) 
{
  if (idx < this->getLength()) delete (*this)[idx];
  SbPList::operator[](idx) = (void*) detail;
}
