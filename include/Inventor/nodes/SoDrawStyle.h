/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SODRAWSTYLE_H
#define COIN_SODRAWSTYLE_H

#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFUShort.h>
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

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);

protected:
  virtual ~SoDrawStyle();
};

#endif // !COIN_SODRAWSTYLE_H
