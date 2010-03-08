#ifndef COIN_SOSFVEC4B_H
#define COIN_SOSFVEC4B_H

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

#include <Inventor/fields/SoSField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbVec4b.h>

class COIN_DLL_API SoSFVec4b : public SoSField {
  typedef SoSField inherited;

  SO_SFIELD_HEADER(SoSFVec4b, SbVec4b, SbVec4b);

public:
  static void initClass(void);

  void setValue(int8_t x, int8_t y, int8_t z, int8_t w);
  void setValue(const int8_t xyzw[4]);

}; // SoSFVec4b

#endif // !COIN_SOSFVEC4B_H
