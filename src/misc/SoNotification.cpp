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
#include <assert.h>
#include <time.h>

/*!
  Constructor. Initializes the record with \a notifbase pointer.
*/
SoNotRec::SoNotRec(SoBase * const notifbase)
  : base(notifbase), prev(NULL)
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
  assert(this != prev);
  return this->prev;
}

/*!
  Set pointer to the previous notification record.
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
  (void)fprintf(file, "SoNotRec %p {\n  type = ", this);
  switch (this->type) {
  case CONTAINER:  (void)fprintf(file, "CONTAINER"); break;
  case PARENT:     (void)fprintf(file, "PARENT"); break;
  case SENSOR:     (void)fprintf(file, "SENSOR"); break;
  case FIELD:      (void)fprintf(file, "FIELD"); break;
  case ENGINE:     (void)fprintf(file, "ENGINE"); break;
  case INTERP:     (void)fprintf(file, "INTERP"); break;
  default:         (void)fprintf(file, "???"); break;
  }
  (void)fprintf(file, ", \n  base = %p, \n  previous = %p\n}\n",
                this->base, this->prev);
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
  this->lastinterp = NULL;
//    this->stamp = 0;
  this->stamp = time(NULL);
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
  // Link into list.
  rec->setPrevious(this->tail);
  this->tail = rec;
  if (!this->head) this->head = rec;

  if (!this->firstnoderec) this->firstnoderec = rec;
//    this->stamp = time(NULL);
}

/*!
  Append \a rec notification source to the list, setting \a field as
  the last field having been influenced by the notification process.
*/
void
SoNotList::append(SoNotRec * const rec, SoField * const field)
{
  assert(field);
  this->lastfield = field;
  this->append(rec);
}

/*!
  Append \a rec notification source to the list, setting \a interpout
  as the last VRML interpolator having been influenced by the
  notification process.
*/
void
SoNotList::append(SoNotRec * const rec, SoVRMLInterpOutput * const interpout)
{
  assert(interpout);
  this->lastinterp = interpout;
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
  Returns the last VRML interpolator output field touched by
  notification.
*/
SoVRMLInterpOutput *
SoNotList::getLastInterpOutput(void) const
{
  return this->lastinterp;
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
  (void)fprintf(file, "SoNotList {\n");
  const SoNotRec * ptr = this->tail;
  while (ptr) {
    ptr->print(file);
    ptr = ptr->getPrevious();
  }
  (void)fprintf(file, "}\n");
#endif // COIN_DEBUG
}
