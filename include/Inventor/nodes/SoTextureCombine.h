#ifndef COIN_SOTEXTURECOMBINE_H
#define COIN_SOTEXTURECOMBINE_H

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

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFVec4f.h>
#include <Inventor/elements/SoTextureCombineElement.h>

class COIN_DLL_API SoTextureCombine : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoTextureCombine);

public:
  static void initClass(void);
  SoTextureCombine(void);

  enum Source {
    PRIMARY_COLOR = SoTextureCombineElement::PRIMARY_COLOR,
    TEXTURE = SoTextureCombineElement::TEXTURE,
    CONSTANT = SoTextureCombineElement::CONSTANT,
    PREVIOUS = SoTextureCombineElement::PREVIOUS
  };
  enum Operand {
    SRC_COLOR = SoTextureCombineElement::SRC_COLOR,
    ONE_MINUS_SRC_COLOR = SoTextureCombineElement::ONE_MINUS_SRC_COLOR,
    SRC_ALPHA = SoTextureCombineElement::SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA = SoTextureCombineElement::ONE_MINUS_SRC_ALPHA
  };
  enum Operation {
    REPLACE = SoTextureCombineElement::REPLACE,
    MODULATE = SoTextureCombineElement::MODULATE,
    ADD = SoTextureCombineElement::ADD,
    ADD_SIGNED = SoTextureCombineElement::ADD_SIGNED,
    SUBTRACT = SoTextureCombineElement::SUBTRACT,
    INTERPOLATE = SoTextureCombineElement::INTERPOLATE,
    DOT3_RGB = SoTextureCombineElement::DOT3_RGB,
    DOT3_RGBA = SoTextureCombineElement::DOT3_RGBA
  };
  
  SoMFEnum rgbSource;
  SoMFEnum alphaSource;

  SoMFEnum rgbOperand;
  SoMFEnum alphaOperand;

  SoSFEnum rgbOperation;
  SoSFEnum alphaOperation;
  
  SoSFFloat rgbScale;
  SoSFFloat alphaScale;

  SoSFVec4f constantColor;

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void pick(SoPickAction * action);

protected:
  virtual ~SoTextureCombine();
};

#endif // !COIN_SOTEXTURECOMBINE_H
