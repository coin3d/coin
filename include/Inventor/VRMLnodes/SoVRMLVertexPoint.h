/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifndef COIN_SOVRMLVERTEXPOINT_H
#define COIN_SOVRMLVERTEXPOINT_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLGeometry.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/elements/SoMaterialBindingElement.h>

class COIN_DLL_API SoVRMLVertexPoint : public SoVRMLGeometry
{
  typedef SoVRMLGeometry inherited;
  SO_NODE_ABSTRACT_HEADER(SoVRMLVertexPoint);

public:
  static void initClass(void);

  SoSFNode coord;
  SoSFNode color;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

  virtual void notify(SoNotList * list);

protected:
  SoVRMLVertexPoint(void);
  virtual ~SoVRMLVertexPoint();

  virtual SbBool shouldGLRender(SoGLRenderAction * action);
  virtual void computeBBox(SoAction * action, SbBox3f & box,
                           SbVec3f & center);
}; // class SoVRMLVertexPoint

#endif // ! COIN_SOVRMLVERTEXPOINT_H
