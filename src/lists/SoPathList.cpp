/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoPathList Inventor/lists/SoPathList.h
  \brief The SoPathList class is a container class for arrays of pointers to
  SoPath objects.

  FIXME: write doc.
*/

#include <Inventor/lists/SoPathList.h>

#include <Inventor/SoPath.h>

/*!
  A constructor (default).
*/

SoPathList::SoPathList(void)
  : SoBaseList()
{
}

/*!
  A constructor.
*/

SoPathList::SoPathList(const int size)
  : SoBaseList(size)
{
}

/*!
  A constructor (copy).
*/

SoPathList::SoPathList(const SoPathList & l)
  : SoBaseList(l)
{
}

/*!
  The destructor.
*/

SoPathList::~SoPathList(void)
{
}

/*!
  FIXME: write doc.
*/

//$ EXPORT INLINE
void 
SoPathList::append(SoPath * const path)
{
  SoBaseList::append((SoBase *)path);
}

/*!
  FIXME: write doc.
*/

//$ EXPORT INLINE
SoPath *
SoPathList::operator [](const int i) const
{
  return (SoPath *)SoBaseList::operator[](i);
}

/*!
  FIXME: write doc.
*/

SoPathList &
SoPathList::operator =(const SoPathList & l)
{
  SoBaseList::copy(l);
  return *this;
}

/*!
  FIXME: write doc.
*/

int 
SoPathList::findPath(const SoPath & path)
{
  return this->find((void *)&path);
}

/*!
  FIXME: write doc.
*/

void 
SoPathList::sort()
{
  //Sort on increasing addres of path->getHead()
  //then on increasing indices of children
}

/*!
  FIXME: write doc.
*/

void 
SoPathList::uniquify()
{
}

/*!
  FIXME: write doc.
*/

//
// inline only for this class
//
inline SoPath *
SoPathList::get(const int index) const
{
  return (SoPath *)SbPList::get(index);
}

/*!
  FIXME: write doc.
*/

inline void 
SoPathList::set(const int index, SoPath * const path)
{
  SbPList::set(index, (void *)path);
}

