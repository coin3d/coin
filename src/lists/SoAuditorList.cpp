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
  \class SoAuditorList Inventor/lists/SoAuditorList.h
  \brief The SoAuditorList class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/lists/SoAuditorList.h>

#include <Inventor/fields/SoFieldContainer.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/sensors/SoDataSensor.h>
#include <Inventor/nodes/SoNode.h>
#include <assert.h>

/*!
  A constructor.
*/

SoAuditorList::SoAuditorList()
  : SbPList(8) // instead of 4
{
}

/*!
  The destructor.
*/
SoAuditorList::~SoAuditorList()
{
  while (this->getLength()) this->remove(0);
}

/*!
  FIXME: write doc.
*/

void
SoAuditorList::append(void * const auditor, const SoNotRec::Type type)
{
  SbPList::append(auditor);
  SbPList::append((void *) type);
}

/*!
  FIXME: write doc.
*/

void
SoAuditorList::set(const int index,
                   void * const auditor,
                   const SoNotRec::Type type)
{
  assert(index >= 0 && index < this->getLength());
  SbPList::set(index * 2, auditor);
  SbPList::set(index * 2+1, (void *)type);
}

/*!
  FIXME: write doc.
*/

int
SoAuditorList::find(void * const auditor,
                    const SoNotRec::Type type) const
{
  const int num = SbPList::getLength();
  for (int i = 0; i < num; i += 2) {
    if (SbPList::get(i) == auditor &&
        SbPList::get(i+1) == (void *) type) {
      return i / 2;
    }
  }
  return -1;
}

/*!
  FIXME: write doc.
*/

void *
SoAuditorList::getObject(const int index) const
{
  return SbPList::operator[](index * 2);
}

/*!
  FIXME: write doc.
*/

SoNotRec::Type
SoAuditorList::getType(const int index) const
{
  return (SoNotRec::Type) (int)SbPList::operator[](index*2+1);
}

/*!
  FIXME: write doc.
*/

void
SoAuditorList::remove(const int index)
{
  assert(index >= 0 && index < this->getLength());

  // FIXME: isn't this "copy functionality" part of SbPList::remove()?
  // 19990622 mortene.
  // Remove item.
  const int num = SbPList::getLength();
  for (int i = index * 2; i < num - 2; i++)
    SbPList::set(i, this->get(i + 2));
  this->truncate(num-2);
}

/*!
  FIXME: write doc.
*/

void
SoAuditorList::remove(void * const auditor, const SoNotRec::Type type)
{
  this->remove(this->find(auditor, type));
}

/*!
  FIXME: write doc.
*/

int
SoAuditorList::getLength() const
{
  return SbPList::getLength() / 2;
}

/*!
  FIXME: write doc.
*/

void
SoAuditorList::notify(SoNotList * list)
{
  const int max = this->getLength();
  for (int i=0;i<max;i++) {
    SoNotRec::Type type=this->getType(i);
    switch (type) {
    case SoNotRec::CONTAINER:
      {
        SoFieldContainer * obj = (SoFieldContainer *)this->getObject(i);
        // FIXME: this leads to a failed assertion in SoNotification
        // (because of a missing call to
        // SoNotification::append()?). 19990205 mortene.
//        list->setLastType(SoNotRec::CONTAINER);
        obj->notify(list);
      }
      break;
    case SoNotRec::PARENT:
      {
        SoNode * obj = (SoNode *)this->getObject(i);
      // FIXME: this leads to a failed assertion in SoNotification
      // (because of a missing call to
      // SoNotification::append()?). 19990205 mortene.
//        list->setLastType(SoNotRec::PARENT);
        obj->notify(list);
      }
      break;
    case SoNotRec::SENSOR:
      {
        SoDataSensor * obj = (SoDataSensor *)this->getObject(i);
        obj->schedule();
      }
      break;
    case SoNotRec::FIELD:
    case SoNotRec::ENGINE:
    case SoNotRec::INTERP:
      {
        // FIXME: this leads to a failed assertion in SoNotification
        // (because of a missing call to
        // SoNotification::append()?). 19990205 mortene.
//        list->setLastType(type);
        SoField * obj = (SoField *)this->getObject(i);
        if (!obj->getDirty()) obj->notify(list);
      }
      break;
    default:
      assert(0);
    }
  }
}
