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

#ifndef COIN_SOVRMLBILLBOARD_H
#define COIN_SOVRMLBILLBOARD_H

#include <Inventor/VRMLnodes/SoVRMLParent.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFVec3f.h>

class SbRotation;
class SoState;

class COIN_DLL_API SoVRMLBillboard : public SoVRMLParent
{
  typedef SoVRMLParent inherited;
  SO_NODE_HEADER(SoVRMLBillboard);

public:
  static void initClass(void);
  SoVRMLBillboard(void);
  SoVRMLBillboard( int numchildren );

  SoSFVec3f axisOfRotation;
  SoSFVec3f bboxCenter;
  SoSFVec3f bboxSize;

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void pick(SoPickAction * action);
  virtual void search(SoSearchAction * action);

  virtual void GLRenderBelowPath(SoGLRenderAction * action);
  virtual void GLRenderInPath(SoGLRenderAction * action);
  virtual void GLRenderOffPath(SoGLRenderAction * action);

  virtual void notify(SoNotList * list);

protected:
  virtual ~SoVRMLBillboard();

private:
  void performRotation(SoState * state) const;

}; // class SoVRMLBillboard

#endif // ! COIN_SOVRMLBILLBOARD_H
