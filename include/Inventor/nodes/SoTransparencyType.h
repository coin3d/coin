#ifndef COIN_SOTRANSPARENCYTYPE_H
#define COIN_SOTRANSPARENCYTYPE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/actions/SoGLRenderAction.h>

class COIN_DLL_API SoTransparencyType : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoTransparencyType);

public:
  static void initClass(void);
  SoTransparencyType(void);

  enum Type {
    SCREEN_DOOR = SoGLRenderAction::SCREEN_DOOR,
    ADD = SoGLRenderAction::ADD,
    DELAYED_ADD = SoGLRenderAction::DELAYED_ADD,
    SORTED_OBJECT_ADD = SoGLRenderAction::SORTED_OBJECT_ADD,
    BLEND = SoGLRenderAction::BLEND,
    DELAYED_BLEND = SoGLRenderAction::DELAYED_BLEND,
    SORTED_OBJECT_BLEND = SoGLRenderAction::SORTED_OBJECT_BLEND,
    SORTED_OBJECT_SORTED_TRIANGLE_ADD =
    SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_ADD,
    SORTED_OBJECT_SORTED_TRIANGLE_BLEND =
    SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_BLEND,
    NONE = SoGLRenderAction::NONE
  };

  SoSFEnum value;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);

protected:
  virtual ~SoTransparencyType();
};

#endif // !COIN_SOTRANSPARENCYTYPE_H
