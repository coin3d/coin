#ifndef COIN_SOMFCOLORRGBA_H
#define COIN_SOMFCOLORRGBA_H

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
#include <Inventor/SbColor4f.h>

class COIN_DLL_API SoMFColorRGBA : public SoMField {
  typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFColorRGBA, SbColor4f, const SbColor4f &);

  SO_MFIELD_SETVALUESPOINTER_HEADER(float);
  SO_MFIELD_SETVALUESPOINTER_HEADER(SbColor4f);

public:
  static void initClass(void);

  void setValues(int start, int num, const float rgba[][4]);
  void setHSVValues(int start, int num, const float hsv[][4]);

  void setValue(const SbVec4f & vec);
  void setValue(float r, float g, float b, float a);
  void setValue(const float rgba[4]);

  void setHSVValue(float h, float s, float v, float a);
  void setHSVValue(const float hsva[4]);

  void set1Value(int idx, const SbVec4f & vec);
  void set1Value(int idx, float r, float g, float b, float a);
  void set1Value(int idx, const float rgba[4]);

  void set1HSVValue(int idx, float h, float s, float v, float a);
  void set1HSVValue(int idx, const float hsva[4]);

}; // SoMFColorRGBA

#endif // !COIN_SOMFCOLORRGBA_H
