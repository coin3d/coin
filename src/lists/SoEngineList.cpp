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
  \class SoEngineList Inventor/lists/SoEngineList.h
  \brief The SoEngineList class is a container class for arrays of
  SoEngine-derived objects.

  FIXME: write doc.
*/

#include <Inventor/lists/SoEngineList.h>

/*!
  A constructor (default).
*/

SoEngineList::SoEngineList(void)
{
}

/*!
  A constructor.
*/

SoEngineList::SoEngineList(const int size)
  : SoBaseList(size)
{
}

/*!
  A constructor (copy).
*/

SoEngineList::SoEngineList(const SoEngineList &l)
  : SoBaseList(l)
{
}

/*!
  The destructor.
*/

SoEngineList::~SoEngineList(void)
{
}

/*!
  FIXME: write doc.
*/

void 
SoEngineList::append(SoEngine * const ptr)
{
  SoBaseList::append((SoBase *)ptr);
}

/*!
  FIXME: write doc.
*/

//$ EXPORT INLINE
SoEngine *
SoEngineList::operator [](const int i) const
{
  return (SoEngine *)SoBaseList::operator[](i);
}

/*!
  FIXME: write doc.
*/

SoEngineList &
SoEngineList::operator =(const SoEngineList &l)
{
  this->copy(l);
  return *this;
}

