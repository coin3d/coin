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
  \class SoPickedPointList Inventor/lists/SoPickedPointList.h
  \brief The SoPickedPointList class is a container class for arrays of
  pointers to SoPickedPoint objects.

  FIXME: write doc.
*/

#include <Inventor/lists/SoPickedPointList.h>

/*!
  A constructor (default).
*/

SoPickedPointList::SoPickedPointList(void)
  :SbPList()
{
}

/*!
  A constructor.
*/

SoPickedPointList::SoPickedPointList(const int size)
  : SbPList(size)
{
}

/*!
  A constructor (copy).
*/

SoPickedPointList::SoPickedPointList(const SoPickedPointList &l)
  : SbPList(l)
{
}

/*!
  The destructor.
*/

SoPickedPointList::~SoPickedPointList()
{
}

/*!
  FIXME: write doc.
*/

void
SoPickedPointList::append(SoPickedPoint * const ptr)
{
  SbPList::append((void*)ptr);
}

/*!
  FIXME: write doc.
*/

void
SoPickedPointList::insert(SoPickedPoint * const ptr, const int addBefore)
{
  SbPList::insert((void*)ptr, addBefore);
}

/*!
  FIXME: write doc.
*/

void
SoPickedPointList::truncate(int start)
{
  SbPList::truncate(start);
}

/*!
  FIXME: write doc.
*/

SoPickedPoint *
SoPickedPointList::operator [](const int i) const
{
  return (SoPickedPoint *)((*(const SbPList *)this)[i]);
}

/*!
  FIXME: write doc.
*/

void
SoPickedPointList::set(const int i, SoPickedPoint * const ptr)
{
  SbPList::set(i, (void*)ptr);
}
