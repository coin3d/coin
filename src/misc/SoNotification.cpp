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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoNotRec include/Inventor/misc/SoNotification.h
  \brief The SoNotRec class ...

  FIXME: write class doc
*/

/*!
  \enum SoNotRec::Type include/Inventor/misc/SoNotification.h
  The Type enum is used to specify the notification type.
*/

/*!
  \class SoNotList include/Inventor/misc/SoNotification.h
  \brief The SoNotList class is a container class for a list of SoNotRec
  notification records.
*/


#include <Inventor/misc/SoNotification.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <assert.h>
#include <time.h>

/*!
  Constructor.
*/
SoNotRec::SoNotRec(SoBase * const theBase)
  : base(theBase), prev(NULL)
{
#if 0 // debug
  SoDebugError::postInfo("SoNotRec::SoNotRec", "rec %p, base %p",
			 this, base);
#endif // debug
}

/*!
  Sets the notification type.
*/
void
SoNotRec::setType(const SoNotRec::Type type)
{
  this->type = type;
}

/*!
  Returns the SoBase pointer for this notification.
*/
SoBase * 
SoNotRec::getBase(void) const
{
#if 0 // debug
  SoDebugError::postInfo("SoNotRec::getBase", "rec %p, base %p",
			 this, this->base);
#endif // debug
  return this->base;
}

/*!
  Returns the notification type.
*/
SoNotRec::Type
SoNotRec::getType(void) const
{
  return this->type;
}

/*!
  Returns the previous notification record.
*/
const SoNotRec *
SoNotRec::getPrevious(void) const
{
  assert(this != prev);
  return this->prev;
}

/*!
  Sets the previous notification record.
*/
void
SoNotRec::setPrevious(const SoNotRec * const prev)
{
  assert(this != prev);
  this->prev = prev;
}

/*!
  Prints debug information.
*/
void
SoNotRec::print(FILE * const file) const
{
#if COIN_DEBUG
  fprintf(file, "SoNotRec %p {\n", this);
  switch (this->type) {
  case CONTAINER:       fprintf(file, "  type = CONTAINER, \n"); break;
  case PARENT:          fprintf(file, "  type = PARENT, \n"); break;
  case SENSOR:          fprintf(file, "  type = SENSOR, \n"); break;
  case FIELD:           fprintf(file, "  type = FIELD, \n"); break;
  case ENGINE:          fprintf(file, "  type = ENGINE, \n"); break;
  case INTERP:          fprintf(file, "  type = INTERP, \n"); break;
  default:              fprintf(file, "  type = ???, \n"); break;
  }
  if (this->base)     fprintf(file, "  base = %p, \n", this->base);
  else                  fprintf(file, "  base = NULL\n");
  if (this->prev) fprintf(file, "  previous = %p\n", this->prev);
  else                  fprintf(file, "  previous = NULL\n");
  fprintf(file, "}\n");
#endif
}

/*!
  A constructor.
*/
SoNotList::SoNotList(void)
{
  this->head = this->tail = NULL;
  this->firstnoderec = NULL;
  this->lastfield = NULL;
  this->lastengine = NULL;
  this->lastinterp = NULL;
  this->stamp = 0;
}

/*!
  Copy constructor.  Does a bitwise copy of the copyFrom object (no
  duplication of list elements).
*/
SoNotList::SoNotList(const SoNotList * const copyFrom)
{
  *this = *copyFrom;
}

/*!
  FIXME: write function documentation
*/
void
SoNotList::append(SoNotRec * const rec)
{
  // Link into list.
  rec->setPrevious(this->tail);
  this->tail = rec;
  if (!this->head) this->head = this->tail;

  if (!this->firstnoderec) this->firstnoderec = rec;
  this->stamp = time(NULL);
}

/*!
  FIXME: write function documentation
*/
void
SoNotList::append(SoNotRec * const rec, SoField * const field)
{
#if 0 // debug
  SoDebugError::postInfo("SoNotList::append", "rec %p, base %p, field %p",
			 rec, rec->getBase(), field);
#endif // debug

  assert(field);
  this->lastfield = field;
  this->append(rec);
}

/*!
  FIXME: write function documentation
*/
void
SoNotList::append(SoNotRec * const rec, SoVRMLInterpOutput * const interpout)
{
  assert(interpout);
  this->lastinterp = interpout;
  this->append(rec);
}

/*!
  FIXME: write function documentation
*/
void
SoNotList::append(SoNotRec * const rec, SoEngineOutput * const engineout)
{
  assert(engineout);
  this->lastengine = engineout;
  this->append(rec);
}

/*!
  FIXME: write function documentation
*/
void
SoNotList::setLastType(const SoNotRec::Type type)
{
  assert(this->tail);
  this->tail->setType(type);
#if 0 // OBSOLETE: this looks wrong. 19990623 mortene.
  switch (type) {
  case SoNotRec::FIELD:
  case SoNotRec::ENGINE:
  case SoNotRec::INTERP:
    this->firstnoderec = NULL;
  default:
    break;
  }
#endif // OBSOLETE
}

/*!
  This function returns the first record in the list.
*/
SoNotRec *
SoNotList::getFirstRec(void) const
{
  return this->head;
}

/*!
  This function returns the last record in the list.
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
  Returns the last field set by notification.
*/
SoField *
SoNotList::getLastField(void) const
{
  return this->lastfield;
}

/*!
  FIXME: write function documentation
*/
SoVRMLInterpOutput *
SoNotList::getLastInterpOutput(void) const
{
  return this->lastinterp;
}

/*!
  FIXME: write function documentation
*/
SoEngineOutput *
SoNotList::getLastEngineOutput(void) const
{
  return this->lastengine;
}

/*!
  Returns the time stamp for this notification.
*/
uint32_t
SoNotList::getTimeStamp(void) const
{
  return this->stamp;
}

/*!
  Dump contents of list. For debugging only.
*/
void
SoNotList::print(FILE * const file) const
{
#if COIN_DEBUG
  fprintf(file, "SoNotList {\n");
  const SoNotRec * ptr = this->tail;
  while (ptr) {
    ptr->print(file);
    ptr = ptr->getPrevious();
  }
  fprintf(file, "}\n");
#endif // COIN_DEBUG
}
