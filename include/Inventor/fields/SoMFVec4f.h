#ifndef COIN_SOMFVEC4F_H
#define COIN_SOMFVEC4F_H

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
#include <Inventor/SbVec4f.h>

class COIN_DLL_API SoMFVec4f : public SoMField {
  typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFVec4f, SbVec4f, const SbVec4f &);

  SO_MFIELD_SETVALUESPOINTER_HEADER(SbVec4f);
  SO_MFIELD_SETVALUESPOINTER_HEADER(float);

public:
  static void initClass(void);

  void setValues(int start, int num, const float xyzw[][4]);
  void set1Value(int idx, float x, float y, float z, float w);
  void set1Value(int idx, const float xyzw[4]);
  void setValue(float x, float y, float z, float w);
  void setValue(const float xyzw[4]);

}; // SoMFVec4f

#endif // !COIN_SOMFVEC4F_H
