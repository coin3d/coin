#ifndef COIN_SOALPHATEST_H
#define COIN_SOALPHATEST_H

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
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodes/SoSubNode.h>

class COIN_DLL_API SoAlphaTest : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoAlphaTest);

public:
  static void initClass(void);
  SoAlphaTest(void);

  enum Function {
      NONE,
      NEVER,
      ALWAYS,
      LESS,
      LEQUAL,
      EQUAL,
      GEQUAL,
      GREATER,
      NOTEQUAL
  };

  SoSFEnum function;
  SoSFFloat value;

  virtual void GLRender(SoGLRenderAction * action);

protected:
  virtual ~SoAlphaTest();

}; // SoAlphaTest

#endif // !COIN_SOALPHATEST_H
