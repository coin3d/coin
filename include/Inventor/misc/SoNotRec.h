#ifndef COIN_SONOTREC_H
#define COIN_SONOTREC_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/SbBasic.h>
#include <stdio.h>

class SoBase;


class COIN_DLL_API SoNotRec {
public:
  enum Type {
    CONTAINER,
    PARENT,
    SENSOR,
    FIELD,
    ENGINE
  };

  SoNotRec(SoBase * const notifbase);
  void setType(const SoNotRec::Type type);
  SoBase * getBase(void) const;
  SoNotRec::Type getType(void) const;
  const SoNotRec * getPrevious(void) const;
  void setPrevious(const SoNotRec * const prev);
  void print(FILE * const file) const;

private:
  Type type;
  SoBase * base;
  const SoNotRec * prev;
};

#endif // !COIN_SONOTREC_H
