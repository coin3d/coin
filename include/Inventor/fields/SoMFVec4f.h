#ifndef COIN_SOMFVEC4F_H
#define COIN_SOMFVEC4F_H

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

#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbVec4f.h>

class COIN_DLL_API SoMFVec4f : public SoMField {
  typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFVec4f, SbVec4f, const SbVec4f &);

  SO_MFIELD_SETVALUESPOINTER_HEADER(float);
  SO_MFIELD_SETVALUESPOINTER_HEADER(SbVec4f);

public:
  static void initClass(void);

  void setValues(const int start, const int num, const float xyzw[][4]);
  void set1Value(const int idx,
                 const float x, const float y, const float z, const float w);
  void set1Value(const int idx, const float xyzw[4]);
  void setValue(const float x, const float y, const float z, const float w);
  void setValue(const float xyzw[4]);
};

#endif // !COIN_SOMFVEC4F_H
