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
  \class SoBaseList Inventor/lists/SoBaseList.h
  \brief The SoBaseList class is a container class for arrays of pointers to
  SoBase derived objects.

  The main purpose of the SoBaseList class is to handle ref'ing and unref'ing
  of all the items in the lists.

  FIXME: write doc.
*/

#include <Inventor/lists/SoBaseList.h>
#include <Inventor/misc/SoBase.h>
#include <assert.h>

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
  if (this->doRef) {
    const int n = this->getLength();
    for (int i = 0; i < n; i++) {
      this->get(i)->ref();
    }
  }
}

/*!
  The destructor.
*/

SoBaseList::~SoBaseList(void)
{
  truncate(0); // will handle unref
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
  assert(which >= 0 && which < this->getLength());
  if (this->doRef && this->get(which) != NULL) this->get(which)->unref();
  SbPList::remove(which);
}

/*!
  FIXME: write doc.
*/

void
SoBaseList::truncate(const int start)
{
  if (this->doRef) {
    const int n = this->getLength();
    for (int i = start; i < n; i++) {
      if (this->get(i) != NULL) this->get(i)->unref();
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
  if (this == &l) return;

  this->truncate(0);

  this->doRef = l.doRef;
  const int n = l.getLength();
  for (int i = 0; i < n; i++)
    this->append(l.get(i)); // wil handle ref'ing
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
  return (SoBase*) SbPList::operator[](i);
}

/*!
  FIXME: write doc.
*/

void
SoBaseList::addReferences(const SbBool flag)
{
  if (flag == this->doRef) return; // no change
  this->doRef = flag;

  // this method should probably never be called when there are items in
  // the list, but I think the code below should handle that case also.
  // If refing changes from on to off, all items are unref'ed, since
  // they were ref'ed when inserted. If state changes from off to on, all
  // items are ref'ed, since they will be unref'ed when removed from list.

  const int n = this->getLength();
  for (int i = 0; i < n; i++) {
    SoBase *item = this->get(i);
    if (item) {
      if (flag) item->ref();
      else item->unref();
    }
  }
}

/*!
  FIXME: write doc.
*/

void
SoBaseList::set(const int i, SoBase * const ptr)
{
  if (this->doRef) {
    ptr->ref();
    if (this->get(i)) this->get(i)->unref();
  }
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
