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
  \class SoNotRec SoNotRec.h Inventor/misc/SoNotification.h
  \brief The SoNotRec class specifies records for notification lists.
  \ingroup general

  \sa SoNotification
*/

/*!
  \enum SoNotRec::Type

  This enum is used to specify the type of the notification source
  within the record.
*/

#include <Inventor/misc/SoNotRec.h>
#include <Inventor/errors/SoDebugError.h>
#include <assert.h>

#if COIN_DEBUG  // for SoNotRec::print() method
#include <Inventor/misc/SoBase.h>
#include <Inventor/SbName.h>
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
