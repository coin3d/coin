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
  \class SoPointLight SoPointLight.h Inventor/nodes/SoPointLight.h
  \brief The SoPointLight class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoPointLight.h>

#include <Inventor/SbVec4f.h>
#include <Inventor/SbColor4f.h>
#include <Inventor/elements/SoEnvironmentElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#ifdef _WIN32
#include <windows.h>
#endif // _WIN32
#include <GL/gl.h>
#include <Inventor/elements/SoGLLightIdElement.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var SoSFVec3f SoPointLight::location
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoPointLight);

/*!
  Constructor.
*/
SoPointLight::SoPointLight()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoPointLight);

  SO_NODE_ADD_FIELD(location, (0.0f, 0.0f, 1.0f));
}

/*!
  Destructor.
*/
SoPointLight::~SoPointLight()
{
}

/*!
  Does initialization common for all objects of the
  SoPointLight class. This includes setting up the
  type system, among other things.
*/
void
SoPointLight::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoPointLight);
}

/*!
  FIXME: write function documentation
*/
void
SoPointLight::GLRender(SoGLRenderAction * action)
{
  if (!on.getValue()) return;

  int idx = SoGLLightIdElement::increment(action->getState());

  if (idx < 0) {
#if COIN_DEBUG
    SoDebugError::post("SoPointLight::GLRender()",
                       "Max # lights exceeded :(\n");
#endif // COIN_DEBUG
    return;
  }

  SoState *state = action->getState();

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

  // turning off spot light properties for ordinary lights
  glLightf(light, GL_SPOT_EXPONENT, 0.0);
  glLightf(light, GL_SPOT_CUTOFF, 180.0);
}
