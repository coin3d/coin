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
  \class SoDirectionalLight SoDirectionalLight.h Inventor/nodes/SoDirectionalLight.h
  \brief The SoDirectionalLight class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoDirectionalLight.h>

#include <Inventor/SbVec4f.h>
#include <Inventor/SbColor4f.h>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32
#include <GL/gl.h>
#include <Inventor/actions/SoGLRenderAction.h>

#include <Inventor/elements/SoGLLightIdElement.h>
#include <Inventor/elements/SoEnvironmentElement.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var SoSFVec3f SoDirectionalLight::direction
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoDirectionalLight);

/*!
  Constructor.
*/
SoDirectionalLight::SoDirectionalLight()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoDirectionalLight);

  SO_NODE_ADD_FIELD(on,(TRUE));
  SO_NODE_ADD_FIELD(intensity,(1.0f));
  SO_NODE_ADD_FIELD(color,(1.0f, 1.0f, 1.0f));
  SO_NODE_ADD_FIELD(direction,(0.0f, 0.0f, -1.0f));
}

/*!
  Destructor.
*/
SoDirectionalLight::~SoDirectionalLight()
{
}

/*!
  Does initialization common for all objects of the
  SoDirectionalLight class. This includes setting up the
  type system, among other things.
*/
void
SoDirectionalLight::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoDirectionalLight);
}

/*!
  FIXME: write function documentation
*/
void
SoDirectionalLight::GLRender(SoGLRenderAction * action)
{
  if (!on.getValue()) return;

  SoState *state = action->getState();
  int idx = SoGLLightIdElement::increment(state);

  if (idx < 0) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoDirectionalLight::GLRender",
                              "Max # of OpenGL lights exceeded :(");
#endif // COIN_DEBUG
    return;
  }

  GLenum light = (GLenum) (idx + GL_LIGHT0);

  SbColor4f lightcolor(0.0f, 0.0f, 0.0f, 1.0f);
  lightcolor.setRGB(SoEnvironmentElement::getAmbientColor(state));
  lightcolor *= SoEnvironmentElement::getAmbientIntensity(state);
  glLightfv(light, GL_AMBIENT, lightcolor.getValue());

  lightcolor.setRGB(color.getValue());
  if (!intensity.isIgnored()) lightcolor *= intensity.getValue();

  glLightfv(light, GL_DIFFUSE, lightcolor.getValue());
  glLightfv(light, GL_SPECULAR, lightcolor.getValue());

  // GL directional light is specified towards light source
  SbVec3f dir = -direction.getValue();
  dir.normalize();

  // directional when w = 0.0
  SbVec4f dirvec(dir[0], dir[1], dir[2], 0.0f);
  glLightfv(light, GL_POSITION, dirvec.getValue());

  // FIXME: is this needed for directional lights?
  // turn off spot light properties for ordinary lights
  glLightf(light, GL_SPOT_EXPONENT, 0.0);
  glLightf(light, GL_SPOT_CUTOFF, 180.0);
  glLightf(light, GL_CONSTANT_ATTENUATION, 1);
  glLightf(light, GL_LINEAR_ATTENUATION, 0);
  glLightf(light, GL_QUADRATIC_ATTENUATION, 0);
}

