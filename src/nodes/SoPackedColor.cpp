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
  \class SoPackedColor SoPackedColor.h Inventor/nodes/SoPackedColor.h
  \brief The SoPackedColor class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoPackedColor.h>



#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLDiffuseColorElement.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SODIFFUSECOLORELEMENT)
#include <Inventor/elements/SoDiffuseColorElement.h>
#endif // !COIN_EXCLUDE_SODIFFUSECOLORELEMENT

#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/actions/SoCallbackAction.h>

/*!
  \var SoMFUInt32 SoPackedColor::orderedRGBA
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoPackedColor);

/*!
  Constructor.
*/
SoPackedColor::SoPackedColor()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoPackedColor);
  
  SO_NODE_ADD_FIELD(orderedRGBA, (0xccccccff));
}

/*!
  Destructor.
*/
SoPackedColor::~SoPackedColor()
{
}

/*!
  Does initialization common for all objects of the
  SoPackedColor class. This includes setting up the
  type system, among other things.
*/
void
SoPackedColor::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoPackedColor);

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoGLDiffuseColorElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

  SO_ENABLE(SoCallbackAction, SoDiffuseColorElement);
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoPackedColor::GLRender(SoGLRenderAction * action)
{
  SoPackedColor::doAction(action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoPackedColor::doAction(SoAction *action)
{
  SoState *state = action->getState();
  if (!orderedRGBA.isIgnored() && 
      !SoOverrideElement::getDiffuseColorOverride(state)) {
    SoDiffuseColorElement::set(state,
			       this,
			       orderedRGBA.getNum(),
			       orderedRGBA.getValues(0));
  }
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoPackedColor::callback(SoCallbackAction *action)
{
  SoPackedColor::doAction(action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

/*!
  FIXME: write doc
 */
SbBool
SoPackedColor::isTransparent(void)
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}
