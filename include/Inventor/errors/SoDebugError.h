/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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


class COIN_DLL_API SoDebugError : public SoError {
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

  static void post(const char * const source, const char * const format, ...);
  static void postWarning(const char * const source, const char * const format, ...);
  static void postInfo(const char * const source, const char * const format, ...);

  static void initClass(void);

protected:
  virtual SoErrorCB * getHandler(void * & data) const;

private:
  static SoType classTypeId;
  static SoErrorCB * callback;
  static void * callbackData;
  Severity severity;
};

// Avoid problem with Microsoft Win32 API headers (see above).
#if defined(SODEBUGERROR_STORE_ERROR_DEF)
#define ERROR SODEBUGERROR_STORE_ERROR_DEF
#undef SODEBUGERROR_STORE_ERROR_DEF
#endif /* SODEBUGERROR_STORE_ERROR_DEF */

#endif // !COIN_SODEBUGERROR_H
