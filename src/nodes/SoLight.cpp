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
  \class SoLight SoLight.h Inventor/nodes/SoLight.h
  \brief The SoLight class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoLight.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOLIGHTATTENUATIONELEMENT)
#include <Inventor/elements/SoLightAttenuationElement.h>
#endif // !COIN_EXCLUDE_SOLIGHTATTENUATIONELEMENT
#if !defined(COIN_EXCLUDE_SOGLLIGHTIDELEMENT)
#include <Inventor/elements/SoGLLightIdElement.h>
#endif // !COIN_EXCLUDE_SOGLLIGHTIDELEMENT

/*!
  \var SoSFBool SoLight::on
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoLight::intensity
  FIXME: write documentation for field
*/
/*!
  \var SoSFColor SoLight::color
  FIXME: write documentation for field
*/


//$ BEGIN TEMPLATE NodeAbstractSource(SoLight)
SoType SoLight::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the SoLight class.
*/
SoType
SoLight::getClassTypeId(void)
{
  return SoLight::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoLight::getTypeId(void) const
{
  return SoLight::classTypeId;
}
//$ END TEMPLATE NodeAbstractSource

/*!
  Constructor.
*/
SoLight::SoLight()
{
//$ BEGIN TEMPLATE NodeConstructor(SoLight)
  // Make sure the class has been initialized.
  assert(SoLight::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(on,(TRUE));
  SO_NODE_ADD_FIELD(intensity,(1.0f));
  SO_NODE_ADD_FIELD(color,(SbColor(1.0f, 1.0f, 1.0f)));
}

/*!
  Destructor.
*/
SoLight::~SoLight()
{
}


/*!
  Does initialization common for all objects of the
  SoLight class. This includes setting up the
  type system, among other things.
*/
void
SoLight::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeAbstractSource(Light)
  // Make sure we only initialize once.
  assert(SoLight::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoLight::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "Light",
                       NULL,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeAbstractSource

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoLightAttenuationElement);
  SO_ENABLE(SoGLRenderAction, SoGLLightIdElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoLight::cleanClass(void)
{
}


#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoLight::callback(SoCallbackAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
