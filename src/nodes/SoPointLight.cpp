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
  \class SoPointLight SoPointLight.h Inventor/nodes/SoPointLight.h
  \brief The SoPointLight class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/SoDB.h>
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>
#include <Inventor/SbVec4f.h>
#include <Inventor/SbColor4f.h>

#if !defined(COIN_EXCLUDE_SOENVIRONMENTELEMENT)
#include <Inventor/elements/SoEnvironmentElement.h>
#endif // ! COIN_EXCLUDE_SOENVIRONMENTELEMENT
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#ifdef _WIN32
#include <windows.h>
#endif // _WIN32
#include <GL/gl.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOGLLIGHTIDELEMENT)
#include <Inventor/elements/SoGLLightIdElement.h>
#endif // !COIN_EXCLUDE_SOGLLIGHTIDELEMENT

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var SoSFVec3f SoPointLight::location
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoPointLight)
SoType SoPointLight::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoPointLight node class.
*/
void *
SoPointLight::createInstance(void)
{
  return new SoPointLight;
}

/*!
  Returns the unique type identifier for the SoPointLight class.
*/
SoType
SoPointLight::getClassTypeId(void)
{
  return SoPointLight::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoPointLight::getTypeId(void) const
{
  return SoPointLight::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoPointLight::SoPointLight()
{
//$ BEGIN TEMPLATE NodeConstructor(SoPointLight)
  // Make sure the class has been initialized.
  assert(SoPointLight::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

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
//$ BEGIN TEMPLATE InitNodeSource(PointLight)
  // Make sure we only initialize once.
  assert(SoPointLight::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoPointLight::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "PointLight",
                       &SoPointLight::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoPointLight::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoPointLight::GLRender(SoGLRenderAction * action)
{
  if (!on.isIgnored() && !on.getValue()) return;

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
#if !defined(COIN_EXCLUDE_SOENVIRONMENTELEMENT)
  lightcolor.setRGB(SoEnvironmentElement::getAmbientColor(state));      
  lightcolor *= SoEnvironmentElement::getAmbientIntensity(state);
  attenuation = SoEnvironmentElement::getLightAttenuation(state);
#endif // ! COIN_EXCLUDE_SOENVIRONMENTELEMENT

  glLightfv(light, GL_AMBIENT, lightcolor.getValue());
  glLightf(light, GL_QUADRATIC_ATTENUATION, attenuation[0]);
  glLightf(light, GL_LINEAR_ATTENUATION, attenuation[1]);
  glLightf(light, GL_CONSTANT_ATTENUATION, attenuation[2]);
  
  if (!color.isIgnored()) lightcolor.setRGB(color.getValue());
  else lightcolor.setRGB(SbColor(1.0f, 1.0f, 1.0f));
  if (!intensity.isIgnored()) lightcolor *= intensity.getValue();
  
  glLightfv(light, GL_DIFFUSE, lightcolor.getValue());
  glLightfv(light, GL_SPECULAR, lightcolor.getValue());
  
  SbVec3f loc(0.0f, 0.0f, 1.0f); // default
  if (!location.isIgnored()) loc = location.getValue();

  // point (or spot) light when w = 1.0
  SbVec4f posvec(loc[0], loc[1], loc[2], 1.0f);
  glLightfv(light, GL_POSITION, posvec.getValue());
  
  // turning off spot light properties for ordinary lights
  glLightf(light, GL_SPOT_EXPONENT, 0.0);
  glLightf(light, GL_SPOT_CUTOFF, 180.0);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
