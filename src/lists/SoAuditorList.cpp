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
  \class SoAuditorList SoAuditorList.h Inventor/lists/SoAuditorList.h
  \brief The SoAuditorList class is used to keep track of auditors for certain object classes.
  \ingroup general

  This class is mainly for internal use (from SoBase) and it should
  not be necessary to be familiar with it for "ordinary" Coin use.
*/

#include <Inventor/lists/SoAuditorList.h>

#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoFieldContainer.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/sensors/SoDataSensor.h>


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

// Override from parent class.
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
  int tmp = (int)SbPList::operator[](index*2+1);
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
  for (int i = 0; i < num; i++) {
    void * auditor = this->getObject(i);
    SoNotRec::Type type = this->getType(i);

    switch (type) {

    case SoNotRec::CONTAINER:
    case SoNotRec::PARENT:
      {
        SoFieldContainer * obj = (SoFieldContainer *)auditor;
        SoNotRec rec(obj);
        l->append(&rec);
        l->setLastType(type);
        obj->notify(l);
      }
      break;

    case SoNotRec::SENSOR:
      {
        SoDataSensor * obj = (SoDataSensor *)auditor;
        obj->schedule();
      }
      break;

    case SoNotRec::FIELD:
    case SoNotRec::ENGINE:
    case SoNotRec::INTERP:
      {
        SoField * obj = (SoField *)auditor;
        if (!obj->getDirty()) obj->notify(l);
      }
      break;

    default:
      assert(0 && "Unknown auditor type");
    }
  }
}
