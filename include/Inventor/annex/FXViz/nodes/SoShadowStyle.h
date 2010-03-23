#ifndef COIN_SOSHADOWSTYLE_H
#define COIN_SOSHADOWSTYLE_H

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
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/annex/FXViz/elements/SoShadowStyleElement.h>

class COIN_DLL_API SoShadowStyle : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoShadowStyle);

public:
  static void initClass(void);
  SoShadowStyle(void);

  enum Style {
    NO_SHADOWING = SoShadowStyleElement::NO_SHADOWING, 
    CASTS_SHADOW = SoShadowStyleElement::CASTS_SHADOW, 
    SHADOWED = SoShadowStyleElement::SHADOWED, 
    CASTS_SHADOW_AND_SHADOWED = SoShadowStyleElement::CASTS_SHADOW_AND_SHADOWED 
  };
  
  SoSFEnum style;

  virtual void GLRender(SoGLRenderAction * action);

protected:
  virtual ~SoShadowStyle();
};

#endif // !COIN_SOSHADOWSTYLE_H
