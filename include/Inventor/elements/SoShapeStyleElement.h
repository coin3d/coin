#ifndef COIN_SOSHAPESTYLEELEMENT_H
#define COIN_SOSHAPESTYLEELEMENT_H

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

#include <Inventor/elements/SoSubElement.h>

class COIN_DLL_API SoShapeStyleElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoShapeStyleElement);
public:
  static void initClass(void);
protected:
  virtual ~SoShapeStyleElement();

public:

  // flags used for optimized testing of features
  enum Flags {
    LIGHTING                = 0x00000100,
    TEXENABLED              = 0x00000200,
    TEXFUNC                 = 0x00000400,
    BBOXCMPLX               = 0x00000800,
    INVISIBLE               = 0x00001000,
    ABORTCB                 = 0x00002000,
    OVERRIDE                = 0x00004000,
    TEX3ENABLED             = 0x00008000,
    BIGIMAGE                = 0x00010000,
    BUMPMAP                 = 0x00020000,
    VERTEXARRAY             = 0x00040000,
    TRANSP_TEXTURE          = 0x00080000,
    TRANSP_MATERIAL         = 0x00100000,
    TRANSP_SORTED_TRIANGLES = 0x00200000,
    SHADOWMAP               = 0x00400000,
    SHADOWS                 = 0x00800000
  };
  
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state, const SoElement * prevTopElement);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo(void) const;

  static const SoShapeStyleElement * get(SoState * const state);

  unsigned int getFlags(void) const;
  SbBool mightNotRender(void) const;
  SbBool needNormals(void) const;
  SbBool needTexCoords(void) const;
  int getRenderCaseMask(void) const;

  static void setDrawStyle(SoState * const state, const int32_t value);
  static void setComplexityType(SoState * const state,
                                const int32_t value);
  static void setTransparencyType(SoState * const state,
                                  const int32_t value);
  static void setTextureEnabled(SoState * const state, const SbBool value);
  static void setTexture3Enabled(SoState * const state, const SbBool value);
  static void setTextureFunction(SoState * const state,
                                 const SbBool value);
  static void setLightModel(SoState * const state, const int32_t value);
  static void setOverrides(SoState * const state, const SbBool value);

  static SbBool isScreenDoor(SoState * const state);
  static int getTransparencyType(SoState * const state);
  SbBool isTextureFunction(void) const;

  static void setBumpmapEnabled(SoState * state, const SbBool value);
  static void setBigImageEnabled(SoState * state, const SbBool value);
  static void setVertexArrayRendering(SoState * state, const SbBool value);

  static void setTransparentMaterial(SoState * state, const SbBool value);
  static void setTransparentTexture(SoState * state, const SbBool value);

  static void setShadowMapRendering(SoState * state, const SbBool value);
  static void setShadowsRendering(SoState * state, const SbBool value);

private:

  static SoShapeStyleElement * getElement(SoState * const state);
  static const SoShapeStyleElement * getConstElement(SoState * const state);

  unsigned int flags;
};

#endif // !COIN_SOSHAPESTYLEELEMENT_H
