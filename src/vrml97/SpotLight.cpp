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

/*!
  \class SoVRMLSpotLight SoVRMLSpotLight.h Inventor/VRMLnodes/SoVRMLSpotLight.h
  \brief The SoVRMLSpotLight class defines a spot light source.
*/

/*!
  \var SoSFVec3f SoVRMLSpotLight::location
  The light position. Default value is (0, 0, 0).
*/

/*!
  \var SoSFVec3f SoVRMLSpotLight::direction
  The light direction. Default value is (0, 0, 1).
*/

/*!
  \var SoSFFloat SoVRMLSpotLight::beamWidth
  The spot beam width. Default value is PI/2.
*/

/*!
  \var SoSFFloat SoVRMLSpotLight::cutOffAngle
  The spot light cut off angle. Default value is PI/4.
*/

/*!
  \var SoSFFloat SoVRMLSpotLight::radius
  The light radius. Light is not emitted past it. Default value is 100.
*/

/*!
  \var SoSFVec3f SoVRMLSpotLight::attenuation
  The attenuiation vector. Default value is (1, 0, 0).
*/

#include <Inventor/VRMLnodes/SoVRMLSpotLight.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/SbColor4f.h>
#include <Inventor/SbVec4f.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoEnvironmentElement.h>
#include <Inventor/elements/SoGLLightIdElement.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>

#include <math.h>

SO_NODE_SOURCE(SoVRMLSpotLight);

// Doc in parent
void
SoVRMLSpotLight::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLSpotLight, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLSpotLight::SoVRMLSpotLight(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLSpotLight);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(location, (0.0f, 0.0f, 0.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(direction,(0.0f, 0.0f, -1.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(beamWidth, (float(M_PI)/2.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(cutOffAngle, (float(M_PI)/4.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(radius, (100.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(attenuation, (1.0f, 0.0f, 0.0f));
}

/*!
  Destructor.
*/
SoVRMLSpotLight::~SoVRMLSpotLight()
{
}

// Doc in parent
void
SoVRMLSpotLight::GLRender(SoGLRenderAction * action)
{
  if (!this->on.getValue()) return;

  SoState * state = action->getState();
  int idx = SoGLLightIdElement::increment(state);

  if (idx < 0) {
#if COIN_DEBUG
    SoDebugError::post("SoSpotLight::GLRender()",
                       "Max # lights exceeded :(\n");
#endif // COIN_DEBUG
    return;
  }

  GLenum light = (GLenum) (idx + GL_LIGHT0);

  SbVec3f att = this->attenuation.getValue();

  glLightf(light, GL_QUADRATIC_ATTENUATION, att[0]);
  glLightf(light, GL_LINEAR_ATTENUATION, att[1]);
  glLightf(light, GL_CONSTANT_ATTENUATION, att[2]);

  SbColor4f lightcolor(0.0f, 0.0f, 0.0f, 1.0f);
  lightcolor.setRGB(this->color.getValue());
  lightcolor *= this->ambientIntensity.getValue();
  glLightfv(light, GL_AMBIENT, lightcolor.getValue());

  lightcolor.setRGB(this->color.getValue());
  lightcolor *= this->intensity.getValue();

  glLightfv(light, GL_DIFFUSE, lightcolor.getValue());
  glLightfv(light, GL_SPECULAR, lightcolor.getValue());

  SbVec3f loc = this->location.getValue();

  // point (or spot) light when w = 1.0
  SbVec4f posvec(loc[0], loc[1], loc[2], 1.0f);
  glLightfv(light, GL_POSITION, posvec.getValue());
  glLightfv(light, GL_SPOT_DIRECTION, this->direction.getValue().getValue());

  float cutoff = this->cutOffAngle.getValue() * 180.0f / float(M_PI);
  glLightf(light, GL_SPOT_EXPONENT, 0.0f);
  glLightf(light, GL_SPOT_CUTOFF, cutoff);

  // FIXME: consider radius and beamWidth
}
