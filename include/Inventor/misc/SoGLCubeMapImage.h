#ifndef COIN_SOGLCUBEMAPIMAGE_H
#define COIN_SOGLCUBEMAPIMAGE_H

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

#include <Inventor/SbBasic.h>
#include <Inventor/misc/SoGLImage.h>

class COIN_DLL_API SoGLCubeMapImage : public SoGLImage {
  typedef SoGLImage inherited;

public:

  SoGLCubeMapImage();
  virtual void unref(SoState * state = NULL);

  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const ;

  enum Target {
    NEGATIVE_X = 0,
    POSITIVE_X,
    NEGATIVE_Y,
    POSITIVE_Y,
    NEGATIVE_Z,
    POSITIVE_Z,
  };

  void setCubeMapImage(const Target target,
                       const unsigned char * bytes,
                       const SbVec2s & size,
                       const int numcomponents);
  
  virtual void setData(const SbImage * image,
                       const Wrap wraps = REPEAT,
                       const Wrap wrapt = REPEAT,
                       const float quality = 0.5f,
                       const int border = 0,
                       SoState * createinstate = NULL);

  virtual void setData(const SbImage * image,
                       const Wrap wraps,
                       const Wrap wrapt,
                       const Wrap wrapr,
                       const float quality = 0.5f,
                       const int border = 0,
                       SoState * createinstate = NULL);

  virtual SoGLDisplayList * getGLDisplayList(SoState * state);


 public:
  static void initClass(void);

 private:
  virtual ~SoGLCubeMapImage();

  class SoGLCubeMapImageP * pimpl;
  friend class SoGLCubeMapImageP;
  static void cleanupClass(void);
};

#endif // !COIN_SOGLCUBEMAPIMAGE_H
