#ifndef COIN_SOGLIMAGE_H
#define COIN_SOGLIMAGE_H

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

// WARNING: This is work in progress. Do not expect this class to have
// a stable interface over a long period of time. It is installed
// because we need it in an internal project.

// FIXME: make sure we do a design review for this class for Coin v3.0
// pederb, 2001-11-28

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec3s.h>
#include <Inventor/SoType.h>
#include <stddef.h>

class SoGLDisplayList;
class SoState;
class SbImage;

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
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER
  };

  enum ResizeReason {
    IMAGE = 0,
    SUBIMAGE,
    MIPMAP
  };

  typedef SbBool SoGLImageResizeCB(SoState * state,
                                   const SbVec3s &newsize,
                                   unsigned char * destbuffer,
                                   ResizeReason reason,
                                   void * closure,
                                   class SoGLImage * image);

  void setGLDisplayList(SoGLDisplayList * dl,
                        SoState * state,
                        const Wrap wraps = REPEAT,
                        const Wrap wrapt = REPEAT,
                        const float quality = 0.5f);

  void setPBuffer(SoState * state,
                  void * context,
                  const Wrap wraps = REPEAT,
                  const Wrap wrapt = REPEAT,
                  const float quality = 0.5f);

  void setData(const unsigned char * bytes,
               const SbVec2s & size,
               const int numcomponents,
               const Wrap wraps = REPEAT,
               const Wrap wrapt = REPEAT,
               const float quality = 0.5f,
               const int border = 0,
               SoState * createinstate = NULL);

  void setData(const unsigned char * bytes,
               const SbVec3s & size,
               const int numcomponents,
               const Wrap wraps = REPEAT,
               const Wrap wrapt = REPEAT,
               const Wrap wrapr = REPEAT,
               const float quality = 0.5f,
               const int border = 0,
               SoState * createinstate = NULL);

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

  // these flags can be used to set texture properties.
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

    INVINCIBLE                = 0x0200, // never die of old age

    // use GL_NV_texture_rectangle or GL_EXT_texture_rectangle
    RECTANGLE                 = 0x0400,

    // Compress texture if available from OpenGL
    COMPRESSED                = 0x0800,

    // use quality value to decide mipmap, filtering and scaling. This
    // is the default.
    USE_QUALITY_VALUE         = 0X8000
  };

  void setFlags(const uint32_t flags);
  uint32_t getFlags(void) const;

  const SbImage * getImage(void) const;

  virtual SoGLDisplayList * getGLDisplayList(SoState * state);
  SbBool hasTransparency(void) const;
  SbBool useAlphaTest(void) const;
  Wrap getWrapS(void) const;
  Wrap getWrapT(void) const;
  Wrap getWrapR(void) const;

  float getQuality(void) const;
  uint32_t getGLImageId(void) const;

protected:

  void incAge(void) const;
  void resetAge(void) const;
  virtual void unrefOldDL(SoState * state, const uint32_t maxage);
  virtual ~SoGLImage();

private:

  class SoGLImageP * pimpl;
  friend class SoGLImageP;
  static void cleanupClass(void);

public:
  // internal methods for texture resource management
  static void beginFrame(SoState * state);
  static void tagImage(SoState * state, SoGLImage * image);
  static void endFrame(SoState * state);
  static void setDisplayListMaxAge(const uint32_t maxage);
  static void freeAllImages(SoState * state = NULL);

  void setEndFrameCallback(void (*cb)(void *), void * closure);
  int getNumFramesSinceUsed(void) const;

public:
  static void initClass(void);
  static void setResizeCallback(SoGLImageResizeCB * f, void * closure);

private:
  static void registerImage(SoGLImage * image);
  static void unregisterImage(SoGLImage * image);
};

#endif // !COIN_SOGLIMAGE_H
