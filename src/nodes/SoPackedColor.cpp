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
  \class SoPackedColor SoPackedColor.h Inventor/nodes/SoPackedColor.h
  \brief The SoPackedColor class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoPackedColor.h>
#include <coindefs.h> // COIN_STUB()

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLDiffuseColorElement.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/elements/SoOverrideElement.h>

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

  SO_ENABLE(SoGLRenderAction, SoGLDiffuseColorElement);

  SO_ENABLE(SoCallbackAction, SoDiffuseColorElement);
}

/*!
  FIXME: write function documentation
*/
void
SoPackedColor::GLRender(SoGLRenderAction * action)
{
  SoPackedColor::doAction(action);
}

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
    if (this->isOverride()) {
      SoOverrideElement::setDiffuseColorOverride(state, this, TRUE);
    }
  }
}

/*!
  FIXME: write doc
 */
void
SoPackedColor::callback(SoCallbackAction *action)
{
  SoPackedColor::doAction(action);
}

/*!
  FIXME: write doc
 */
SbBool
SoPackedColor::isTransparent(void)
{
  COIN_STUB();
  return FALSE;
}
