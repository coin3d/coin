#ifndef COIN_SOSCENETEXTURE2_H
#define COIN_SOSCENETEXTURE2_H

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

#include <Inventor/fields/SoSFVec2s.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFVec4f.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/elements/SoMultiTextureImageElement.h>

class SoSceneTexture2P;

class COIN_DLL_API SoSceneTexture2 : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoSceneTexture2);

public:
  static void initClass(void);
  SoSceneTexture2(void);

  enum Model {
    MODULATE = SoMultiTextureImageElement::MODULATE,
    DECAL = SoMultiTextureImageElement::DECAL,
    BLEND = SoMultiTextureImageElement::BLEND,
    REPLACE = SoMultiTextureImageElement::REPLACE
  };

  enum Wrap {
    REPEAT = SoMultiTextureImageElement::REPEAT,
    CLAMP = SoMultiTextureImageElement::CLAMP,
    CLAMP_TO_BORDER = SoMultiTextureImageElement::CLAMP_TO_BORDER
  };

  enum TransparencyFunction {
    NONE,
    ALPHA_BLEND,
    ALPHA_TEST
  };

  enum Type {
    DEPTH,
    RGBA8, // normal unsigned byte rgba
    RGBA32F,
    RGB32F,
    RGBA16F,
    RGB16F,

    // FIXME: consider how many of these we should have here
    R3_G3_B2,
    RGB,
    RGB4,
    RGB5,
    RGB8,
    RGB10,
    RGB12,
    RGB16,
    RGBA,
    RGBA2,
    RGBA4,
    RGB5_A1,
    RGB10_A2,
    RGBA12,
    RGBA16
  };

  SoSFEnum wrapS;
  SoSFEnum wrapT;
  SoSFEnum model;
  SoSFColor blendColor;

  SoSFVec4f backgroundColor;
  SoSFVec2s size;
  SoSFNode scene;
  SoSFNode sceneTransparencyType;
  SoSFEnum transparencyFunction;

  SoSFEnum type;

  virtual void notify(SoNotList * list);
  virtual void write(SoWriteAction * action);

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void rayPick(SoRayPickAction * action);

protected:
  virtual ~SoSceneTexture2(void);

private:
  SoSceneTexture2P * pimpl;
};

#endif // !COIN_SOSCENETEXTURE2_H
