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
  \class SoMemoryError Inventor/elements/SoMemoryError.h
  \brief The SoMemoryError class is yet to be documented.

  FIXME: write doc.
*/

/*¡
  potensial buffer overflow errors detected, should be fixed - 990610 larsa
*/

#include <Inventor/errors/SoMemoryError.h>

#include <Inventor/SoType.h>
#include <Inventor/SbName.h>

#include <stdio.h>

/*!
  \var SoMemoryError::classTypeId

  FIXME: write doc.
*/

SoType SoMemoryError::classTypeId;

/*!
  \var SoMemoryError::callback

  FIXME: write doc.
*/

SoErrorCB * SoMemoryError::callback = SoError::defaultHandlerCB;

/*!
  \var SoMemoryError::callbackData

  FIXME: write doc.
*/

void * SoMemoryError::callbackData = NULL;

/*!
  This static method initializes static data for the SoMemoryError class.
*/

void
SoMemoryError::initClass( void ) // static
{
  SoMemoryError::callback = SoError::defaultHandlerCB;
  SoMemoryError::callbackData = NULL;
  SoMemoryError::classTypeId =
    SoType::createType( SoError::getClassTypeId(), "SoMemoryError" );
} // initClass()

/*!
  This static method cleans up static data for the SoMemoryError class.
*/

void
SoMemoryError::cleanClass( void ) // static
{
} // cleanClass()

/*!
  FIXME: write doc.
*/

SoType
SoMemoryError::getClassTypeId( void )  // static
{
  return SoMemoryError::classTypeId;
} // getClassTypeId()

/*!
  FIXME: write doc.
*/

SoType
SoMemoryError::getTypeId( void ) const // virtual
{
  return SoMemoryError::classTypeId;
} // getTypeId()

/*!
  FIXME: write doc.
*/

void
SoMemoryError::setHandlerCallback( // static
  SoErrorCB * const function,
  void * const data )
{
  SoMemoryError::callback = function;
  SoMemoryError::callbackData = data;
} // setHandlerCallback()

/*!
  FIXME: write doc.
*/

SoErrorCB *
SoMemoryError::getHandlerCallback( void ) // static
{
  return SoMemoryError::callback;
} // getHandlerCallback()

/*!
  FIXME: write doc.
*/

void *
SoMemoryError::getHandlerData( void ) // static
{
  return SoMemoryError::callbackData;
} // getHandlerData()

/*!
  FIXME: write doc.
*/

void
SoMemoryError::post( // static
  const char * const whatWasAllocated )
{
  char buffer[ 128 ]; // FIXME: possible overflow, 990610 larsa
  sprintf( buffer, "ERROR allocating '%s'.", whatWasAllocated );
  SoMemoryError error;
  error.setDebugString( buffer );
  error.handleError();
} // post()

/*!
  FIXME: write doc.
*/

SoErrorCB *
SoMemoryError::getHandler( // virtual
  void * & data ) const
{
  data = SoMemoryError::callbackData;
  return SoMemoryError::callback;
} // getHandler()

