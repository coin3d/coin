#ifndef COIN_SOENVIRONMENTELEMENT_H
#define COIN_SOENVIRONMENTELEMENT_H

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

#include <Inventor/elements/SoReplacedElement.h>
#include <Inventor/SbColor.h>

class COIN_DLL_API SoEnvironmentElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

  SO_ELEMENT_HEADER(SoEnvironmentElement);
public:
  static void initClass(void);
protected:
  virtual ~SoEnvironmentElement();

public:

  enum FogType {
    NONE,
    HAZE,
    FOG,
    SMOKE
  };

  virtual void init(SoState * state);

  static  void set(SoState * const state, SoNode * const node,
                   const float ambientIntensity, const SbColor & ambientColor,
                   const SbVec3f & attenuation, const int32_t fogType,
                   const SbColor & fogColor, const float fogVisibility,
                   const float fogStart = 0.0f);
  static  void get(SoState * const state,
                   float & ambientIntensity, SbColor & ambientColor,
                   SbVec3f & attenuation, int32_t & fogType,
                   SbColor & fogColor, float & fogVisibility,
                   float & fogStart);

  static float getAmbientIntensity(SoState * const state);
  static float getFogVisibility(SoState * const state);
  static const SbVec3f & getLightAttenuation(SoState * const state);
  static const SbColor & getAmbientColor(SoState * const state);
  static const SbColor & getFogColor(SoState * const state);
  static int32_t getFogType(SoState * const state);

  static void getDefault(float & ambientIntensity,
                         SbColor & ambientColor, SbVec3f & attenuation,
                         int32_t & fogType, SbColor & fogColor,
                         float & fogVisibility, float & fogNear);

  virtual void print(FILE * file) const;

protected:

  virtual void setElt(SoState * const state,
                      const float ambientIntensity,
                      const SbColor & ambientColor,
                      const SbVec3f & attenuation,
                      const int32_t fogType,
                      const SbColor & fogColor,
                      const float fogVisibility,
                      const float fogNear);

  float ambientIntensity;
  SbColor ambientColor;
  SbVec3f attenuation;
  int32_t fogType;
  SbColor fogColor;
  float fogVisibility;
  float fogStart;
};

#endif // !COIN_SOENVIRONMENTELEMENT_H
