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

#ifndef __SODEBUGERROR_H__
#define __SODEBUGERROR_H__

#include <Inventor/errors/SoError.h>

class SoDebugError : public SoError {
  typedef SoError inherited;

public:
  enum Severity {
#if defined(ERROR) // Problem with MS Visual C++ v5.0 (at least)
    SERROR = 0,
#else
    ERROR = 0,
#endif
#if defined(WARNING) // FIXME: not sure if this is necessary. 19990808 mortene.
    SWARNING = 1,
#else
    WARNING = 1,
#endif
    INFO = 2
  };

  static void setHandlerCallback(SoErrorCB * const function,
                                 void * const data);
  static SoErrorCB * getHandlerCallback(void);
  static void * getHandlerData(void);

  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  SoDebugError::Severity getSeverity(void) const;

  static void post(const char * const methodName,
                   const char * const formatString, ...);
  static void postWarning(const char * const methodName,
                          const char * const formatString, ...);
  static void postInfo(const char * const methodName,
                       const char * const formatString, ...);

  static void initClass(void);

protected:
  virtual SoErrorCB * getHandler(void * & data) const;

private:
  static SoType classTypeId;
  static SoErrorCB * callback;
  static void * callbackData;

  Severity severity;
};

#endif // !__SODEBUGERROR_H__
