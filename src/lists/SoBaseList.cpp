/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoBaseList SoBaseList.h Inventor/lists/SoBaseList.h
  \brief The SoBaseList class is a container for pointers to SoBase derived objects.
  \ingroup general

  The additional capability of the SoBaseList class over its parent
  class, SbPList, is to automatically handle referencing and
  dereferencing of items as they are added or removed from the lists.
*/

#include <Inventor/lists/SoBaseList.h>
#include <Inventor/misc/SoBase.h>
#include <assert.h>

// Convenience macro for casting array element from void * to SoBase *.
#define GET_BASEPTR(idx) ((SoBase *)SbPList::get(idx))


/*!
  Default constructor.
*/
SoBaseList::SoBaseList(void)
  : SbPList(), referencing(TRUE)
{
}

/*!
  Constructor with a hint about the maximum number of pointers in the
  list.

  \sa SbPList::SbPList(const int)
*/
SoBaseList::SoBaseList(const int size)
  : SbPList(size), referencing(TRUE)
{
}

/*!
  Copy constructor. Copy the items of \a l into this list, adding to
  the item reference counts if the reference flag of \a l is \c TRUE.

  \sa SbPList::SbPList(const SbPList &)
*/
SoBaseList::SoBaseList(const SoBaseList & l)
  : SbPList(l) // copies list items "raw"
{
  this->referencing = l.referencing;
  if (this->referencing) {
    const int n = this->getLength();
    for (int i = 0; i < n; i++) GET_BASEPTR(i)->ref();
  }
}

/*!
  Destructor. Dereferences items before freeing resources.
*/
SoBaseList::~SoBaseList()
{
  this->truncate(0); // truncate() does unref-ing
}

/*!
  Append \a ptr to list, adding to the reference count of the object
  (unless addReferences() has been set to \c FALSE).

  \sa SbPList::append()
*/
void
SoBaseList::append(SoBase * ptr)
{
  if (this->referencing) ptr->ref();
  SbPList::append((void *)ptr);
}

/*!
  Insert \a ptr in the list at position \a addbefore, adding to the
  reference count of the object (unless addReferences() has been set
  to \c FALSE).

  \sa SbPList::insert()
*/
void
SoBaseList::insert(SoBase * ptr, const int addbefore)
{
  if (this->referencing) ptr->ref();
  SbPList::insert((void *)ptr, addbefore);
}

/*!
  Removes item at \a index from the list, dereferencing the object
  (unless addReferences() has been set to \c FALSE).

  \sa SbPList::remove()
*/
void
SoBaseList::remove(const int index)
{
  assert(index >= 0 && index < this->getLength());
  if (this->referencing && GET_BASEPTR(index) != NULL)
    GET_BASEPTR(index)->unref();
  SbPList::remove(index);
}

/*!
  Makes the list contain only the \a length first items, removing all
  items from index \a length and onwards to the end of the
  list. Dereferences the objects to be removed (unless addReferences()
  has been set to \c FALSE).

  \sa SbPList::truncate()
*/
void
SoBaseList::truncate(const int length)
{
  if (this->referencing) {
    const int n = this->getLength();
    for (int i = length; i < n; i++)
      if (GET_BASEPTR(i) != NULL) GET_BASEPTR(i)->unref();
  }
  SbPList::truncate(length);
}

/*!
  Copy the item pointers of \a l list into this one, first removing
  all items in this list.
*/
void
SoBaseList::copy(const SoBaseList & l)
{
  if (this == &l) return;

  this->truncate(0);

  this->referencing = l.referencing;
  const int n = l.getLength();
  for (int i = 0; i < n; i++) this->append(l[i]); // handles ref'ing
}

/*!
  Copy \a l list into this one, returning a pointer to ourself.

  \sa copy()
*/
SoBaseList &
SoBaseList::operator=(const SoBaseList & l)
{
  this->copy(l);
  return *this;
}

// Documented in superclass. Overridden from parent class to cast from
// the \c void pointer actually stored.
SoBase *
SoBaseList::operator[](const int i) const
{
  return (SoBase *)SbPList::operator[](i);
}

/*!
  Decide whether or not the SoBase items should be automatically
  referenced and dereferenced as they are added and removed from the
  list.

  Default setting is to do referencing.
*/
void
SoBaseList::addReferences(const SbBool flag)
{
#if 0 // OBSOLETED: don't do this, it looks like it could give the
      // user some nasty surprises. 20000228 mortene.

  // this method should probably never be called when there are items in
  // the list, but I think the code below should handle that case also.
  // If refing changes from on to off, all items are unref'ed, since
  // they were ref'ed when inserted. If state changes from off to on, all
  // items are ref'ed, since they will be unref'ed when removed from list.

  if (flag == this->referencing) return; // no change

  const int n = this->getLength();
  for (int i = 0; i < n; i++) {
    SoBase * item = GET_BASEPTR(i);
    if (item) {
      if (flag) item->ref();
      else item->unref();
    }
  }

#else

  assert(this->getLength() == 0);

#endif

  this->referencing = flag;
}

/*!
  Index operator to set element at \a i. Does \e not expand array
  bounds if \a i is outside the list.
*/
void
SoBaseList::set(const int i, SoBase * const ptr)
{
  // Overridden from parent class to provide referencing on the SoBase
  // object.

  if (this->referencing) {
    ptr->ref();
    if (GET_BASEPTR(i)) GET_BASEPTR(i)->unref();
  }
  SbPList::set(i, (void *)ptr);
}


#undef GET_BASEPTR
