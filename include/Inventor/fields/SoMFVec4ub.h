#ifndef COIN_SOMFVEC4UB_H
#define COIN_SOMFVEC4UB_H

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

#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbVec4ub.h>

class COIN_DLL_API SoMFVec4ub : public SoMField {
  typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFVec4ub, SbVec4ub, SbVec4ub);

  SO_MFIELD_SETVALUESPOINTER_HEADER(SbVec4ub);
  SO_MFIELD_SETVALUESPOINTER_HEADER(uint8_t);

public:
  static void initClass(void);

  void setValues(int start, int num, const uint8_t xyzw[][4]);
  void set1Value(int idx, uint8_t x, uint8_t y, uint8_t z, uint8_t w);
  void set1Value(int idx, const uint8_t xyzw[4]);
  void setValue(uint8_t x, uint8_t y, uint8_t z, uint8_t w);
  void setValue(const uint8_t xyzw[4]);

}; // SoMFVec4ub

#endif // !COIN_SOMFVEC4UB_H
