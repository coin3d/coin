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

#ifndef COIN_SOVRMLDIRECTIONALLIGHT_H
#define COIN_SOVRMLDIRECTIONALLIGHT_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLLight.h>
#include <Inventor/fields/SoSFVec3f.h>

class COIN_DLL_API SoVRMLDirectionalLight : public SoVRMLLight
{
  typedef SoVRMLLight inherited;
  SO_NODE_HEADER(SoVRMLDirectionalLight);

public:
  static void initClass(void);
  SoVRMLDirectionalLight(void);

  SoSFVec3f direction;

  virtual void GLRender( SoGLRenderAction * action );

protected:
  virtual ~SoVRMLDirectionalLight();

}; // class SoVRMLDirectionalLight

#endif // ! COIN_SOVRMLDIRECTIONALLIGHT_H
