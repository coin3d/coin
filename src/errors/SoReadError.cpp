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
  \class SoReadError Inventor/errors/SoReadError.h
  \brief The SoReadError class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/errors/SoReadError.h>

#include <Inventor/SbName.h>
#include <Inventor/SoType.h>
#include <Inventor/SoInput.h>

// FIXME: use configure tests (and check for <strstream> first,
// BTW). 19991215 mortene.
#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32

#include <stdarg.h>
#include <stdio.h>

/*!
  \var SoReadError::classTypeId

  FIXME: write doc.
*/

SoType SoReadError::classTypeId;

/*!
  \var SoReadError::callback

  FIXME: write doc.
*/

SoErrorCB * SoReadError::callback = SoError::defaultHandlerCB;

/*!
  \var SoReadError::callbackData

  FIXME: write doc.
*/

void * SoReadError::callbackData = NULL;

/*!
  This static method initializes static data for the SoReadError class.
*/

void
SoReadError::initClass(void)
{
  SoReadError::callback = SoError::defaultHandlerCB;
  SoReadError::callbackData = NULL;
  SoReadError::classTypeId =
    SoType::createType(SoError::getClassTypeId(), "ReadError");
}

/*!
  FIXME: write doc.
*/

SoType
SoReadError::getClassTypeId(void)
{
  return SoReadError::classTypeId;
}

/*!
  FIXME: write doc.
*/

SoType
SoReadError::getTypeId(void) const
{
  return SoReadError::classTypeId;
}

/*!
  FIXME: write doc.
*/

void
SoReadError::setHandlerCallback(SoErrorCB * const function, void * const data)
{
  SoReadError::callback = function;
  SoReadError::callbackData = data;
}

/*!
  FIXME: write doc.
*/

SoErrorCB *
SoReadError::getHandlerCallback(void)
{
  return SoReadError::callback;
}

/*!
  FIXME: write doc.
*/

void *
SoReadError::getHandlerData(void)
{
  return SoReadError::callbackData;
}

/*!
  FIXME: write doc.
*/

void
SoReadError::post(const SoInput * const in, const char * const format, ...)
{
  va_list args;
  va_start(args, format);
  ostrstream msg;
  msg.vform(format, args);
  va_end(args);

  SbString s;
  in->getLocationString(s);

  ostrstream buffer;
  buffer << "Coin read error: " << msg.str() << endl << s.getString();

  SoReadError error;
  error.setDebugString(buffer.str());
  error.handleError();
}

/*!
  FIXME: write doc.
*/

SoErrorCB *
SoReadError::getHandler(void * & data) const
{
  data = SoReadError::callbackData;
  return SoReadError::callback;
}
