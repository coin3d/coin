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
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
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
class SoGLColorIndexElement;

class COIN_DLL_API SoMaterialBundle : public SoBundle {
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
  float firstAlpha;
  int currIndex;

  SoGLAmbientColorElement * ambientElt;
  SoGLDiffuseColorElement * diffuseElt;
  SoGLSpecularColorElement * specularElt;
  SoGLEmissiveColorElement * emissiveElt;
  SoGLShininessElement * shininessElt;
  SoTransparencyElement * transparencyElt;
  SoGLPolygonStippleElement * stippleElt;
  SoGLColorIndexElement * indexElt;
};

#endif // !COIN_SOMATERIALBUNDLE_H
