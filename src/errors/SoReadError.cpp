/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
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

#include <Inventor/errors/SoReadError.h>

#include <Inventor/SoType.h>
#include <Inventor/SoInput.h>
#include <Inventor/SbName.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


SoType SoReadError::classTypeId;
SoErrorCB * SoReadError::callback = SoError::defaultHandlerCB;
void * SoReadError::callbackData = NULL;


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
  /* FIXME: Overriding the error handler for subclasses of SoError
     doesn't work yet. Use SoError::setHandlerCallback() instead as a
     workaround, but note that this will stop working when callback
     override is implemented properly. 2003-01-22 thammer.  
  */
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
  error.handleError();
}

// Documented for parent class.
SoErrorCB *
SoReadError::getHandler(void * & data) const
{
  data = SoReadError::callbackData;
  return SoReadError::callback;
}
