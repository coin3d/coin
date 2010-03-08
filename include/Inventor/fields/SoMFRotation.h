#ifndef COIN_SOMFROTATION_H
#define COIN_SOMFROTATION_H

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
#include <Inventor/SbRotation.h>

class COIN_DLL_API SoMFRotation : public SoMField {
  typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFRotation, SbRotation, const SbRotation &);

public:
  static void initClass(void);

  void setValues(const int start, const int num, const float q[][4]);
  void set1Value(const int idx, const float q0, const float q1, const float q2, const float q3);
  void set1Value(const int idx, const float q[4]);
  void set1Value(const int idx, const SbVec3f & axis, const float angle);
  void setValue(const float q0, const float q1, const float q2, const float q3);
  void setValue(const float q[4]);
  void setValue(const SbVec3f & axis, const float angle);
};

#endif // !COIN_SOMFROTATION_H
