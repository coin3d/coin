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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoVRMLInterpOutputList Inventor/lists/SoVRMLInterpOutputList.h
  \brief The SoVRMLInterpOutputList class is a container class for lists of
  SoVRMLInterpOutput objects.
  \ingroup lists
*/

#include <Inventor/lists/SoVRMLInterpOutputList.h>

/*!
  A constructor (default).
*/

SoVRMLInterpOutputList::SoVRMLInterpOutputList()
  :SbPList()
{
}

/*!
  A constructor which takes a hint for the list size.
*/

SoVRMLInterpOutputList::SoVRMLInterpOutputList(const int size)
  : SbPList(size)
{
}

/*!
  Copy constructor.
*/

SoVRMLInterpOutputList::SoVRMLInterpOutputList(const SoVRMLInterpOutputList & l)
  : SbPList(l)
{
}

/*!
  The destructor.
*/

SoVRMLInterpOutputList::~SoVRMLInterpOutputList(void)
{
}

/*!
  Add \a ptr at the end of the list.
*/

void 
SoVRMLInterpOutputList::append(SoVRMLInterpOutput * const ptr)
{
  SbPList::append((SoVRMLInterpOutput *)ptr);
}

/*!
  Insert \a ptr into the list at index \a addBefore.
*/

void 
SoVRMLInterpOutputList::insert(SoVRMLInterpOutput * const ptr, const int addBefore)
{
  SbPList::insert((SoVRMLInterpOutput *)ptr, addBefore);
}

/*!
  Returns list element at index \a i.
*/

//$ EXPORT INLINE
SoVRMLInterpOutput *
SoVRMLInterpOutputList::operator [](const int i) const
{
  return (SoVRMLInterpOutput *)((*(const SbPList *)this)[i]);
}

/*!
  Replace list element at index \a with \a ptr.
*/
void 
SoVRMLInterpOutputList::set(const int i, SoVRMLInterpOutput * const ptr)
{
  SbPList::set(i, (SoVRMLInterpOutput *)ptr);
}
