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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOANNOTEXT3PROPERTY_H
#define COIN_SOANNOTEXT3PROPERTY_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBool.h>


class COIN_DLL_EXPORT SoAnnoText3Property : public SoNode {
    typedef SoNode inherited;

  SO_NODE_HEADER(SoAnnoText3Property);

public:
  static void initClass(void);
  SoAnnoText3Property(void);

  enum RenderPrintType {
    RENDER3D_PRINT_VECTOR, RENDER3D_PRINT_RASTER, RENDER2D_PRINT_RASTER
  };

  enum FontSizeHint {
    ANNOTATION, FIT_VECTOR_TEXT
  };

  SoSFEnum renderPrintType;
  SoSFBool isCharOrientedRasterPrint;
  SoSFEnum fontSizeHint;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoAnnoText3Property();
};

#endif // !COIN_SOANNOTEXT3PROPERTY_H
