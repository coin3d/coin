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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoNodeList SoNodeList.h Inventor/lists/SoNodeList.h
  \brief The SoNodeList class is a container for pointers to SoNode objects.
  \ingroup nodes

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
