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

#ifndef COIN_SODEBUGERROR_H
#define COIN_SODEBUGERROR_H

#include <Inventor/errors/SoError.h>

// Avoid problem with Microsoft Win32 API headers (yes, they actually
// #define ERROR -- in wingdi.h).
#if defined(ERROR)
#define SODEBUGERROR_STORE_ERROR_DEF ERROR
#undef ERROR
#endif /* ERROR */


class SoDebugError : public SoError {
  typedef SoError inherited;

public:
  enum Severity { ERROR, WARNING, INFO };

  static void setHandlerCallback(SoErrorCB * const function,
                                 void * const data);
  static SoErrorCB * getHandlerCallback(void);
  static void * getHandlerData(void);

  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  SoDebugError::Severity getSeverity(void) const;

  static void post(const char * const type, const char * const format, ...);
  static void postWarning(const char * const type, const char * const format, ...);
  static void postInfo(const char * const type, const char * const format, ...);

  static void initClass(void);

protected:
  virtual SoErrorCB * getHandler(void * & data) const;

private:
  static void cleanClass(void);

  static SoType classTypeId;
  static SoErrorCB * callback;
  static void * callbackData;
  static char * strbuffer;
  static size_t strbuffersize;
  Severity severity;
};

// Avoid problem with Microsoft Win32 API headers (see above).
#if defined(SODEBUGERROR_STORE_ERROR_DEF)
#define ERROR SODEBUGERROR_STORE_ERROR_DEF
#undef SODEBUGERROR_STORE_ERROR_DEF
#endif /* SODEBUGERROR_STORE_ERROR_DEF */

#endif // !COIN_SODEBUGERROR_H
