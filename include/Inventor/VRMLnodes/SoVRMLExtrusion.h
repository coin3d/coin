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
