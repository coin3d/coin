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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef _SO_DEBUG_ERROR_H_
#define _SO_DEBUG_ERROR_H_

#include <Inventor/errors/SoError.h>

class SoDebugError : public SoError {
  typedef SoError inherited;

public:
  enum Severity {
    ERROR = 0,
    WARNING = 1,
    INFO = 2
  }; // enum Severity

  static void setHandlerCallback( SoErrorCB * const function,
      void * const data );
  static SoErrorCB * getHandlerCallback( void );
  static void * getHandlerData( void );

  static SoType getClassTypeId( void );
  virtual SoType getTypeId( void ) const;

  SoDebugError::Severity getSeverity( void ) const;

  static void post( const char * const methodName,
      const char * const formatString, ... );
  static void postWarning( const char * const methodName,
      const char * const formatString, ... );
  static void postInfo( const char * const methodName,
      const char * const formatString, ... );

  static void initClass( void );
  static void cleanClass( void );

protected:
  virtual SoErrorCB * getHandler( void * & data ) const;

private:
  static SoType classTypeId;
  static SoErrorCB * callback;
  static void * callbackData;

  Severity severity;
}; // class SoDebugError

#endif // ! _SO_DEBUG_ERROR_H_
