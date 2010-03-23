#ifndef COIN_SOSFBOX2I32_H
#define COIN_SOSFBOX2I32_H

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
#include <Inventor/SbBox2i32.h>

class COIN_DLL_API SoSFBox2i32 : public SoSField {
  typedef SoSField inherited;

  SO_SFIELD_HEADER(SoSFBox2i32, SbBox2i32, const SbBox2i32 &);

public:
  static void initClass(void);

  void setValue(int32_t xmin, int32_t ymin, int32_t xmax, int32_t ymax);
  void setValue(const SbVec2i32 & minvec, const SbVec2i32 & maxvec);
  void getValue(SbBox2i32 & box) const;

}; // SoSFBox2i32

#endif // !COIN_SOSFBOX2I32_H
