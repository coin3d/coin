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
  \class SoDetailList Inventor/lists/SoDetailList.h
  \brief The SoDetailList class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/lists/SoDetailList.h>

#include <Inventor/misc/SoBase.h>

/*!
  A constructor (default).
*/

SoDetailList::SoDetailList(void)
  :SbPList()
{
}

/*!
  A constructor.
*/

SoDetailList::SoDetailList(const int size)
  : SbPList(size)
{
}

/*!
  A constructor.
*/

SoDetailList::SoDetailList(const SoDetailList &l)
  : SbPList(l)
{
}

/*!
  The destructor.
*/

SoDetailList::~SoDetailList(void)
{
}

/*!
  FIXME: write doc.
*/

void
SoDetailList::append(SoDetail * const ptr)
{
  SbPList::append((void*)ptr);
}

/*!
  FIXME: write doc.
*/

void
SoDetailList::insert(SoDetail * const ptr, const int addBefore)
{
  SbPList::insert((void*)ptr, addBefore);
}

/*!
  FIXME: write doc.
*/

void
SoDetailList::truncate(const int start)
{
  SbPList::truncate(start);
}

/*!
  FIXME: write doc.
*/

void
SoDetailList::copy(const SoDetailList &l)
{
  SbPList::copy(l);
}

/*!
  FIXME: write doc.
*/

SoDetailList &
SoDetailList::operator =(const SoDetailList &l)
{
  this->copy(l);
  return *this;
}

/*!
  FIXME: write doc.
*/

//$ EXPORT INLINE
SoDetail *
SoDetailList::operator [](const int i) const
{
  return (SoDetail *)((*(const SbPList *)this)[i]);
}

/*!
  FIXME: write doc.
*/

void
SoDetailList::set(const int i, SoDetail * const ptr)
{
  SbPList::set(i, (void*)ptr);
}
