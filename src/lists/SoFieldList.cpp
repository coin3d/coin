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
  \class SoFieldList Inventor/lists/SoFieldList.h
  \brief The SoFieldList class is a container class for lists of pointers
  to SoField-derived objects.

  FIXME: write doc.
*/

#include <Inventor/lists/SoFieldList.h>

/*!
  A constructor (default).
*/

SoFieldList::SoFieldList(void)
  :SbPList()
{
}

/*!
  A constructor.
*/

SoFieldList::SoFieldList(const int size)
  : SbPList(size)
{
}

/*!
  Copy constructor.
*/
SoFieldList::SoFieldList(const SoFieldList & source)
  : SbPList()
{
  this->operator=(source);
}

/*!
  The destructor.
*/

SoFieldList::~SoFieldList(void)
{
}

/*!
  FIXME: write doc.
*/

void 
SoFieldList::append(SoField * const ptr)
{
  SbPList::append((void *)ptr);
}

/*!
  FIXME: write doc.
*/

void 
SoFieldList::insert(SoField * const ptr, const int addBefore)
{
  SbPList::insert((void *)ptr, addBefore);
}

/*!
  FIXME: write doc.
*/

SoField *
SoFieldList::operator [](const int i) const
{
  return (SoField *)((*(const SbPList *)this)[i]);
}

/*!
  FIXME: write doc.
*/

void
SoFieldList::set(const int i, SoField * const ptr)
{
  SbPList::set(i, (void *)ptr);
}

/*!
  FIXME: write doc.
*/

SoField *
SoFieldList::get(const int i) const
{
  return (SoField *)SbPList::get(i);
}

/*!
  Make a shallow copy of the contents of \a source into this list.
  Returns a reference to \a this.
 */
SoFieldList &
SoFieldList::operator=(const SoFieldList & source)
{
  inherited::copy(source);
  return *this;
}
