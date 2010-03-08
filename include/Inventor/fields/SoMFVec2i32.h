#ifndef COIN_SOMFVEC2I32_H
#define COIN_SOMFVEC2I32_H

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
#include <Inventor/SbVec2i32.h>

class COIN_DLL_API SoMFVec2i32 : public SoMField {
  typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFVec2i32, SbVec2i32, const SbVec2i32 &);

  SO_MFIELD_SETVALUESPOINTER_HEADER(SbVec2i32);
  SO_MFIELD_SETVALUESPOINTER_HEADER(int32_t);

public:
  static void initClass(void);

  void setValues(int start, int num, const int32_t xy[][2]);
  void set1Value(int idx, int32_t x, int32_t y);
  void set1Value(int idx, const int32_t xy[2]);
  void setValue(int32_t x, int32_t y);
  void setValue(const int32_t xy[2]);

}; // SoMFVec2i32

#endif // !COIN_SOMFVEC2I32_H
