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
  \class SoAuditorList SoAuditorList.h Inventor/lists/SoAuditorList.h
  \brief The SoAuditorList class is used to keep track of auditors for certain object classes.
  \ingroup general

  This class is mainly for internal use (from SoBase) and it should
  not be necessary to be familiar with it for "ordinary" Coin use.
*/


#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoFieldContainer.h>
#include <Inventor/sensors/SoDataSensor.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



/*!
  Default constructor.
*/
SoAuditorList::SoAuditorList(void)
  : SbPList(8)
{
}

/*!
  Destructor.
*/
SoAuditorList::~SoAuditorList()
{
}

/*!
  Append an \a auditor of \a type to the list.
*/
void
SoAuditorList::append(void * const auditor, const SoNotRec::Type type)
{
  SbPList::append(auditor);
  SbPList::append((void *)type);
}

/*!
  Set \a auditor pointer and auditor \a type in list at \a index.
*/
void
SoAuditorList::set(const int index,
                   void * const auditor, const SoNotRec::Type type)
{
  assert(index >= 0 && index < this->getLength());

  SbPList::set(index * 2, auditor);
  SbPList::set(index * 2 + 1, (void *)type);
}

/*!
  Returns number of elements in list.
*/
int
SoAuditorList::getLength(void) const
{
  return SbPList::getLength() / 2;
}

/*!
  Find \a auditor of \a type in list and return index. Returns -1 if
  \a auditor is not in the list.
*/
int
SoAuditorList::find(void * const auditor, const SoNotRec::Type type) const
{
  const int num = this->getLength();
  for (int i = 0; i < num; i++) {
    if (this->getObject(i) == auditor && this->getType(i) == type)
      return i;
  }
  return -1;
}

/*!
  Returns auditor pointer at \a index.
*/
void *
SoAuditorList::getObject(const int index) const
{
  return SbPList::operator[](index * 2);
}

/*!
  Returns auditor type at \a index.
*/
SoNotRec::Type
SoAuditorList::getType(const int index) const
{
  // the temporary int is necessary since gcc 2.95.2
  // does not allow a direct cast from void * to SoNotRec::Type
  // pederb, 20000219
  //
  // the "intermediate" cast from void* to long is needed for the SGI
  // MIPSPro 64-bits CC compiler to not complain
  // mortene, 20020611
  int tmp = (int)((long)SbPList::operator[](index*2+1));
  return (SoNotRec::Type)tmp;
}

/*!
  Remove auditor at \a index.
*/
void
SoAuditorList::remove(const int index)
{
  assert(index >= 0 && index < this->getLength());
  SbPList::remove(index * 2); // ptr
  SbPList::remove(index * 2); // type
}

/*!
  Remove \a auditor of \a type from list.
*/
void
SoAuditorList::remove(void * const auditor, const SoNotRec::Type type)
{
  this->remove(this->find(auditor, type));
}

/*!
  Send notification to all our auditors.
*/
void
SoAuditorList::notify(SoNotList * l)
{
  const int num = this->getLength();
  if (num == 1) { // fast path for common case
    this->doNotify(l, this->getObject(0), this->getType(0));
  }
  else if (num > 1) { // handle multiple auditors
    // FIXME: should perhaps use a more general mechanism to detect when
    // to ignore notification? (In SoFieldContainer::notify() -- based
    // on SoNotList::getTimeStamp()?) 20000304 mortene.
    SbList <void *> notified(num);
    
    for (int i = 0; i < num; i++) {
      void * auditor = this->getObject(i);
      if (notified.find(auditor) == -1) {
        // use a copy of 'l', since the notification list might change
        // when auditors are notified
        SoNotList listcopy(l);
        this->doNotify(&listcopy, auditor, this->getType(i));
        notified.append(auditor);
      }
    }
    
    // FIXME: it should be possible for the application programmer to
    // do this (it is for instance useful and tempting to do it upon
    // changes in engines). pederb, 2001-11-06
    assert(num == this->getLength() &&
           "auditors can not be removed during the notification loop");
  }
}

//
// Private method used to propagate 'l' to the 'auditor' of type 'type'
//
void 
SoAuditorList::doNotify(SoNotList * l, const void * auditor, const SoNotRec::Type type)
{
  switch (type) {
  case SoNotRec::CONTAINER:
  case SoNotRec::PARENT:
    {
      SoFieldContainer * obj = (SoFieldContainer *)auditor;
      obj->notify(l);
    }
    break;
    
  case SoNotRec::SENSOR:
    {
      SoDataSensor * obj = (SoDataSensor *)auditor;
#if COIN_DEBUG && 0 // debug
      SoDebugError::postInfo("SoAuditorList::notify",
                             "notify and schedule sensor: %p", obj);
#endif // debug
      obj->notify(l);
      obj->schedule();
    }
    break;
    
  case SoNotRec::FIELD:
  case SoNotRec::ENGINE:
    {
      // We used to check whether or not the fields was already
      // dirty before we transmitted the notification
      // message. This is _not_ correct (the dirty flag is
      // conceptually only relevant for whether or not to do
      // re-evaluation), so don't try to "optimize" the
      // notification mechanism by re-introducing that "feature".
      // :^/
      ((SoField *)auditor)->notify(l);
    }
    break;
    
  default:
    assert(0 && "Unknown auditor type");
  }
}
