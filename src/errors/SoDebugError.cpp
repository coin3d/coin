/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoDebugError SoDebugError.h Inventor/errors/SoDebugError.h
  \brief The SoDebugError class is the internal debugging message
  passing mechanism.

  This class basically serves two purposes:

  1) it is the message interface through which error and warning
  conditions are passed to the programmer which is using the Coin
  library API for building applications.  These messages are generated
  when API methods are used in an incorrect manner, or if actions on
  the library has caused it to enter an inconsistent state. Coin
  programmers should then quickly be able to trace down errors in
  their application code.

  For this service to be available, the Coin library must be compiled
  with debugging information. For release builds, most of this
  "programmer's safety net" is removed to gain maximum speed and to
  use minimum space (build the smallest possible library).

  2) Coin application programmers can call the SoDebugError methods
  within their own code as a convenient way of debugging application
  code, replacing the usual cascades of fprintf() calls.

*/

#include <Inventor/errors/SoDebugError.h>

#include <Inventor/SoType.h>
#include <Inventor/SbName.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


SoType SoDebugError::classTypeId;
SoErrorCB * SoDebugError::callback = SoError::defaultHandlerCB;
void * SoDebugError::callbackData = NULL;


/*!
  \enum SoDebugError::Severity

  Specifies the available severity levels of the debug messages.
*/


// Documented for parent class.
void
SoDebugError::initClass(void)
{
  SoDebugError::callback = SoError::defaultHandlerCB;
  SoDebugError::callbackData = NULL;
  SoDebugError::classTypeId =
    SoType::createType(SoError::getClassTypeId(), "DebugError");
}

// Documented for parent class.
void
SoDebugError::setHandlerCallback(SoErrorCB * const function, void * const data)
{
  SoDebugError::callback = function;
  SoDebugError::callbackData = data;
}

// Documented for parent class.
SoErrorCB *
SoDebugError::getHandlerCallback(void)
{
  return SoDebugError::callback;
}

// Documented for parent class.
void *
SoDebugError::getHandlerData(void)
{
  return SoDebugError::callbackData;
}

// Documented for parent class.
SoType
SoDebugError::getClassTypeId(void)
{
  return SoDebugError::classTypeId;
}

// Documented for parent class.
SoType
SoDebugError::getTypeId(void) const
{
  return SoDebugError::classTypeId;
}

/*!
  Returns severity level of current message.

  You can use this to filter out debug messages and only let warnings
  and/or errors pass through to the end-user if you have set your own
  handler callback.

  \sa setHandlerCallback()
*/
SoDebugError::Severity
SoDebugError::getSeverity(void) const
{
  return this->severity;
}


#define SODEBUGERROR_POST(SEVERITY, TYPE) \
  va_list args; \
  va_start(args, format); \
  SbString s; \
  s.vsprintf(format, args); \
  va_end(args); \
 \
  SoDebugError error; \
  error.severity = SEVERITY; \
  error.setDebugString("Coin "); \
  error.appendToDebugString(TYPE); \
  error.appendToDebugString(" in "); \
  error.appendToDebugString(source); \
  error.appendToDebugString("(): "); \
  error.appendToDebugString(s.getString()); \
  error.handleError()


/*!
  This method posts a message with severity level "ERROR". \a source should
  be the name of the calling function.
*/
void
SoDebugError::post(const char * const source, const char * const format, ...)
{
  SODEBUGERROR_POST(ERROR, "error");
}

/*!
  This method posts a message with severity level "WARNING". \a source should
  be the name of the calling function.
*/
void
SoDebugError::postWarning(const char * const source, const char * const format, ...)
{
  SODEBUGERROR_POST(WARNING, "warning");
}

/*!
  This method posts a message with severity level "INFO". \a source should
  be the name of the calling function.
*/
void
SoDebugError::postInfo(const char * const source, const char * const format, ...)
{
  SODEBUGERROR_POST(INFO, "info");
}

// Documented for parent class.
SoErrorCB *
SoDebugError::getHandler(void * & data) const
{
  data = SoDebugError::callbackData;
  return SoDebugError::callback;
}
