/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOVRMLFONTSTYLE_H
#define COIN_SOVRMLFONTSTYLE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/SbString.h>

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

  virtual void doAction( SoAction * action );
  virtual void callback( SoCallbackAction * action );
  virtual void GLRender( SoGLRenderAction * action );
  virtual void getBoundingBox( SoGetBoundingBoxAction * action );
  virtual void pick( SoRayPickAction * action );
  virtual void getPrimitiveCount( SoGetPrimitiveCountAction * action );

protected:
  virtual ~SoVRMLFontStyle();

}; // class SoVRMLFontStyle

#endif // ! COIN_SOVRMLFONTSTYLE_H
