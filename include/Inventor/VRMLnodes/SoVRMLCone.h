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

#ifndef COIN_SOVRMLCONE_H
#define COIN_SOVRMLCONE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLGeometry.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>

class COIN_DLL_API SoVRMLCone : public SoVRMLGeometry
{
  typedef SoVRMLGeometry inherited;
  SO_NODE_HEADER(SoVRMLCone);

public:
  static void initClass(void);
  SoVRMLCone(void);

  SoSFFloat bottomRadius;
  SoSFFloat height;
  SoSFBool side;
  SoSFBool bottom;

  virtual void GLRender(SoGLRenderAction * action);
  virtual void rayPick(SoRayPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoVRMLCone();

  virtual void generatePrimitives(SoAction * action);
  virtual void computeBBox(SoAction * action,
                           SbBox3f & box, SbVec3f & center);

}; // class SoVRMLCone

#endif // ! COIN_SOVRMLCONE_H
