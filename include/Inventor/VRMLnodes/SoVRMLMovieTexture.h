/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
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
  SoMFString url;
  SoSFTime duration_changed;
  SoSFBool isActive;

#ifndef COIN_BETA_VERSION
#error Consider adding fields "alpha", "model", "blendColor" and "scaleTexCoord" to be compatible with TGS OIV >= 4.0.
#endif // COIN_BETA_VERSION

  virtual void GLRender( SoGLRenderAction * action );

protected:
  virtual ~SoVRMLMovieTexture();

private:
  SoVRMLMovieTextureP * pimpl;
};

#endif // ! COIN_SOVRMLMOVIETEXTURE_H
