#ifndef COIN_SOSCENETEXTURECUBEMAP_H
#define COIN_SOSCENETEXTURECUBEMAP_H

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

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFVec2s.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/elements/SoMultiTextureImageElement.h>

class SoSensor;
class SoSceneTextureCubeMapP;

class COIN_DLL_API SoSceneTextureCubeMap : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoSceneTextureCubeMap);

public:
  static void initClass(void);
  SoSceneTextureCubeMap(void);

  enum Model {
    MODULATE = SoMultiTextureImageElement::MODULATE,
    DECAL = SoMultiTextureImageElement::DECAL,
    BLEND = SoMultiTextureImageElement::BLEND,
    REPLACE = SoMultiTextureImageElement::REPLACE
  };

  enum Wrap {
    REPEAT = SoMultiTextureImageElement::REPEAT,
    CLAMP = SoMultiTextureImageElement::CLAMP
  };

  enum TransparencyFunction {
    NONE,
    ALPHA_BLEND,
    ALPHA_TEST
  };

  SoSFVec2s size;
  SoSFNode scene;

  SoSFEnum wrapS;
  SoSFEnum wrapT;
  SoSFEnum wrapR;
  SoSFEnum model;
  SoSFColor backgroundColor;

  SoSFEnum transparencyFunction;
  SoSFColor blendColor;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void rayPick(SoRayPickAction * action);

protected:
  virtual ~SoSceneTextureCubeMap();

  virtual void notify(SoNotList * list);

private:
  SoSceneTextureCubeMapP * pimpl;
};

#endif // !COIN_SOSCENETEXTURECUBEMAP_H
