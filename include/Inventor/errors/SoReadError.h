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

#ifndef __SOREADERROR_H__
#define __SOREADERROR_H__

#include <Inventor/errors/SoError.h>

class SoInput;


class SoReadError : public SoError {
  typedef SoError inherited;

public:
  static void setHandlerCallback(SoErrorCB * const function,
                                 void * const data);
  static SoErrorCB * getHandlerCallback(void);
  static void * getHandlerData(void);

  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void post(const SoInput * const in, const char * const format, ...);

  static void initClass(void);

protected:
  virtual SoErrorCB * getHandler(void *& data) const;

private:
  static SoType classTypeId;
  static SoErrorCB * callback;
  static void * callbackData;
};

#endif // !__SOREADERROR_H__
