#ifndef COIN_SOTEXTURECOORDINATE2_H
#define COIN_SOTEXTURECOORDINATE2_H

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
#include <Inventor/fields/SoMFVec2f.h>

class SoTextureCoordinate2P;

class COIN_DLL_API SoTextureCoordinate2 : public SoNode {
  typedef SoNode inherited;
  SO_NODE_HEADER(SoTextureCoordinate2);

public:
  static void initClass(void);
  SoTextureCoordinate2(void);

  SoMFVec2f point;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void pick(SoPickAction * action);

protected:
  virtual ~SoTextureCoordinate2();

private:
  SoTextureCoordinate2P * pimpl;
};

#endif // !COIN_SOTEXTURECOORDINATE2_H
