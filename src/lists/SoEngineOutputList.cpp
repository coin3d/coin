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
  \class SoEngineOutputList Inventor/lists/SoEngineOutputList.h
  \brief The SoEngineOutputList class is a container class for lists of
  SoEngineOutput objects.
  \ingroup lists
*/

#include <Inventor/lists/SoEngineOutputList.h>

/*!
  A constructor (default).
*/

SoEngineOutputList::SoEngineOutputList()
  :SbPList()
{
}

/*!
  A constructor which takes a hint for the list size.
*/

SoEngineOutputList::SoEngineOutputList(const int size)
  : SbPList(size)
{
}

/*!
  Copy constructor.
*/

SoEngineOutputList::SoEngineOutputList(const SoEngineOutputList & l)
  : SbPList(l)
{
}

/*!
  The destructor.
*/

SoEngineOutputList::~SoEngineOutputList(void)
{
}

/*!
  Add \a ptr at the end of the list.
*/

void
SoEngineOutputList::append(SoEngineOutput * const ptr)
{
  SbPList::append((SoEngineOutput *)ptr);
}

/*!
  Insert \a ptr into the list at index \a addBefore.
*/

void
SoEngineOutputList::insert(SoEngineOutput * const ptr, const int addBefore)
{
  SbPList::insert((SoEngineOutput *)ptr, addBefore);
}

/*!
  Returns list element at index \a i.
*/

//$ EXPORT INLINE
SoEngineOutput *
SoEngineOutputList::operator [](const int i) const
{
  return (SoEngineOutput *)((*(const SbPList *)this)[i]);
}

/*!
  Replace list element at index \a with \a ptr.
*/
void
SoEngineOutputList::set(const int i, SoEngineOutput * const ptr)
{
  SbPList::set(i, (SoEngineOutput *)ptr);
}
