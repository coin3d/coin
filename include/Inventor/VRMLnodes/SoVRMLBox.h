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

#ifndef COIN_SOVRMLBOX_H
#define COIN_SOVRMLBOX_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLGeometry.h>
#include <Inventor/fields/SoSFVec3f.h>

class COIN_DLL_API SoVRMLBox : public SoVRMLGeometry
{
  typedef SoVRMLGeometry inherited;
  SO_NODE_HEADER(SoVRMLBox);

public:
  static void initClass(void);
  SoVRMLBox(void);

  SoSFVec3f size;

  virtual void GLRender(SoGLRenderAction * action);
  virtual void rayPick(SoRayPickAction * action);
  virtual void getPrimitiveCount( SoGetPrimitiveCountAction * action );

protected:
  virtual ~SoVRMLBox();

  virtual void generatePrimitives( SoAction * action );
  virtual void computeBBox( SoAction * action, SbBox3f & box,
                            SbVec3f & center );
}; // class SoVRMLBox

#endif // ! COIN_SOVRMLBOX_H
