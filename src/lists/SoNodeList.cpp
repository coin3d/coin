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
  \class SoNodeList SoNodeList.h Inventor/lists/SoNodeList.h
  \brief The SoNodeList class is a container for pointers to SoNode objects.

  As this class inherits SoBaseList, referencing and dereferencing
  will default be done on the objects at append(), remove(), insert()
  etc.
*/

#include <Inventor/lists/SoNodeList.h>


/*!
  Default constructor.
*/
SoNodeList::SoNodeList(void)
{
}

/*!
  Constructor with a hint about the number of elements the list will
  hold.

  \sa SoBaseList::SoBaseList(const int)
*/
SoNodeList::SoNodeList(const int size)
  : SoBaseList(size)
{
}

/*!
  Copy constructor.

  \sa SoBaseList::SoBaseList(const SoBaseList &)
*/
SoNodeList::SoNodeList(const SoNodeList & nl)
  : SoBaseList(nl)
{
}

/*!
  Destructor.

  \sa SoBaseList::~SoBaseList()
*/
SoNodeList::~SoNodeList()
{
}

/*!
  Append \a ptr to the list.

  \sa SoBaseList::append()
*/
void
SoNodeList::append(SoNode * const ptr)
{
  SoBaseList::append((SoBase *)ptr);
}

/*!
  Return node pointer at index \a i.

  \sa SoBaseList::operator[]()
*/
SoNode *
SoNodeList::operator[](const int i) const
{
  return (SoNode *)SoBaseList::operator[](i);
}

/*!
  Copy contents of list \a nl to this list.

  \sa SoBaseList::operator=()
*/
SoNodeList &
SoNodeList::operator=(const SoNodeList & nl)
{
  this->copy(nl);
  return *this;
}
