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
  \class SoNodeList Inventor/lists/SoNodeList.h
  \brief The SoNodeList class is a container class for arrays of pointers to
  SoNode-derived objects.

  FIXME: write doc.
*/

#include <Inventor/lists/SoNodeList.h>

/*!
  A constructor (default).
*/

SoNodeList::SoNodeList(void)
{
}

/*!
  A constructor.
*/

SoNodeList::SoNodeList(const int size)
  : SoBaseList(size)
{
}

/*!
  A constructor (copy).
*/

SoNodeList::SoNodeList(const SoNodeList & l)
  : SoBaseList(l)
{
}

/*!
  The destructor.
*/

SoNodeList::~SoNodeList(void)
{
}

/*!
  FIXME: write doc.
*/

void
SoNodeList::append(SoNode * const ptr)
{
  SoBaseList::append((SoBase *)ptr);
}

/*!
  FIXME: write doc.
*/

//$ EXPORT INLINE
SoNode *
SoNodeList::operator [](const int i) const
{
  return (SoNode *)SoBaseList::operator[](i);
}

/*!
  FIXME: write doc.
*/

SoNodeList &
SoNodeList::operator =(const SoNodeList & l)
{
  this->copy(l);
  return *this;
}

/*!
  FIXME: write doc.
*/

void
SoNodeList::insert(SoNode * const ptr, const int addBefore)
{
  SoBaseList::insert((SoBase *)ptr, addBefore);
}
