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

#ifndef COIN_SOVRMLELEVATIONGRID_H
#define COIN_SOVRMLELEVATIONGRID_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLGeometry.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoSFNode.h>

class SoChildList;

class COIN_DLL_API SoVRMLElevationGrid : public SoVRMLGeometry
{
  typedef SoVRMLGeometry inherited;
  SO_NODE_HEADER(SoVRMLElevationGrid);

public:
  static void initClass(void);
  SoVRMLElevationGrid(void);

  SoSFBool ccw;
  SoSFBool solid;
  SoSFFloat creaseAngle;
  SoSFInt32 zDimension;
  SoSFInt32 xDimension;
  SoSFFloat zSpacing;
  SoSFFloat xSpacing;
  SoMFFloat height;
  SoSFNode texCoord;
  SoSFNode normal;
  SoSFNode color;
  SoSFBool colorPerVertex;
  SoSFBool normalPerVertex;

  virtual void GLRender(SoGLRenderAction * action);
  virtual void rayPick(SoRayPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoVRMLElevationGrid(void);

  virtual void notify(SoNotList * list);
  virtual void generatePrimitives( SoAction * action );
  virtual void computeBBox(SoAction * action, SbBox3f & bbox,
                           SbVec3f & center);

private:
  friend class SoVRMLElevationGridP;

  enum Binding {
    OVERALL,
    PER_QUAD,
    PER_VERTEX
  };

  Binding findMaterialBinding(void) const;
  Binding findNormalBinding(void) const;

  const SbVec3f * updateNormalCache(Binding & nbind);

  class SoVRMLElevationGridP * pimpl;

}; // class SoVRMLElevationGrid

#endif // ! COIN_SOVRMLELEVATIONGRID_H
