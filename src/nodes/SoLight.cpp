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
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoLight SoLight.h Inventor/nodes/SoLight.h
  \brief The SoLight class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoLight.h>



#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOLIGHTATTENUATIONELEMENT)
#include <Inventor/elements/SoLightAttenuationElement.h>
#endif // !COIN_EXCLUDE_SOLIGHTATTENUATIONELEMENT
#if !defined(COIN_EXCLUDE_SOGLLIGHTIDELEMENT)
#include <Inventor/elements/SoGLLightIdElement.h>
#endif // !COIN_EXCLUDE_SOGLLIGHTIDELEMENT

#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoLightElement.h>
#include <Inventor/actions/SoCallbackAction.h>

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


SO_NODE_ABSTRACT_SOURCE(SoLight);

/*!
  Constructor.
*/
SoLight::SoLight()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoLight);

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
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoLight);
  
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoLightAttenuationElement);
  SO_ENABLE(SoGLRenderAction, SoGLLightIdElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
  
  SO_ENABLE(SoCallbackAction, SoLightAttenuationElement);
  SO_ENABLE(SoCallbackAction, SoLightElement);
}

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoLight::callback(SoCallbackAction *action)
{
  SoState *state = action->getState();
  SoLightElement::add(state, this, 
		      SoModelMatrixElement::get(state));
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
