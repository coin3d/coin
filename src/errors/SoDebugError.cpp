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
  \class SoDebugError Inventor/errors/SoDebugError.h
  \brief The SoDebugError class is yet to be documented.

  FIXME: write doc.
*/

/*¡
  potensial buffer overflow errors detected, should be fixed - 990610 larsa
*/

#include <Inventor/errors/SoDebugError.h>

#include <Inventor/SbString.h>
#include <Inventor/SbName.h>
#include <Inventor/SoType.h>

#include <stdarg.h>
#include <stdio.h>

/*!
  \var SoDebugError::classTypeId

  FIXME: write doc.
*/

SoType SoDebugError::classTypeId;

/*!
  \var SoDebugError::callback

  FIXME: write doc.
*/

SoErrorCB * SoDebugError::callback = SoError::defaultHandlerCB;

/*!
  \var SoDebugError::callbackData

  FIXME: write doc.
*/

void * SoDebugError::callbackData = NULL;

/*!
  \enum SoDebugError::Severity

  FIXME: write doc.
*/

/*!
  This static method initializes static data for the SoDebugError class.
*/

void
SoDebugError::initClass(void)
{
  SoDebugError::callback = SoError::defaultHandlerCB;
  SoDebugError::callbackData = NULL;
  SoDebugError::classTypeId =
    SoType::createType(SoError::getClassTypeId(), "DebugError");
}

/*!
  FIXME: write doc.
*/

void
SoDebugError::setHandlerCallback(SoErrorCB * const function,
                                 void * const data)
{
  SoDebugError::callback = function;
  SoDebugError::callbackData = data;
}

/*!
  FIXME: write doc.
*/

SoErrorCB *
SoDebugError::getHandlerCallback(void)
{
  return SoDebugError::callback;
}

/*!
  FIXME: write doc.
*/

void *
SoDebugError::getHandlerData(void)
{
  return SoDebugError::callbackData;
}

/*!
  FIXME: write doc.
*/

SoType
SoDebugError::getClassTypeId(void)
{
  return SoDebugError::classTypeId;
}

/*!
  FIXME: write doc.
*/

SoType
SoDebugError::getTypeId(void) const
{
  return SoDebugError::classTypeId;
}

/*!
  FIXME: write doc.
*/

SoDebugError::Severity
SoDebugError::getSeverity(void) const
{
  return this->severity;
}

/*!
  This method posts a message with severity level "ERROR".
*/

void
SoDebugError::post(const char * const methodName,
                   const char * const formatString,
                   ...)
{
  va_list args;
  va_start(args, formatString);
  char buffer[ 512 ]; // FIXME: possible overflow, 990610 larsa
  vsprintf(buffer, formatString, args);
  va_end(args);
  char string[ 512 ]; // FIXME: possible overflow, 990610 larsa
  sprintf(string, "Coin error in %s(): %s", methodName, buffer);
  SoDebugError error;
  error.severity = ERROR;
  error.setDebugString(string);
  error.handleError();
}

/*!
  FIXME: write doc.
*/

void
SoDebugError::postWarning(const char * const methodName,
                          const char * const formatString,
                          ...)
{
  va_list args;
  va_start(args, formatString);
  char buffer[ 512 ]; // FIXME: possible overflow?  990610 larsa
  vsprintf(buffer, formatString, args);
  va_end(args);
  char string[ 512 ]; // FIXME: possible overflow?  990610 larsa
  sprintf(string, "Coin warning in %s(): %s", methodName, buffer);
  SoDebugError error;
  error.severity = WARNING;
  error.setDebugString(string);
  error.handleError();
}

/*!
  FIXME: write doc.
*/

void
SoDebugError::postInfo(const char * const methodName,
                       const char * const formatString,
                       ...)
{
  va_list args;
  va_start(args, formatString);
  char buffer[ 512 ]; // FIXME: possible overflow?  990610 larsa
  vsprintf(buffer, formatString, args);
  va_end(args);
  char string[ 512 ]; // FIXME: possible overflow?  990610 larsa
  sprintf(string, "Coin info from %s(): %s", methodName, buffer);
  SoDebugError error;
  error.severity = INFO;
  error.setDebugString(string);
  error.handleError();
}

/*!
  FIXME: write doc.
*/

SoErrorCB *
SoDebugError::getHandler(void * & data) const
{
  data = SoDebugError::callbackData;
  return SoDebugError::callback;
}
