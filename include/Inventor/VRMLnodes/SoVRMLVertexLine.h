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

#ifndef COIN_SOVRMLVERTEXLINE_H
#define COIN_SOVRMLVERTEXLINE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLGeometry.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/elements/SoMaterialBindingElement.h>

class COIN_DLL_API SoVRMLVertexLine : public SoVRMLGeometry  {
  typedef SoVRMLGeometry inherited;
  SO_NODE_ABSTRACT_HEADER(SoVRMLVertexLine);

public:
  static void initClass(void);

  SoSFNode coord;
  SoSFNode color;
  SoSFBool colorPerVertex;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void pick(SoPickAction * action);

  virtual void notify(SoNotList * list);

protected:
  SoVRMLVertexLine(void);
  virtual ~SoVRMLVertexLine();

  virtual SbBool shouldGLRender(SoGLRenderAction * action);

}; // class SoVRMLVertexLine

#endif // ! COIN_SOVRMLVERTEXLINE_H
