/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoSpotLight SoSpotLight.h Inventor/nodes/SoSpotLight.h
  \brief The SoSpotLight class is a node type for light sources with a cone shaped lightvolume.
  \ingroup nodes

  Spotlights are light sources with a position and a direction. They
  can be thought of as a pointlight with a lampshade.

  See also documentation of parent class for important information
  regarding light sources in general.

  \sa SoSpotLight
*/


#include <Inventor/nodes/SoSpotLight.h>
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



/*!
  \var SoSFVec3f SoSpotLight::location

  3D position of light source. Default position is <0, 0, 1>.
*/
/*!
  \var SoSFVec3f SoSpotLight::direction

  Direction vector, where the light is pointing. Default is to point
  along the negative z-axis.
*/
/*!
  \var SoSFFloat SoSpotLight::dropOffRate

  The rate of intensity drop-off from the ray along the direction
  vector. Value must be between 0.0 (equal intensity for the whole
  cone of light), to 1.0 (a narrow intensity ray).

  Default value is 0.0.
*/
/*!
  \var SoSFFloat SoSpotLight::cutOffAngle

  The angle in radians from the direction vector where there will be
  no light outside (i.e. the angle of the "lampshade"). Default value
  is PI/4.0 (i.e. 45°).
*/


// *************************************************************************

SO_NODE_SOURCE(SoSpotLight);

/*!
  Constructor.
*/
SoSpotLight::SoSpotLight(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoSpotLight);

  SO_NODE_ADD_FIELD(location, (0.0f, 0.0f, 1.0f));
  SO_NODE_ADD_FIELD(direction, (0.0f, 0.0f, -1.0f));
  SO_NODE_ADD_FIELD(dropOffRate, (0.0f));
  SO_NODE_ADD_FIELD(cutOffAngle, (float(M_PI)/4.0f));
}

/*!
  Destructor.
*/
SoSpotLight::~SoSpotLight()
{
}

// Doc in superclass.
void
SoSpotLight::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoSpotLight, SO_FROM_INVENTOR_2_1);
}

// Doc in superclass.
void
SoSpotLight::GLRender(SoGLRenderAction * action)
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

  SbVec3f attenuation = SoEnvironmentElement::getLightAttenuation(state);

  glLightf(light, GL_QUADRATIC_ATTENUATION, attenuation[0]);
  glLightf(light, GL_LINEAR_ATTENUATION, attenuation[1]);
  glLightf(light, GL_CONSTANT_ATTENUATION, attenuation[2]);

  SbColor4f lightcolor(0.0f, 0.0f, 0.0f, 1.0f);
  // disable ambient contribution from this light source
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
  float dropoff = this->dropOffRate.getValue() * 128.0f;

  glLightf(light, GL_SPOT_EXPONENT, dropoff);
  glLightf(light, GL_SPOT_CUTOFF, cutoff);
}
