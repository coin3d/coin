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

#ifndef __SOMATERIALBUNDLE_H__
#define __SOMATERIALBUNDLE_H__

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOMATERIALBUNDLE)
#error "Configuration settings disrespected -- do not include this file!"
#endif // COIN_EXCLUDE_SOMATERIALBUNDLE

#include <Inventor/bundles/SoBundle.h>
#include <Inventor/SbBasic.h>
#include <Inventor/system/inttypes.h>

class SoGLAmbientColorElement;
class SoGLDiffuseColorElement;
class SoGLEmissiveColorElement;
class SoGLMaterialIndexElement;
class SoGLPolygonStippleElement;
class SoGLShininessElement;
class SoGLSpecularColorElement;
class SoTransparencyElement;

class SoMaterialBundle : public SoBundle {
  typedef SoBundle inherited;
public:
  SoMaterialBundle(SoAction *action);
  ~SoMaterialBundle();

  void setUpMultiple();
  void sendFirst();
  void send(const int index, const SbBool betweenBeginEnd);
  void forceSend(const int index);
  SbBool isColorOnly() const;

  void setPacked(const uint32_t *packed,
                 const int num);

private:

  void reallySend(const int index, const SbBool isBetweenBeginEnd);
  void setupElements(const SbBool betweenBeginEnd);

  SbBool doStipple;
  SbBool firstTime;
  SbBool colorOnly;
  SbBool diffuseOnly;
  int currIndex;

#if !defined(COIN_EXCLUDE_SOGLAMBIENTCOLORELEMENT)
  SoGLAmbientColorElement *ambientElt;
#endif // !COIN_EXCLUDE_SOGLAMBIENTCOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT)
  SoGLDiffuseColorElement *diffuseElt;
#endif // !COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLSPECULARCOLORELEMENT)
  SoGLSpecularColorElement *specularElt;
#endif // !COIN_EXCLUDE_SOGLSPECULARCOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLEMISSIVECOLORELEMENT)
  SoGLEmissiveColorElement *emissiveElt;
#endif // !COIN_EXCLUDE_SOGLEMISSIVECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLSHININESSELEMENT)
  SoGLShininessElement *shininessElt;
#endif // !COIN_EXCLUDE_SOGLSHININESSELEMENT
#if !defined(COIN_EXCLUDE_SOTRANSPARENCYELEMENT)
  SoTransparencyElement *transparencyElt;
#endif // !COIN_EXCLUDE_SOTRANSPARENCYELEMENT
#if !defined(COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT)
  SoGLPolygonStippleElement *stippleElt;
#endif // !COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT

  const uint32_t *packedColors;
  int numPacked;
  int currPacked;
};

#endif // !__SOMATERIALBUNDLE_H__
