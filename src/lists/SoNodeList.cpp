/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
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
