/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoDirectionalLight SoDirectionalLight.h Inventor/nodes/SoDirectionalLight.h
  \brief The SoDirectionalLight class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>
#include <Inventor/SbVec4f.h>
#include <Inventor/SbColor4f.h>

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <GL/gl.h>
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGLLIGHTIDELEMENT)
#include <Inventor/elements/SoGLLightIdElement.h>
#endif // !COIN_EXCLUDE_SOGLLIGHTIDELEMENT

#if !defined(COIN_EXCLUDE_SOENVIRONMENTELEMENT)
#include <Inventor/elements/SoEnvironmentElement.h>
#endif // ! COIN_EXCLUDE_SOENVIRONMENTELEMENT

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var SoSFVec3f SoDirectionalLight::direction
  FIXME: write documentation for field
*/


// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoDirectionalLight)
SoType SoDirectionalLight::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoDirectionalLight node class.
*/
void *
SoDirectionalLight::createInstance(void)
{
  return new SoDirectionalLight;
}

/*!
  Returns the unique type identifier for the SoDirectionalLight class.
*/
SoType
SoDirectionalLight::getClassTypeId(void)
{
  return SoDirectionalLight::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoDirectionalLight::getTypeId(void) const
{
  return SoDirectionalLight::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoDirectionalLight::SoDirectionalLight()
{
//$ BEGIN TEMPLATE NodeConstructor(SoDirectionalLight)
  // Make sure the class has been initialized.
  assert(SoDirectionalLight::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

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
//$ BEGIN TEMPLATE InitNodeSource(DirectionalLight)
  // Make sure we only initialize once.
  assert(SoDirectionalLight::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoDirectionalLight::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "DirectionalLight",
                       &SoDirectionalLight::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoDirectionalLight::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoDirectionalLight::GLRender(SoGLRenderAction * action)
{
  if (!on.isIgnored() && !on.getValue()) return;

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
#if !defined(COIN_EXCLUDE_SOENVIRONMENTELEMENT)
  lightcolor.setRGB(SoEnvironmentElement::getAmbientColor(state));  
  lightcolor *= SoEnvironmentElement::getAmbientIntensity(state);
#endif // ! COIN_EXCLUDE_SOENVIRONMENTELEMENT
  glLightfv(light, GL_AMBIENT, lightcolor.getValue());
  
  if (!color.isIgnored()) lightcolor.setRGB(color.getValue());
  else lightcolor.setRGB(SbColor(1.0f, 1.0f, 1.0f));
  if (!intensity.isIgnored()) lightcolor *= intensity.getValue();
  
  glLightfv(light, GL_DIFFUSE, lightcolor.getValue());
  glLightfv(light, GL_SPECULAR, lightcolor.getValue());
    
  // GL directional light is specified towards light source
  SbVec3f dir(0,0,1);
  if (!direction.isIgnored()) {
    dir = -direction.getValue(); 
    dir.normalize();
  }
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
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
