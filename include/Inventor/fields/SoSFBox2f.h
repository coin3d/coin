#ifndef COIN_SOSFBOX2F_H
#define COIN_SOSFBOX2F_H

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
#include <Inventor/SbBox2f.h>

class COIN_DLL_API SoSFBox2f : public SoSField {
  typedef SoSField inherited;

  SO_SFIELD_HEADER(SoSFBox2f, SbBox2f, const SbBox2f &);

public:
  static void initClass(void);

  void setValue(float xmin, float ymin, float xmax, float ymax);
  void setValue(const SbVec2f & minvec, const SbVec2f & maxvec);
  void getValue(SbBox2f & box) const;

}; // SoSFBox2f

#endif // !COIN_SOSFBOX2F_H
