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
  \class SoReadError SoReadError.h Inventor/errors/SoReadError.h
  \brief The SoReadError class is used during model import operations.

  During model file import, this class will be used to output any
  error or warning messages.

  Depending on your application, setting up your own error handler
  callbacks for SoReadError might be a good idea.
*/

#include <Inventor/errors/SoReadError.h>

#include <Inventor/SbName.h>
#include <Inventor/SoType.h>
#include <Inventor/SoInput.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <../snprintf.h> // snprintf() and vsnprintf() definitions.


SoType SoReadError::classTypeId;
SoErrorCB * SoReadError::callback = SoError::defaultHandlerCB;
void * SoReadError::callbackData = NULL;
char * SoReadError::strbuffer = NULL;
size_t SoReadError::strbuffersize = 0;

static const size_t buffer_inc = 512;


// Documented for parent class.
void
SoReadError::initClass(void)
{
  (void)atexit(SoReadError::cleanClass);

  SoReadError::callback = SoError::defaultHandlerCB;
  SoReadError::callbackData = NULL;
  SoReadError::classTypeId =
    SoType::createType(SoError::getClassTypeId(), "ReadError");
}

/*!
  \internal
  Free resources used by this class.
 */
void
SoReadError::cleanClass(void)
{
  delete SoReadError::strbuffer;
}

// Documented for parent class.
SoType
SoReadError::getClassTypeId(void)
{
  return SoReadError::classTypeId;
}

// Documented for parent class.
SoType
SoReadError::getTypeId(void) const
{
  return SoReadError::classTypeId;
}

// Documented for parent class.
void
SoReadError::setHandlerCallback(SoErrorCB * const function, void * const data)
{
  SoReadError::callback = function;
  SoReadError::callbackData = data;
}

// Documented for parent class.
SoErrorCB *
SoReadError::getHandlerCallback(void)
{
  return SoReadError::callback;
}

// Documented for parent class.
void *
SoReadError::getHandlerData(void)
{
  return SoReadError::callbackData;
}

/*!
  Method used from import code to post error or warning messages for
  model files which are not 100% compliant to the format specification.

  The messages will be wrapped within information on line number,
  file offset etc.
*/
void
SoReadError::post(const SoInput * const in, const char * const format, ...)
{
  va_list args;
  va_start(args, format);

  while (!SoReadError::strbuffersize || vsnprintf(SoReadError::strbuffer,
                                                  SoReadError::strbuffersize,
                                                  format, args) == -1) {
    delete SoReadError::strbuffer;
    SoReadError::strbuffersize += buffer_inc;
    SoReadError::strbuffer = new char[SoReadError::strbuffersize];
  }

  va_end(args);

  SoReadError error;
  error.setDebugString("Coin read error: ");
  error.appendToDebugString(SoReadError::strbuffer);
  error.appendToDebugString("\n");
  SbString s;
  in->getLocationString(s);
  error.appendToDebugString(s.getString());
  error.handleError();
}

// Documented for parent class.
SoErrorCB *
SoReadError::getHandler(void * & data) const
{
  data = SoReadError::callbackData;
  return SoReadError::callback;
}
