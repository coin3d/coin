/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOENVIRONMENTELEMENT_H
#define COIN_SOENVIRONMENTELEMENT_H

#include <Inventor/elements/SoReplacedElement.h>
#include <Inventor/SbColor.h>


class COIN_DLL_EXPORT SoEnvironmentElement : public SoReplacedElement {
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
                   const SbColor & fogColor, const float fogVisibility);
  static  void get(SoState * const state,
                   float & ambientIntensity, SbColor & ambientColor,
                   SbVec3f & attenuation, int32_t & fogType,
                   SbColor & fogColor, float & fogVisibility);

  static float getAmbientIntensity(SoState * const state);
  static float getFogVisibility(SoState * const state);
  static const SbVec3f & getLightAttenuation(SoState * const state);
  static const SbColor & getAmbientColor(SoState * const state);
  static const SbColor & getFogColor(SoState * const state);
  static int32_t getFogType(SoState * const state);

  static void getDefault(float & ambientIntensity,
                         SbColor & ambientColor, SbVec3f & attenuation,
                         int32_t & fogType, SbColor & fogColor,
                         float & fogVisibility);

  virtual void print(FILE * file) const;

protected:

  virtual void setElt(SoState * const state,
                      const float ambientIntensity,
                      const SbColor & ambientColor,
                      const SbVec3f & attenuation,
                      const int32_t fogType,
                      const SbColor & fogColor,
                      const float fogVisibility);

  float ambientIntensity;
  SbColor ambientColor;
  SbVec3f attenuation;
  int32_t fogType;
  SbColor fogColor;
  float fogVisibility;

};

#endif // !COIN_SOENVIRONMENTELEMENT_H
