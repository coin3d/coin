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

#ifndef COIN_SOHEIGHTMAPTONORMALMAP_H
#define COIN_SOHEIGHTMAPTONORMALMAP_H

#include <Inventor/SbVec2s.h>
#include <Inventor/engines/SoTexture2Convert.h>
#include <Inventor/fields/SoSFEnum.h>

class SbImage;

class COIN_DLL_API SoHeightMapToNormalMap : public SoTexture2Convert {
  typedef SoTexture2Convert inherited;
  SO_ENGINE_HEADER(SoHeightMapToNormalMap);

public:
  static void initClass(void);
  SoHeightMapToNormalMap(void);

  enum NormalMapFormat {
    INT8
  };

  SoSFEnum format;

  static void convert(const unsigned char * srcptr, SbVec2s size, int nc, SbImage & dst_out);

protected:
  virtual void inputChanged(SoField * which);
  virtual void evaluate(void);

}; // SoHeightMapToNormalMap

#endif // !COIN_SOHEIGHTMAPTONORMALMAP_H
