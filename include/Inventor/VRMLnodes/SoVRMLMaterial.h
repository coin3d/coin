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

#ifndef COIN_SOVRMLMATERIAL_H
#define COIN_SOVRMLMATERIAL_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFFloat.h>

class SoVRMLMaterialP;

class COIN_DLL_API SoVRMLMaterial : public SoNode
{
  typedef SoNode inherited;
  SO_NODE_HEADER(SoVRMLMaterial);

public:
  static void initClass(void);
  SoVRMLMaterial(void);

  SoSFColor diffuseColor;
  SoSFFloat ambientIntensity;
  SoSFColor specularColor;
  SoSFColor emissiveColor;
  SoSFFloat shininess;
  SoSFFloat transparency;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);

protected:
  virtual ~SoVRMLMaterial();

private:
  SoVRMLMaterialP * pimpl;
}; // class SoVRMLMaterial

#endif // ! COIN_SOVRMLMATERIAL_H
