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

#include <Inventor/SbColor4f.h>
#include <Inventor/SbVec4f.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoEnvironmentElement.h>
#include <Inventor/elements/SoGLLightIdElement.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef _WIN32
#include <windows.h> // Needed on Win32 to include gl.h.
#endif // _WIN32
#include <GL/gl.h>



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

  SO_NODE_ADD_FIELD(location, (SbVec3f(0.0f, 0.0f, 1.0f)));
  SO_NODE_ADD_FIELD(direction, (SbVec3f(0.0f, 0.0f, -1.0f)));
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
  SO_NODE_INTERNAL_INIT_CLASS(SoSpotLight);
}

// Doc in superclass.
void
SoSpotLight::GLRender(SoGLRenderAction * action)
{
  if (!on.getValue()) return;

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

  SbColor4f lightcolor(0.0f, 0.0f, 0.0f, 1.0f);
  SbVec3f attenuation(0.0f, 0.0f, 1.0f);
  lightcolor.setRGB(SoEnvironmentElement::getAmbientColor(state));
  lightcolor *= SoEnvironmentElement::getAmbientIntensity(state);
  attenuation = SoEnvironmentElement::getLightAttenuation(state);

  glLightfv(light, GL_AMBIENT, lightcolor.getValue());
  glLightf(light, GL_QUADRATIC_ATTENUATION, attenuation[0]);
  glLightf(light, GL_LINEAR_ATTENUATION, attenuation[1]);
  glLightf(light, GL_CONSTANT_ATTENUATION, attenuation[2]);

  lightcolor.setRGB(color.getValue());
  if (!intensity.isIgnored()) lightcolor *= intensity.getValue();

  glLightfv(light, GL_DIFFUSE, lightcolor.getValue());
  glLightfv(light, GL_SPECULAR, lightcolor.getValue());

  SbVec3f loc = location.getValue();

  // point (or spot) light when w = 1.0
  SbVec4f posvec(loc[0], loc[1], loc[2], 1.0f);
  glLightfv(light, GL_POSITION, posvec.getValue());

  float cutOff = !cutOffAngle.isIgnored() ?
    cutOffAngle.getValue() * float(M_PI)/180.0f :
    float(M_PI)/4.0f;

  float dropOff = !dropOffRate.isIgnored() ?
    dropOffRate.getValue() * 128.0f :
    0.0f;

  glLightf(light, GL_SPOT_EXPONENT, dropOff);
  glLightf(light, GL_SPOT_CUTOFF, cutOff);
}
