/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOLIGHTMODEL_H
#define COIN_SOLIGHTMODEL_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/elements/SoLightModelElement.h>


class COIN_DLL_API SoLightModel : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoLightModel);

public:
  static void initClass(void);
  SoLightModel(void);

  enum Model {
    BASE_COLOR = SoLightModelElement::BASE_COLOR,
    PHONG = SoLightModelElement::PHONG
  };

  SoSFEnum model;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);

protected:
  virtual ~SoLightModel();
};

#endif // !COIN_SOLIGHTMODEL_H
