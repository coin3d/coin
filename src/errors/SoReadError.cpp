/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoReadError SoReadError.h Inventor/errors/SoReadError.h
  \brief The SoReadError class is used during model import operations.
  \ingroup errors

  During model file import, this class will be used to output any
  error or warning messages.

  Depending on your application, setting up your own error handler
  callbacks for SoReadError might be a good idea.
*/

// *************************************************************************

#include <Inventor/errors/SoReadError.h>

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include <Inventor/SoType.h>
#include <Inventor/SoInput.h>
#include <Inventor/SbName.h>

// *************************************************************************

SoType SoReadError::classTypeId STATIC_SOTYPE_INIT;
SoErrorCB * SoReadError::callback = SoError::defaultHandlerCB;
void * SoReadError::callbackData = NULL;

// *************************************************************************

// Documented for parent class.
void
SoReadError::initClass(void)
{
  SoReadError::callback = SoError::defaultHandlerCB;
  SoReadError::callbackData = NULL;
  SoReadError::classTypeId =
    SoType::createType(SoError::getClassTypeId(), "ReadError");
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
  SbString formatstr;
  formatstr.vsprintf(format, args);
  va_end(args);

  SoReadError error;
  error.setDebugString("Coin read error: ");
  error.appendToDebugString(formatstr.getString());
  error.appendToDebugString("\n");

  SbString s;
  in->getLocationString(s);
  error.appendToDebugString(s.getString());

  // for some reason the getIVVersion() function is not const. We
  // therefore need to make the SoInput cast here to be able to test
  // for Inventor V1.0 files
  if (const_cast<SoInput *>(in)->getIVVersion() == 1.0f) {
    error.appendToDebugString("\nThis operation might have failed due to limited "
                              "support for old Inventor files. If this is the case, "
                              "please get in touch with Kongsberg Oil & Gas Technologies "
                              "at <coin-support@coin3d.org>, and we will "
                              "rectify the situation.");
  }

  if (SoReadError::callback != SoError::defaultHandlerCB) {
    SoReadError::callback(&error, SoReadError::callbackData);
  }
  else {
    error.handleError();
  }
}

// Documented for parent class.
SoErrorCB *
SoReadError::getHandler(void * & data) const
{
  data = SoReadError::callbackData;
  return SoReadError::callback;
}
