#ifndef COIN_SOMFVEC3D_H
#define COIN_SOMFVEC3D_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbVec3d.h>

class COIN_DLL_API SoMFVec3d : public SoMField {
  typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFVec3d, SbVec3d, const SbVec3d &);

public:
  static void initClass(void);

  void setValues(const int start, const int num, const double xyz[][3]);
  void set1Value(const int idx, const double x, const double y, const double z);
  void set1Value(const int idx, const double xyz[3]);
  void setValue(const double x, const double y, const double z);
  void setValue(const double xyz[3]);
};

#endif // !COIN_SOMFVEC3D_H
