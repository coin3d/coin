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

#ifndef COIN_SOVRMLFONTSTYLE_H
#define COIN_SOVRMLFONTSTYLE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/SbString.h>

#ifndef COIN_INTERNAL
#include <Inventor/actions/SoCallbackAction.h>
#endif // !COIN_INTERNAL

class SoVRMLFontStyleP;

class COIN_DLL_API SoVRMLFontStyle : public SoNode
{
  typedef SoNode inherited;
  SO_NODE_HEADER(SoVRMLFontStyle);

public:
  static void initClass(void);
  SoVRMLFontStyle(void);

  enum Family {
    SERIF,
    SANS,
    TYPEWRITER
  };

  enum Style {
    PLAIN      = 0x0,
    BOLD       = 0x1,
    ITALIC     = 0x2,
    BOLDITALIC = 0x3
  };

  SoSFFloat size;
  SoMFString family;
  SoMFString style;
  SoSFBool horizontal;
  SoSFBool leftToRight;
  SoSFBool topToBottom;
  SoSFString language;
  SoMFString justify;
  SoSFFloat spacing;

  SbString getFontName(void);

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoVRMLFontStyle();

private:
  SoVRMLFontStyleP * pimpl;
};

#endif // ! COIN_SOVRMLFONTSTYLE_H
