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

#ifndef COIN_SOVRMLSOUND_H
#define COIN_SOVRMLSOUND_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFVec3f.h>

class COIN_DLL_API SoVRMLSound : public SoNode
{
  typedef SoNode inherited;
  SO_NODE_HEADER(SoVRMLSound);

public:
  static void initClass(void);
  SoVRMLSound(void);

  SoSFNode source;
  SoSFFloat intensity;
  SoSFFloat priority;
  SoSFVec3f location;
  SoSFVec3f direction;
  SoSFFloat minFront;
  SoSFFloat maxFront;
  SoSFFloat minBack;
  SoSFFloat maxBack;
  SoSFBool spatialize;

  virtual void GLRender( SoGLRenderAction * action );

protected:
  virtual ~SoVRMLSound(void);

}; // class SoVRMLSound

#endif // ! COIN_SOVRMLSOUND_H
