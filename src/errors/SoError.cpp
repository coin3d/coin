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

/*************************************************************************/

#include <stdio.h>

/*************************************************************************/

/*!
  \class SoError SoError.h Inventor/errors/SoError.h
  \brief The SoError class is the base class for all the error
  handling classes.

  The default error handler just prints messages on the standard error
  output channel, but this can be overridden by client applications.

  Being able to override the default handler is useful when you want
  to collect error messages upon e.g. model import for later
  presentation of the messages to the user in any custom manner (like
  for instance in a GUI messagebox).

  The SoError class is not designed to be particularly useful for
  "direct use". Within the Coin library it is only used through its
  subclasses.

*/

#include <Inventor/errors/SoErrors.h>

#include <Inventor/SoType.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/SoPath.h>
#include <Inventor/SbName.h>
#include <Inventor/engines/SoEngine.h>

#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>

#include <../snprintf.h> // snprintf() and vsnprintf() definitions.


SoType SoError::classTypeId;
SoErrorCB * SoError::callback = SoError::defaultHandlerCB;
void * SoError::callbackData = NULL;
char * SoError::strbuffer = NULL;
size_t SoError::strbuffersize = 0;

static const size_t buffer_inc = 512;


/*!
  \fn SoError::~SoError()

  The default destructor does nothing.
*/


/*!
  This method takes care of initializing all static data for the class.
*/
void
SoError::initClass(void)
{
  (void)atexit(SoError::cleanClass);

  SoError::callback = defaultHandlerCB;
  SoError::callbackData = NULL;
  SoError::classTypeId =
    SoType::createType(SoType::badType(), SbName("Error"));
}

/*!
  \internal
  Free resources used by this class.
 */
void
SoError::cleanClass(void)
{
#if COIN_DEBUG
  delete SoError::strbuffer; SoError::strbuffer = NULL;
#endif // COIN_DEBUG
}

/*!
  This static method initializes all the SoError classes.
*/
void
SoError::initClasses(void)
{
  SoError::initClass();
  SoDebugError::initClass();
  SoMemoryError::initClass();
  SoReadError::initClass();
}

/*!
  This static method returns the SoType for this class.

  \sa getTypeId()
*/
SoType
SoError::getClassTypeId(void)
{
  return SoError::classTypeId;
}

/*!
  This method returns the SoType of a particular object instance.

  \sa getClassTypeId()
*/
SoType
SoError::getTypeId(void) const
{
  return SoError::classTypeId;
}

/*!
  This method returns \c TRUE if the error instance is of - or derived
  from - \a type, and \c FALSE otherwise.
*/
SbBool
SoError::isOfType(const SoType type) const
{
  const SoType myType = getTypeId();
  return (myType == type) ? TRUE : myType.isDerivedFrom( type );
}

/*!
  This method sets the error handler callback for messages posted
  via this class.

  \sa defaultHandlerCB()
*/
void
SoError::setHandlerCallback(SoErrorCB * const function, void * const data)
{
  SoError::callback = function;
  SoError::callbackData = data;
}

/*!
  Returns the error handler callback for messages posted via this
  class.
*/
SoErrorCB *
SoError::getHandlerCallback(void)
{
  return SoError::callback;
}

/*!
  This method returns the pointer used for passing data back to the
  callback handler method.
*/
void *
SoError::getHandlerData(void)
{
  return SoError::callbackData;
}

/*!
  This method returns an SbString containing error info from the given
  error instance.
*/
const SbString &
SoError::getDebugString(void) const
{
  return this->debugstring;
}

/*!
  This method posts an error message.  The \a format string and the
  trailing aguments should follow the printf() standard.
*/
void
SoError::post(const char * const format, ...)
{
  va_list args;
  va_start(args, format);

  while (!SoError::strbuffersize || vsnprintf(SoError::strbuffer,
                                              SoError::strbuffersize,
                                              format, args) == -1) {
    delete SoError::strbuffer;
    SoError::strbuffersize += buffer_inc;
    SoError::strbuffer = new char[SoError::strbuffersize];
  }

  va_end(args);

  SoError error;
  error.setDebugString(SoError::strbuffer);
  error.handleError();
}

/*!
  Constructs a string identifying the \a node with name (if available)
  and memory pointer.
*/
SbString
SoError::getString(const SoNode * const node)
{
  SbString str;
  SoError::generateBaseString(str, node, "node");
  return str;
}

/*!
  Constructs a string identifying the \a path with name (if available)
  and memory pointer.
*/
SbString
SoError::getString(const SoPath * const path)
{
  SbString str;
  SoError::generateBaseString(str, path, "path");
  return str;
}

/*!
  Constructs a string identifying the \a engine with name (if available)
  and memory pointer.
*/
SbString
SoError::getString(const SoEngine * const engine)
{
  SbString str;
  SoError::generateBaseString(str, engine, "engine");
  return str;
}

/*!
  Contains the default code for handling error strings.

  Default treatment of an error message is to print it out on the
  standard error file handle.
*/
void
SoError::defaultHandlerCB(const SoError * error, void * /* data */)
{
  (void)fprintf(stderr, "%s\n", error->getDebugString().getString());
}

/*!
  This is just a convenience wrapper around the getHandlerCallback() and
  getHandlerData() methods.
*/
SoErrorCB *
SoError::getHandler(void * & data) const
{
  data = SoError::callbackData;
  return SoError::callback;
}

/*!
  Replace the latest stored debug string with \a str.
*/
void
SoError::setDebugString(const char * const str)
{
  this->debugstring = str;
}

/*!
  Add \a str at the end of the currently stored debug string.
*/
void
SoError::appendToDebugString(const char * const str)
{
  this->debugstring += str;
}

/*!
  This method calls the appropriate handler for an error instance.  All
  error handling goes through this method, and is therefore a good
  candidate for a debugger breakpoint.
*/
void
SoError::handleError(void)
{
  void * arg = NULL;
  SoErrorCB * function = this->getHandler(arg);
  assert(function);
  (*function)(this, arg);
}

/*!
  This method is used by the getString methods.  It just generates a
  '<what> named "<name>" at address <address>' string.
*/
void
SoError::generateBaseString(SbString & str,
                            const SoBase * const base,
                            const char * const what)
{
  while (!SoError::strbuffersize ||
         snprintf(SoError::strbuffer,
                  SoError::strbuffersize,
                  "%s named \"%s\" at address %p",
                  what, base->getName().getString(), base) == -1) {
    delete SoError::strbuffer;
    SoError::strbuffersize += buffer_inc;
    SoError::strbuffer = new char[SoError::strbuffersize];
  }

  str = SoError::strbuffer;
}
