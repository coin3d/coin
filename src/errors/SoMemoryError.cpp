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

/*!
  \class SoMemoryError SoMemoryError.h Inventor/errors/SoMemoryError.h
  \brief The SoMemoryError class is used to inform of problems with
  memory allocation.

  Modern operating systems takes care of handling most out of memory
  conditions for you, but in certain situations it can be wise to do
  some manual checking and intervention. This class is provided as an
  aid to help out in these situations.

  The basic design of the Coin library is to pass on the
  responsibility for handling failed attempts at memory allocation to
  the application programmer. If you want to detect and take care of
  these, you should compile Coin with the C++ exception throwing on
  and wrap your code within \c try{} and \c catch{} blocks. The most
  you can do if you get a failed allocation is typically to notify the
  user and then exit the application, though, and this is something
  most operating systems will do for you, so you probably need not
  consider this at all.

  So, where does the SoMemoryError class come in handy? There are
  certain things which could happen within the library which are best
  taken care of by internally handling failed attempts at memory
  allocation. An example: the user tries to load a model file which
  contains a filename pointer to a huge bitmapfile with a texture
  map. The machine you are one does not provide you with enough memory
  to load the file and prepare the texture image for rendering,
  though. This is a case where it is possible to just emit a warning
  and continue. The warning will then be passed through this class.

  Note that SoMemoryError is probably not of much use to the
  application programmer.

*/

#include <Inventor/errors/SoMemoryError.h>
#include <Inventor/SoType.h>
#include <Inventor/SbName.h>
#include <stdio.h>


SoType SoMemoryError::classTypeId;
SoErrorCB * SoMemoryError::callback = SoError::defaultHandlerCB;
void * SoMemoryError::callbackData = NULL;


// Documented for parent class.
void
SoMemoryError::initClass(void)
{
  SoMemoryError::callback = SoError::defaultHandlerCB;
  SoMemoryError::callbackData = NULL;
  SoMemoryError::classTypeId =
    SoType::createType(SoError::getClassTypeId(), "MemoryError");
}

// Documented for parent class.
SoType
SoMemoryError::getClassTypeId(void)
{
  return SoMemoryError::classTypeId;
}

// Documented for parent class.
SoType
SoMemoryError::getTypeId(void) const
{
  return SoMemoryError::classTypeId;
}

// Documented for parent class.
void
SoMemoryError::setHandlerCallback(SoErrorCB * const function,
                                  void * const data)
{
  SoMemoryError::callback = function;
  SoMemoryError::callbackData = data;
}

// Documented for parent class.
SoErrorCB *
SoMemoryError::getHandlerCallback(void)
{
  return SoMemoryError::callback;
}

// Documented for parent class.
void *
SoMemoryError::getHandlerData(void)
{
  return SoMemoryError::callbackData;
}

/*!
  Posts a warning about a failed memory allocation. \a whatWasAllocated
  should contain a description of what we tried to allocate.
*/
void
SoMemoryError::post(const char * const whatWasAllocated)
{
  SoMemoryError error;
  error.setDebugString("ERROR allocating '");
  error.appendToDebugString(whatWasAllocated);
  error.appendToDebugString("'.");
  error.handleError();
}

// Documented for parent class.
SoErrorCB * SoMemoryError::getHandler(void * & data) const
{
  data = SoMemoryError::callbackData;
  return SoMemoryError::callback;
}
