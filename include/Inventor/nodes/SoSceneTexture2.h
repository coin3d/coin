#ifndef COIN_SOSCENETEXTURE2_H
#define COIN_SOSCENETEXTURE2_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#include <Inventor/fields/SoSFVec2s.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFVec4f.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/elements/SoTextureImageElement.h>

class SoSceneTexture2P;

class COIN_DLL_API SoSceneTexture2 : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoSceneTexture2);

public:
  static void initClass(void);
  SoSceneTexture2(void);

  enum Model {
    MODULATE = SoTextureImageElement::MODULATE,
    DECAL = SoTextureImageElement::DECAL,
    BLEND = SoTextureImageElement::BLEND,
    REPLACE = SoTextureImageElement::REPLACE
  };

  enum Wrap {
    REPEAT = SoTextureImageElement::REPEAT,
    CLAMP = SoTextureImageElement::CLAMP
  };

  enum TransparencyFunction {
    NONE,
    ALPHA_BLEND,
    ALPHA_TEST
  };

  SoSFEnum wrapS;
  SoSFEnum wrapT;
  SoSFEnum model;
  SoSFColor blendColor;

  SoSFVec4f backgroundColor;
  SoSFVec2s size;
  SoSFNode scene;
  SoSFEnum transparencyFunction;

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
