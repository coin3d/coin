/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef __SOENVIRONMENT_H__
#define __SOENVIRONMENT_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/elements/SoEnvironmentElement.h>


class SoEnvironment : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoEnvironment);

public:
  static void initClass(void);
  SoEnvironment(void);

  enum FogType {
    NONE = SoEnvironmentElement::NONE,
    HAZE = SoEnvironmentElement::HAZE,
    FOG = SoEnvironmentElement::FOG,
    SMOKE = SoEnvironmentElement::SMOKE
  };

  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);

  SoSFFloat ambientIntensity;
  SoSFColor ambientColor;
  SoSFVec3f attenuation;
  SoSFEnum fogType;
  SoSFColor fogColor;
  SoSFFloat fogVisibility;

protected:
  virtual ~SoEnvironment();
};

#endif // !__SOENVIRONMENT_H__
