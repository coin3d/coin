/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

#ifndef COIN_SOMATERIALBUNDLE_H
#define COIN_SOMATERIALBUNDLE_H

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

class COIN_DLL_EXPORT SoMaterialBundle : public SoBundle {
  typedef SoBundle inherited;
public:
  SoMaterialBundle(SoAction *action);
  ~SoMaterialBundle();

  void setUpMultiple(void);
  void sendFirst(void);
  void send(const int index, const SbBool betweenBeginEnd);
  void forceSend(const int index);
  SbBool isColorOnly(void) const;

private:

  void reallySend(const int index, const SbBool isBetweenBeginEnd);
  void setupElements(const SbBool betweenBeginEnd);

  SbBool doStipple;
  SbBool firstTime;
  SbBool colorOnly;
  SbBool diffuseOnly;
  SbBool multiTrans;
  SbBool diffusePacked;
  int currIndex;

  SoGLAmbientColorElement *ambientElt;
  SoGLDiffuseColorElement *diffuseElt;
  SoGLSpecularColorElement *specularElt;
  SoGLEmissiveColorElement *emissiveElt;
  SoGLShininessElement *shininessElt;
  SoTransparencyElement *transparencyElt;
  SoGLPolygonStippleElement *stippleElt;
};

#endif // !COIN_SOMATERIALBUNDLE_H
