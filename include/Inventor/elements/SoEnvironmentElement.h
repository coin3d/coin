/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef __SOENVIRONMENTELEMENT_H__
#define __SOENVIRONMENTELEMENT_H__

#include <Inventor/elements/SoReplacedElement.h>
#include <Inventor/SbColor.h>

#if defined(COIN_EXCLUDE_SOENVIRONMENTELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOENVIRONMENTELEMENT


class SoEnvironmentElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

//$ BEGIN TEMPLATE ElementHeader(SoEnvironmentElement)
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoEnvironmentElement(void);

  static void initClass(void);

protected:
  SoEnvironmentElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

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

#endif // !__SOENVIRONMENTELEMENT_H__
