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
  \class SoError Inventor/errors/SoError.h
  \brief The SoError class is the base class for all the error handling classes.

  The SoError class handles both error posting and error handling.  The
  default handler only prints an error message, but can be overridden by
  client applications.
*/

/*¡
  potensial buffer overflow errors detected, should be fixed - 990610 larsa
*/

#include <Inventor/errors/SoErrors.h>

#include <Inventor/SoType.h>
#include <Inventor/nodes/SoNode.h>
#if !defined(COIN_EXCLUDE_SOPATH)
#include <Inventor/SoPath.h>
#endif // !COIN_EXCLUDE_SOPATH
#include <Inventor/SbName.h>
#if !defined(COIN_EXCLUDE_SOENGINE)
#include <Inventor/engines/SoEngine.h>
#endif // !COIN_EXCLUDE_SOENGINE

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
SoError::initClass( void ) // static
{
    SoError::callback = defaultHandlerCB;
    SoError::callbackData = NULL;
    SoError::classTypeId =
        SoType::createType( SoType::badType(), SbName( "SoError" ) );
} // initClass()

/*!
  This static method cleans up static data for the SoError class.
*/

void
SoError::cleanClass( void ) // static
{
} // cleanClass()

/*!
  This static method initializes all the SoError classes.
*/

void
SoError::initErrors( void ) // static
{
    SoError::initClass();
    SoDebugError::initClass();
    SoMemoryError::initClass();
    SoReadError::initClass();
} // initErrors()

/*!
  This static method cleans up after all the SoError classes.
*/

void
SoError::cleanErrors( void ) // static
{
  SoReadError::cleanClass();
  SoMemoryError::cleanClass();
  SoDebugError::cleanClass();
  SoError::cleanClass();
} // cleanErrors()

/*!
  This static method returns the SoType for the SoError class.
*/

SoType
SoError::getClassTypeId( void ) // static
{
  return SoError::classTypeId;
} // getClassTypeId()

/*!
  This method returns the type of the SoError (or SoError-derived) class
  instance.
*/

SoType
SoError::getTypeId( void ) const // virtual
{
  return SoError::classTypeId;
} // getTypeId()

/*!
  This method returns TRUE if the error instance is of - or derived from -
  \a type, and FALSE otherwise.
*/

SbBool
SoError::isOfType(
  const SoType type ) const
{
  const SoType myType = getTypeId();
  return ( myType == type ) ? TRUE : myType.isDerivedFrom( type );
} // isOfType()

/*!
  This method sets the error handler callback for the SoError class type.
*/

void
SoError::setHandlerCallback( // static
    SoErrorCB * const function,
    void * const data )
{
    SoError::callback = function;
    SoError::callbackData = data;
} // setHandlerCallback()

/*!
  This method returns the error handler callback for the SoError class type.
*/

//$ EXPORT INLINE
SoErrorCB *
SoError::getHandlerCallback( void ) // static
{
    return SoError::callback;
} // getHandlerCallback()

/*!
  This method returns the handler callback data for the SoError class type.
*/

//$ EXPORT INLINE
void *
SoError::getHandlerData( void ) // static
{
    return SoError::callbackData;
} // getHandlerData()

/*!
  This method returns an SbString containing error info from the given
  error instance.
*/

//$ EXPORT INLINE
const SbString &
SoError::getDebugString( void ) const
{
  return debugString;
} // getDebugString()

/*!
  This method posts an error message.  The \a format argument and all the
  trailing aguments follows the printf() standard.
*/

void
SoError::post( // static
  const char * const format, // printf format
  ... )
{
  va_list args;
  va_start( args, format );
  char buffer[ 256 ]; // FIXME: buffer overrun alert. 19980906 mortene.
  vsprintf( buffer, format, args );
  SoError error;
  error.setDebugString( buffer );
  error.handleError();
  va_end( args );
} // post()

/*!
  FIXME: write doc.
*/

SbString
SoError::getString( // static
  const SoNode * const node )
{
  SbString string;
  generateBaseString( string, node, "node" );
  return string;
} // getString()

/*!
  FIXME: write doc.
*/

#if !defined(COIN_EXCLUDE_SOPATH)
SbString
SoError::getString( // static
  const SoPath * const path )
{
  SbString string;
  generateBaseString( string, path, "path" );
  return string;
} // getString()
#endif // !COIN_EXCLUDE_SOPATH

/*!
  FIXME: write doc.
*/

#if !defined(COIN_EXCLUDE_SOENGINE)
SbString
SoError::getString( // static
  const SoEngine * const engine )
{
  SbString string;
  generateBaseString( string, engine, "engine" );
  return string;
} // getString()
#endif // !COIN_EXCLUDE_SOENGINE

/*!
  FIXME: write doc.
*/

void
SoError::defaultHandlerCB( // static, protected
  const SoError * error,
  void * data)
{
  fprintf( stderr, "%s\n", error->getDebugString().getString() );
} // defaultHandlerCB()

/*!
  FIXME: write doc.
*/

SoErrorCB *
SoError::getHandler( // virtual
  void * & data ) const
{
  data = SoError::callbackData;
  return SoError::callback;
} // getHandler()

/*!
  FIXME: write doc.
*/

//$ EXPORT INLINE
void
SoError::setDebugString(
  const char * const string )
{
  debugString = string;
} // setDebugString()

/*!
  FIXME: write doc.
*/

//$ EXPORT INLINE
void
SoError::appendToDebugString(
    const char * const string )
{
    debugString += string;
} // appendToDebugString()

/*!
  This method calls the appropriate handler for an error instance.  All
  error handling goes through this method, and is therefore a good
  candidate for a debugger breakpoint.
*/

void
SoError::handleError(
  void )
{
  void * arg = NULL;
  SoErrorCB * function = getHandler( arg );
  assert(function);
  (*function)( this, arg );
} // handleError()

/*!
  This method is used by the getString methods.  It just generates a
  '<what> named "<name>" at address <address>' string.
*/

void
SoError::generateBaseString( // static
  SbString & string,
  const SoBase * const base,
  const char * const what )
{
  char buffer[120]; // FIXME: buffer overflow?  990610 larsa
  sprintf( buffer, "%s named \"%s\" at address %p",
      what, base->getName().getString(), base );
  string = buffer;
} // getBaseString()

/*!
  This static method is just a wraparound for the SoError::initErrors()
  method.  It's here for OIV compatibility.  We like the name \a initErrors
  better.

  \sa void SoError::initErrors( void )
*/

//$ EXPORT INLINE
void
SoError::initClasses( void )
{
  initErrors();
} // initClasses()

