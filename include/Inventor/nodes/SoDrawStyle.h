#ifndef COIN_SODRAWSTYLE_H
#define COIN_SODRAWSTYLE_H

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

#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFUShort.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/nodes/SoSubNode.h>

class COIN_DLL_API SoDrawStyle : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoDrawStyle);

public:
  static void initClass(void);
  SoDrawStyle(void);

  enum Style {
    FILLED = SoDrawStyleElement::FILLED,
    LINES = SoDrawStyleElement::LINES,
    POINTS = SoDrawStyleElement::POINTS,
    INVISIBLE = SoDrawStyleElement::INVISIBLE
  };

  SoSFEnum style;
  SoSFFloat pointSize;
  SoSFFloat lineWidth;
  SoSFUShort linePattern;
  SoSFInt32 linePatternScaleFactor;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);

protected:
  virtual ~SoDrawStyle();
};

#endif // !COIN_SODRAWSTYLE_H
