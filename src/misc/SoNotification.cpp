/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoNotList SoNotification.h Inventor/misc/SoNotification.h
  \brief The SoNotList class is a list of SoNotRec notification records.
  \ingroup general
*/

#include <Inventor/misc/SoNotification.h>
#include <Inventor/misc/SoNotRec.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/SbName.h>
#include <assert.h>
#include <time.h>


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
