#ifndef COIN_SOPICKSTYLE_H
#define COIN_SOPICKSTYLE_H

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
#include <Inventor/elements/SoPickStyleElement.h>

class COIN_DLL_API SoPickStyle : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoPickStyle);

public:
  static void initClass(void);
  SoPickStyle(void);

  enum Style {
    SHAPE = SoPickStyleElement::SHAPE,
    BOUNDING_BOX = SoPickStyleElement::BOUNDING_BOX,
    UNPICKABLE = SoPickStyleElement::UNPICKABLE,
    SHAPE_ON_TOP = SoPickStyleElement::SHAPE_ON_TOP,
    BOUNDING_BOX_ON_TOP = SoPickStyleElement::BOUNDING_BOX_ON_TOP,
    SHAPE_FRONTFACES = SoPickStyleElement::SHAPE_FRONTFACES
  };

  SoSFEnum style;

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void pick(SoPickAction * action);

protected:
  virtual ~SoPickStyle();
};

#endif // !COIN_SOPICKSTYLE_H
