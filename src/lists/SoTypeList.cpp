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
  \class SoTypeList Inventor/lists/SoTypeList.h
  \brief The SoTypeList class is a container class for arrays of SoType
  objects.

  FIXME: write doc.
*/

#include <Inventor/lists/SoTypeList.h>

#include <Inventor/SoType.h>

/*!
  A constructor (default).
*/

SoTypeList::SoTypeList(void)
  :SbPList()
{
}

/*!
  A constructor.
*/

SoTypeList::SoTypeList(const int size)
  : SbPList(size)
{
}

/*!
  A constructor (copy).
*/

SoTypeList::SoTypeList(const SoTypeList &l)
  : SbPList(l)
{
}

/*!
  The destructor.
*/

SoTypeList::~SoTypeList(void)
{
}

/*!
  FIXME: write doc.
*/

void 
SoTypeList::append(SoType type)
{
  SbPList::append(*((void**)&type));
}

/*!
  FIXME: write doc.
*/

void 
SoTypeList::insert(SoType type, const int addBefore)
{
  SbPList::insert(*((void**)&type), addBefore);
}

/*!
  FIXME: write doc.
*/

SoType &
SoTypeList::operator [](const int i) const
{
#if 1 // testing new code, pederb, 19980922
  void *&v = SbPList::operator[](i);
  SoType *p = (SoType*)&v;
  return *p;
#else // old (buggy?) code
  SoType *p;
  void *v;
  v=((*(const SbPList *)this)[i]);
  p=(SoType *)&v;
  return *p;
#endif
}

/*!
  FIXME: write doc.
*/

void 
SoTypeList::set(const int i, SoType type)
{
  SbPList::set(i, *((void**)&type));
}

/*!
  FIXME: write doc.
*/

int
SoTypeList::find(SoType typeId) const
{
  assert(0 && "FIXME: not implemented yet");
  return 0;
}

/*!
  FIXME: write doc.
*/

void 
SoTypeList::setDefault(SoType type)
{
  SbPList::setDefault(*((void **)&type));
}

/*!
  FIXME: write doc.
*/

SoType
SoTypeList::get(const int i) const
{
  SoType *p;
  void *v;
  v=((const SbPList *)this)->get(i);
  p=(SoType *)&v;
  return *p;
}

