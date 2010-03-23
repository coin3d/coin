#ifndef COIN_SOSHADOWSPOTLIGHT_H
#define COIN_SOSHADOWSPOTLIGHT_H

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
#include <Inventor/nodes/SoSpotLight.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFFloat.h>


class COIN_DLL_API SoShadowSpotLight : public SoSpotLight {
  typedef SoSpotLight inherited;

  SO_NODE_HEADER(SoShadowSpotLight);

public:
  static void initClass(void);
  SoShadowSpotLight(void);

  virtual void GLRender(SoGLRenderAction * action);

  SoSFNode shadowMapScene;
  SoSFFloat nearDistance;
  SoSFFloat farDistance;

protected:
  virtual ~SoShadowSpotLight();
};

#endif // !COIN_SOSHADOWSPOTLIGHT_H
