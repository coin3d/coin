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


/////// SoNotRec //////////////////////////////////////////////////////////

/*!
  \class SoNotRec SoNotification.h Inventor/misc/SoNotification.h
  \brief The SoNotRec class is the basis for records in notification lists.
  \ingroup general
*/

/*!
  \enum SoNotRec::Type

  This enum is used to specify the type of the notification source
  within the record.
*/

#include <Inventor/misc/SoNotification.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/SbName.h>
#include <assert.h>
#include <time.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoBase.h>
#endif // COIN_DEBUG

/*!
  Constructor. Initializes the record with \a notifbase pointer.
*/
SoNotRec::SoNotRec(SoBase * const notifbase)
  : type((SoNotRec::Type)-1), base(notifbase), prev(NULL)
{
}

/*!
  Set the \a type of the notification source of this record.
*/
void
SoNotRec::setType(const SoNotRec::Type type)
{
  this->type = type;
}

/*!
  Returns the notification source within this record.
*/
SoBase *
SoNotRec::getBase(void) const
{
  return this->base;
}

/*!
  Returns the type of the notification source within this record.
*/
SoNotRec::Type
SoNotRec::getType(void) const
{
  return this->type;
}

/*!
  Returns the previous notification source (i.e. the source that the
  base within this record was auditing).
*/
const SoNotRec *
SoNotRec::getPrevious(void) const
{
#if 0 // OBSOLETED: see comment on setPrevious(). 20000304 mortene.
  assert(this != this->prev);
#endif // OBSOLETED
  return this->prev;
}

/*!
  Set pointer to the previous notification record.
*/
void
SoNotRec::setPrevious(const SoNotRec * const prev)
{
#if 0 // OBSOLETED: looks like this can be allowed (and need to be
      // allowed under the current circumstances, as it hits under certain
      // conditions). 20000304 mortene
  assert(this != prev);
#endif // OBSOLETED
  this->prev = prev;
}

/*!
  Prints debug information.
*/
void
SoNotRec::print(FILE * const file) const
{
#if COIN_DEBUG
  (void)fprintf(file, "\tSoNotRec %p: type ", this);
  switch (this->type) {
  case CONTAINER:  (void)fprintf(file, "CONTAINER"); break;
  case PARENT:     (void)fprintf(file, "PARENT"); break;
  case SENSOR:     (void)fprintf(file, "SENSOR"); break;
  case FIELD:      (void)fprintf(file, "FIELD"); break;
  case ENGINE:     (void)fprintf(file, "ENGINE"); break;
  default:         (void)fprintf(file, "UNSET"); break;
  }
  if (this->base) {
    (void)fprintf(file, ", base %p (type %s, \"%s\")\n",
                  this->base, this->base->getTypeId().getName().getString(),
                  this->base->getName().getString());
  }
  else {
    (void)fprintf(file," base is NULL\n");
  }
#endif // COIN_DEBUG
}

/////// SoNotList /////////////////////////////////////////////////////////

/*!
  \class SoNotList SoNotification.h Inventor/misc/SoNotification.h
  \brief The SoNotList class is a list of SoNotRec notification records.
  \ingroup general
*/

/*!
  Initialize list.
*/
SoNotList::SoNotList(void)
{
  this->head = this->tail = NULL;
  this->firstnoderec = NULL;
  this->lastfield = NULL;
  this->lastengine = NULL;
  // this is used in SoNode::notify() to stop a notification
  // when a node has already been notified.
  this->stamp = SoNode::getNextNodeId();
}

/*!
  Copy constructor. Does a bitwise copy of the \a nl object (no
  duplication of list elements).
*/
SoNotList::SoNotList(const SoNotList * nl)
{
  *this = *nl;
}

/*!
  Append \a rec notification source to the list.
*/
void
SoNotList::append(SoNotRec * const rec)
{
#if COIN_DEBUG && 0 // debug
  if (rec->getBase()) {
    SoDebugError::postInfo("SoNotList::append", "%p - %p (base: %p %s \"%s\")",
                           this, rec, rec->getBase(),
                           rec->getBase()->getTypeId().getName().getString(),
                           rec->getBase()->getName().getString());
  }
  else {
    SoDebugError::postInfo("SoNotList::append", "%p - %p (base is NULL)",
                           this, rec);
  }
#endif // debug

  // Link into list.
  rec->setPrevious(this->tail);
  this->tail = rec;
  if (!this->head) this->head = rec;

  if (!this->firstnoderec) this->firstnoderec = rec;
}

/*!
  Append \a rec notification source to the list, setting \a field as
  the last field having been influenced by the notification process.
*/
void
SoNotList::append(SoNotRec * const rec, SoField * const field)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoNotList::append", "field %p", field);
#endif // debug
  assert(field);
  this->lastfield = field;
  this->append(rec);
}

/*!
  Append \a rec notification source to the list, setting \a engineout
  as the last engine output field having been influenced by the
  notification process.
*/
void
SoNotList::append(SoNotRec * const rec, SoEngineOutput * const engineout)
{
  assert(engineout);
  this->lastengine = engineout;
  this->append(rec);
}

/*!
  Set the \a type of the last notification record in the list.
*/
void
SoNotList::setLastType(const SoNotRec::Type type)
{
  assert(this->tail);
  this->tail->setType(type);

  switch (type) {
  case SoNotRec::FIELD:
  case SoNotRec::ENGINE:
    this->firstnoderec = NULL;
  default:
    break;
  }
}

/*!
  Returns the first record in the list.
*/
SoNotRec *
SoNotList::getFirstRec(void) const
{
  return this->head;
}

/*!
  Returns the last record in the list.
*/
SoNotRec *
SoNotList::getLastRec(void) const
{
  return this->tail;
}

/*!
  Returns the first record in the list which is derived from SoBase.
*/
SoNotRec *
SoNotList::getFirstRecAtNode(void) const
{
  return this->firstnoderec;
}

/*!
  Returns the last field touched by notification.
*/
SoField *
SoNotList::getLastField(void) const
{
  return this->lastfield;
}

/*!
  Returns the last engine output field touched by notification.
*/
SoEngineOutput *
SoNotList::getLastEngineOutput(void) const
{
  return this->lastengine;
}

/*!
  Returns the time stamp when the notification started.
*/
uint32_t
SoNotList::getTimeStamp(void) const
{
  return this->stamp;
}

/*!
  Dump contents of list from tail record and backwards. Only available
  if compiled with debug information on.
*/
void
SoNotList::print(FILE * const file) const
{
#if COIN_DEBUG
  (void)fprintf(file, "SoNotList: %p\n", this);
  const SoNotRec * ptr = this->tail;
  while (ptr) {
    ptr->print(file);
    ptr = ptr->getPrevious();
  }
  (void)fprintf(file, "\tfirstAtNode = %p, lastField = %p\n",
                this->getFirstRecAtNode(), this->getLastField());
#endif // COIN_DEBUG
}
