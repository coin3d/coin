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
  \class SoBaseList Inventor/lists/SoBaseList.h
  \brief The SoBaseList class is a container class for arrays of pointers to
  SoBase derived objects.

  The main purpose of the SoBaseList class is to handle ref'ing and unref'ing
  of all the items in the lists.

  FIXME: write doc.
*/

#include <Inventor/lists/SoBaseList.h>

#include <Inventor/misc/SoBase.h>

/*!
  A constructor.
*/

SoBaseList::SoBaseList(void)
  :SbPList()
{
  this->doRef = TRUE;
}

/*!
  A constructor.
*/

SoBaseList::SoBaseList(const int size)
  : SbPList(size)
{
  this->doRef = TRUE;
}

/*!
  The copy constructor.
*/

SoBaseList::SoBaseList(const SoBaseList &l)
  : SbPList(l)
{
  this->doRef = l.doRef;
//   const int n = l.getLength();
//   for (int i = 0; i < n; i++) {
//     this->append(l.get(i));
//   }
}

/*!
  The destructor.
*/

SoBaseList::~SoBaseList(void)
{
  truncate(0);
}

/*!
  FIXME: write doc.
*/

void 
SoBaseList::append(SoBase * const ptr)
{
  if (this->doRef) ptr->ref();
  SbPList::append((void*)ptr);
}

/*!
  FIXME: write doc.
*/

void 
SoBaseList::insert(SoBase * const ptr, const int addBefore)
{
  if (this->doRef) ptr->ref();
  SbPList::insert((void*)ptr, addBefore);
}

/*!
  FIXME: write doc.
*/

void 
SoBaseList::remove(const int which)
{
  const int n = this->getLength();
  //Check if get(which) exists.
  if (this->doRef) this->get(which)->unref();
  SbPList::remove(which);
}

/*!
  FIXME: write doc.
*/

void 
SoBaseList::truncate(const int start)
{
  const int n = this->getLength();
  //Check if get(which) exists.
  if (this->doRef) {
    for (int i = start; i < n; i++) {
      this->get(i)->unref();
    }
  }
  SbPList::truncate(start);
}

/*!
  FIXME: write doc.
*/

void 
SoBaseList::copy(const SoBaseList &l)
{
  SbPList::copy(l);
  this->doRef = l.doRef;

//   truncate(0);
//   this->doRef = l.doRef;
//   const int n = l.getLength();
//   for (int i = 0; i < n; i++) 
//     this->append(l.get(i));
}

/*!
  FIXME: write doc.
*/

SoBaseList &
SoBaseList::operator =(const SoBaseList &l)
{
  this->copy(l);
  return *this;
}

/*!
  FIXME: write doc.
*/

//$ EXPORT INLINE
SoBase *
SoBaseList::operator [](const int i) const
{
  return (SoBase *)((*(const SbPList *)this)[i]);
}

/*!
  FIXME: write doc.
*/

void 
SoBaseList::addReferences(const SbBool flag)
{
  this->doRef = flag;
}

/*!
  FIXME: write doc.
*/

void 
SoBaseList::set(const int i, SoBase * const ptr)
{
  if (this->doRef) this->get(i)->unref();
  SbPList::set(i, (void*)ptr);
}

/*!
  FIXME: write doc.
*/

//$ EXPORT INLINE
SoBase *
SoBaseList::get(const int i) const
{
  return (SoBase*)SbPList::get(i);
}

