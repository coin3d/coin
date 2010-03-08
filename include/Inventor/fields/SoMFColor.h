#ifndef COIN_SOMFCOLOR_H
#define COIN_SOMFCOLOR_H

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
#include <Inventor/SbColor.h>

class COIN_DLL_API SoMFColor : public SoMField {
  typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFColor, SbColor, const SbColor &);

  SO_MFIELD_SETVALUESPOINTER_HEADER(float);
  SO_MFIELD_SETVALUESPOINTER_HEADER(SbColor);

public:
  static void initClass(void);

  void setValues(int start, int num, const float rgb[][3]);
  void setHSVValues(int start, int num, const float hsv[][3]);

  void setValue(const SbVec3f & vec);
  void setValue(float r, float g, float b);
  void setValue(const float rgb[3]);

  void setHSVValue(float h, float s, float v);
  void setHSVValue(const float hsv[3]);

  void set1Value(int idx, const SbVec3f & vec);
  void set1Value(int idx, float r, float g, float b);
  void set1Value(int idx, const float rgb[3]);

  void set1HSVValue(int idx, float h, float s, float v);
  void set1HSVValue(int idx, const float hsv[3]);

}; // SoMFColor

#endif // !COIN_SOMFCOLOR_H
