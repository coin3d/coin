#ifndef COIN_SODEPTHBUFFER_H
#define COIN_SODEPTHBUFFER_H

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

#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/elements/SoDepthBufferElement.h>

class COIN_DLL_API SoDepthBuffer : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoDepthBuffer);

public:
  static void initClass(void);
  SoDepthBuffer(void);

  enum DepthWriteFunction {
    NEVER = SoDepthBufferElement::NEVER,
    ALWAYS = SoDepthBufferElement::ALWAYS,
    LESS = SoDepthBufferElement::LESS,
    LEQUAL = SoDepthBufferElement::LEQUAL,
    EQUAL = SoDepthBufferElement::EQUAL,
    GEQUAL = SoDepthBufferElement::GEQUAL,
    GREATER = SoDepthBufferElement::GREATER,
    NOTEQUAL = SoDepthBufferElement::NOTEQUAL
  };

  SoSFBool test;
  SoSFBool write;
  SoSFEnum function;
  SoSFVec2f range;

  virtual void GLRender(SoGLRenderAction * action);

protected:
  virtual ~SoDepthBuffer();

}; // SoDepthBuffer

#endif // !COIN_SODEPTHBUFFER_H
