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
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoMemoryError Inventor/elements/SoMemoryError.h
  \brief The SoMemoryError class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/errors/SoMemoryError.h>

#include <Inventor/SoType.h>
#include <Inventor/SbName.h>

// FIXME: use configure tests (and check for <strstream> first,
// BTW). 19991215 mortene.
#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32

#include <stdio.h>

/*!
  \var SoMemoryError::classTypeId

  FIXME: write doc.
*/

SoType SoMemoryError::classTypeId;

/*!
  \var SoMemoryError::callback

  FIXME: write doc.
*/

SoErrorCB * SoMemoryError::callback = SoError::defaultHandlerCB;

/*!
  \var SoMemoryError::callbackData

  FIXME: write doc.
*/

void * SoMemoryError::callbackData = NULL;

/*!
  This static method initializes static data for the SoMemoryError class.
*/

void
SoMemoryError::initClass(void)
{
  SoMemoryError::callback = SoError::defaultHandlerCB;
  SoMemoryError::callbackData = NULL;
  SoMemoryError::classTypeId =
    SoType::createType(SoError::getClassTypeId(), "MemoryError");
}

/*!
  FIXME: write doc.
*/

SoType
SoMemoryError::getClassTypeId(void)
{
  return SoMemoryError::classTypeId;
}

/*!
  FIXME: write doc.
*/

SoType
SoMemoryError::getTypeId(void) const
{
  return SoMemoryError::classTypeId;
}

/*!
  FIXME: write doc.
*/

void
SoMemoryError::setHandlerCallback(SoErrorCB * const function,
                                  void * const data)
{
  SoMemoryError::callback = function;
  SoMemoryError::callbackData = data;
}

/*!
  FIXME: write doc.
*/

SoErrorCB *
SoMemoryError::getHandlerCallback(void)
{
  return SoMemoryError::callback;
}

/*!
  FIXME: write doc.
*/

void *
SoMemoryError::getHandlerData(void)
{
  return SoMemoryError::callbackData;
}

/*!
  FIXME: write doc.
*/

void
SoMemoryError::post(const char * const whatWasAllocated)
{
  ostrstream msg;
  msg << "ERROR allocating '" << whatWasAllocated << "'.";
  SoMemoryError error;
  error.setDebugString(msg.str());
  error.handleError();
}

/*!
  FIXME: write doc.
*/

SoErrorCB * SoMemoryError::getHandler(void * & data) const
{
  data = SoMemoryError::callbackData;
  return SoMemoryError::callback;
}
