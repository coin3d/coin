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

/*************************************************************************/

#include <stdio.h>

#if COIN_DEBUG
// This is the method which prints out stub information. Used all over
// the place where there is functionality missing. Prototyped for the
// rest of the system in misc/SoBasic.h.
void
coin_stub(const char * filename, unsigned int linenr, const char * funcname)
{
  if (funcname) {
    fprintf(stderr,
            "STUB: %s not yet completed (at %s, line %u).\n",
            funcname, filename, linenr);
  }
  else {
    fprintf(stderr,
            "STUB: functionality not yet completed at %s, line %u.\n",
            filename, linenr);
  }
}
#else // !COIN_DEBUG
void coin_stub(const char *, unsigned int, const char *) { }
#endif // !COIN_DEBUG

/*************************************************************************/


/*!
  \class SoError Inventor/errors/SoError.h
  \brief The SoError class is the base class for all the error
  \brief handling classes.

  The SoError class handles both error handling and posting.
 
  The default handler just prints messages on the standard error
  output channel, but this can be overridden by client applications.

  Being able to override the default handler is useful when you want
  to collect error messages upon e.g. model import for later
  presentation of the messages to the user in any custom manner (like
  for instance in a GUI messagebox).

*/

/*¡
  Potensial buffer overflow errors, should be fixed - 19990610 larsa
*/

#include <Inventor/errors/SoErrors.h>

#include <Inventor/SoType.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/SoPath.h>
#include <Inventor/SbName.h>
#include <Inventor/engines/SoEngine.h>

#include <stdarg.h>
#include <assert.h>

/*!
  \var SoError::classTypeId

  FIXME: write doc.
*/

SoType SoError::classTypeId;

/*!
  \var SoError::callback

  FIXME: write doc.
*/

SoErrorCB * SoError::callback = SoError::defaultHandlerCB;

/*!
  \var SoError::callbackData

  FIXME: write doc.
*/

void * SoError::callbackData = NULL;

/*!
  This static method initializes static data for the SoError class.
*/

void
SoError::initClass(void)
{
  SoError::callback = defaultHandlerCB;
  SoError::callbackData = NULL;
  SoError::classTypeId =
    SoType::createType(SoType::badType(), SbName("Error"));
}

/*!
  This static method initializes all the SoError classes.
*/

void
SoError::initErrors(void)
{
  SoError::initClass();
  SoDebugError::initClass();
  SoMemoryError::initClass();
  SoReadError::initClass();
}

/*!
  This static method returns the SoType for the SoError class.
*/

SoType
SoError::getClassTypeId(void)
{
  return SoError::classTypeId;
}

/*!
  This method returns the type of the SoError (or SoError-derived) class
  instance.
*/

SoType
SoError::getTypeId(void) const
{
  return SoError::classTypeId;
}

/*!
  This method returns TRUE if the error instance is of - or derived from -
  \a type, and FALSE otherwise.
*/

SbBool
SoError::isOfType(const SoType type) const
{
  const SoType myType = getTypeId();
  return (myType == type) ? TRUE : myType.isDerivedFrom( type );
}

/*!
  This method sets the error handler callback for the SoError class type.
*/

void
SoError::setHandlerCallback(SoErrorCB * const function, void * const data)
{
    SoError::callback = function;
    SoError::callbackData = data;
}

/*!
  This method returns the error handler callback for the SoError class type.
*/

//$ EXPORT INLINE
SoErrorCB *
SoError::getHandlerCallback(void)
{
    return SoError::callback;
}

/*!
  This method returns the handler callback data for the SoError class type.
*/

//$ EXPORT INLINE
void *
SoError::getHandlerData(void)
{
    return SoError::callbackData;
}

/*!
  This method returns an SbString containing error info from the given
  error instance.
*/

//$ EXPORT INLINE
const SbString &
SoError::getDebugString(void) const
{
  return debugString;
}

/*!
  This method posts an error message.  The \a format argument and all the
  trailing aguments follows the printf() standard.
*/

void
SoError::post(const char * const format, // printf format
              ...)
{
  va_list args;
  va_start(args, format);
  char buffer[ 256 ]; // FIXME: buffer overrun alert. 19980906 mortene.
  vsprintf(buffer, format, args);
  SoError error;
  error.setDebugString(buffer);
  error.handleError();
  va_end(args);
}

/*!
  FIXME: write doc.
*/

SbString
SoError::getString(const SoNode * const node)
{
  SbString string;
  generateBaseString(string, node, "node");
  return string;
}

/*!
  FIXME: write doc.
*/

SbString
SoError::getString(const SoPath * const path)
{
  SbString string;
  generateBaseString(string, path, "path");
  return string;
}

/*!
  FIXME: write doc.
*/

SbString
SoError::getString(const SoEngine * const engine)
{
  SbString string;
  generateBaseString(string, engine, "engine");
  return string;
}

/*!
  FIXME: write doc.
*/

void
SoError::defaultHandlerCB(const SoError * error, void * /* data */)
{
  fprintf(stderr, "%s\n", error->getDebugString().getString());
}

/*!
  FIXME: write doc.
*/

SoErrorCB *
SoError::getHandler(void * & data) const
{
  data = SoError::callbackData;
  return SoError::callback;
}

/*!
  FIXME: write doc.
*/

//$ EXPORT INLINE
void
SoError::setDebugString(const char * const string)
{
  debugString = string;
}

/*!
  FIXME: write doc.
*/

//$ EXPORT INLINE
void
SoError::appendToDebugString(const char * const string)
{
    debugString += string;
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
  SoErrorCB * function = getHandler(arg);
  assert(function);
  (*function)(this, arg);
}

/*!
  This method is used by the getString methods.  It just generates a
  '<what> named "<name>" at address <address>' string.
*/

void
SoError::generateBaseString(SbString & string,
                            const SoBase * const base,
                            const char * const what)
{
  char buffer[120]; // FIXME: buffer overflow?  990610 larsa
  sprintf(buffer, "%s named \"%s\" at address %p",
      what, base->getName().getString(), base);
  string = buffer;
}

/*!
  This static method is just a wraparound for the SoError::initErrors()
  method.  It's here for OIV compatibility.  We like the name \a initErrors
  better.

  \sa void SoError::initErrors(void)
*/

//$ EXPORT INLINE
void
SoError::initClasses(void)
{
  initErrors();
}
