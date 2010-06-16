#ifndef COIN_SOMEMORYERROR_H
#define COIN_SOMEMORYERROR_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/errors/SoError.h>

class COIN_DLL_API SoMemoryError : public SoError {
  typedef SoError inherited;

public:
  static void setHandlerCallback(SoErrorCB * const callback,
                                 void * const data);
  static SoErrorCB * getHandlerCallback(void);
  static void * getHandlerData(void);

  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void post(const char * const whatWasAllocated);

  static void initClass(void);

protected:
  virtual SoErrorCBPtr getHandler(void * & data) const;

private:
  static SoType classTypeId;
  static SoErrorCB * callback;
  static void * callbackData;
};

#endif // !COIN_SOMEMORYERROR_H
