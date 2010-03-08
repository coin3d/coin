/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifndef COIN_SOVRMLMOVIETEXTURE_H
#define COIN_SOVRMLMOVIETEXTURE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLTexture.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoMFEnum.h>
#include <Inventor/fields/SoSFColor.h>

class SoVRMLMovieTextureP;

class COIN_DLL_API SoVRMLMovieTexture : public SoVRMLTexture
{
  typedef SoVRMLTexture inherited;
  SO_NODE_HEADER(SoVRMLMovieTexture);

public:
  static void initClass(void);
  SoVRMLMovieTexture(void);

  SoSFBool loop;
  SoSFFloat speed;
  SoSFTime startTime;
  SoSFTime stopTime;
  SoSFTime pauseTime;
  SoSFTime resumeTime;
  SoMFString url;
  SoSFTime duration_changed;
  SoSFBool isActive;
  SoSFFloat alpha;
  SoSFEnum model;
  SoSFColor blendColor;
  SoSFBool scaleTexCoord;

  virtual void GLRender( SoGLRenderAction * action );

protected:
  virtual ~SoVRMLMovieTexture();

private:
  SoVRMLMovieTextureP * pimpl;
};

#endif // ! COIN_SOVRMLMOVIETEXTURE_H
