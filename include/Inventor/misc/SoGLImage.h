/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOGLIMAGE_H
#define COIN_SOGLIMAGE_H

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SoType.h>

class SoGLDisplayList;
class SoState;

class COIN_DLL_API SoGLImage {
public:

  SoGLImage();
  virtual void unref(SoState * state = NULL);

  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const ;
  virtual SbBool isOfType(SoType type) const;

  enum Wrap {
    REPEAT = 0,
    CLAMP,
    CLAMP_TO_EDGE
  };

  virtual void setData(const unsigned char * bytes,
                       const SbVec2s size,
                       const int nc,
                       const Wrap wraps,
                       const Wrap wrapt,
                       const float quality = 0.5f,
                       const int border = 0,
                       SoState * createinstate = NULL);

  // these flags can be used to override using textureQuality to
  // decide some texture properties.
  enum Flags {
    // mipmap, scaling and filtering settings
    SCALE_DOWN =                0x0001,
    NO_MIPMAP =                 0x0002,
    LINEAR_MAG_FILTER =         0x0004,
    LINEAR_MIN_FILTER =         0x0008,
    LINEAR_MIPMAP_FILTER =      0x0010,

    // use if you know your image properties.
    FORCE_TRANSPARENCY_TRUE   = 0x0020,
    FORCE_TRANSPARENCY_FALSE  = 0x0040,
    FORCE_ALPHA_TEST_TRUE     = 0x0080,
    FORCE_ALPHA_TEST_FALSE    = 0x0100,

    // use quality value to decide mipmap, filtering and scaling. This
    // is the default.
    USE_QUALITY_VALUE         = 0X8000
  };

  void setFlags(const uint32_t flags);
  uint32_t getFlags(void) const;
  
  const unsigned char * getDataPtr(void) const;
  SbVec2s getSize(void) const;
  int getNumComponents(void) const;
  
  virtual SoGLDisplayList * getGLDisplayList(SoState * state);
  SbBool hasTransparency(void) const;
  SbBool useAlphaTest(void) const;
  Wrap getWrapS(void) const;
  Wrap getWrapT(void) const;

protected:

  virtual ~SoGLImage();
  
private:

  class SoGLImageP * pimpl;
  friend class SoGLImageP;
};

#endif // !COIN_SOGLIMAGE_H
