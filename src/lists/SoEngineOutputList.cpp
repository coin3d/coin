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
  \class SoEngineOutputList SoEngineOutputList.h Inventor/lists/SoEngineOutputList.h
  \brief The SoEngineOutputList class is a container for lists of SoEngineOutput objects.
  \ingroup lists
*/

#include <Inventor/lists/SoEngineOutputList.h>

/*!
  Default Constructor, initializes an empty list.
*/
SoEngineOutputList::SoEngineOutputList(void)
  : inherited()
{
}

/*!
  Constructor which takes a hint about the list size for effective
  allocation of resources.
*/
SoEngineOutputList::SoEngineOutputList(const int size)
  : inherited(size)
{
}

/*!
  Copy constructor.
*/
SoEngineOutputList::SoEngineOutputList(const SoEngineOutputList & l)
  : inherited(l)
{
}

/*!
  Destructor, free resources.
*/
SoEngineOutputList::~SoEngineOutputList(void)
{
}

/*!
  Add \a ptr to SoEngineOutput at the end of the list.
*/
void
SoEngineOutputList::append(SoEngineOutput * const ptr)
{
  inherited::append((SoEngineOutput *)ptr);
}

/*!
  Insert \a ptr into the list at index \a addbefore.
*/
void
SoEngineOutputList::insert(SoEngineOutput * const ptr, const int addbefore)
{
  inherited::insert((SoEngineOutput *)ptr, addbefore);
}

/*!
  Returns list element at index \a i.
*/
SoEngineOutput *
SoEngineOutputList::operator [](const int i) const
{
  return (SoEngineOutput *)((*(const SbPList *)this)[i]);
}

/*!
  Replace list element at index \a i with \a ptr.
*/
void
SoEngineOutputList::set(const int i, SoEngineOutput * const ptr)
{
  inherited::set(i, (SoEngineOutput *)ptr);
}
