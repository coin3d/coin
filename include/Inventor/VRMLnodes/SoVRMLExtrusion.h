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

#ifndef COIN_SOVRMLEXTRUSION_H
#define COIN_SOVRMLEXTRUSION_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLGeometry.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoMFVec2f.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFRotation.h>

class COIN_DLL_API SoVRMLExtrusion : public SoVRMLGeometry
{
  typedef SoVRMLGeometry inherited;
  SO_NODE_HEADER(SoVRMLExtrusion);

public:
  static void initClass(void);
  SoVRMLExtrusion(void);

  SoSFBool beginCap;
  SoSFBool ccw;
  SoSFBool convex;
  SoSFFloat creaseAngle;
  SoMFVec2f crossSection;
  SoSFBool endCap;
  SoMFRotation orientation;
  SoMFVec2f scale;
  SoSFBool solid;
  SoMFVec3f spine;

  virtual void GLRender(SoGLRenderAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
  virtual void computeBBox(SoAction * action,
                           SbBox3f & bbox, SbVec3f & center);

protected:
  virtual ~SoVRMLExtrusion();

  virtual void notify(SoNotList * list);
  virtual void generatePrimitives( SoAction * action );

  virtual SoDetail * createTriangleDetail(SoRayPickAction * action,
                                          const SoPrimitiveVertex * v1,
                                          const SoPrimitiveVertex * v2,
                                          const SoPrimitiveVertex * v3,
                                          SoPickedPoint * pp);
private:
  void updateCache(void);
  class SoVRMLExtrusionP * pimpl;
}; // class SoVRMLExtrusion

#endif // ! COIN_SOVRMLEXTRUSION_H
