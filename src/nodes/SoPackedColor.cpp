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
  \class SoPackedColor SoPackedColor.h Inventor/nodes/SoPackedColor.h
  \brief The SoPackedColor class is a node for setting diffuse and transparency material values.
  \ingroup nodes

  This node provides a convenient way of setting diffuse colors and
  transparency values with packed 32-bit RGBA vectors.

  \sa SoBaseColor
*/

// FIXME: the name of the "orderedRGBA" field was actually "rgba" in
// the version 2.0 Inventor file format, and the semantics was also
// different (reversed order). This means that Inventor 2.0 files with
// PackedColor nodes will not import properly at the moment.  20020508 mortene.

#include <Inventor/nodes/SoPackedColor.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLDiffuseColorElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoGLPolygonStippleElement.h>

/*!
  \var SoMFUInt32 SoPackedColor::orderedRGBA

  Set of packed 32-bit RGBA vectors.

  The most significant 24 bits specifies 8 bits each for the red,
  green and blue components.

  The least significant 8 bits specifies the transparency value, where
  0x00 means completely transparent, and 0xff completely opaque.
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

  this->checktransparent = FALSE;
  this->transparent = FALSE;
}

/*!
  Destructor.
*/
SoPackedColor::~SoPackedColor()
{
}

// Doc from superclass.
void
SoPackedColor::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoPackedColor);

  SO_ENABLE(SoCallbackAction, SoDiffuseColorElement);
  SO_ENABLE(SoGLRenderAction, SoGLDiffuseColorElement);
}

// Doc from superclass.
void
SoPackedColor::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  if (SoShapeStyleElement::isScreenDoor(state) &&
      ! this->orderedRGBA.isIgnored() &&
      ! SoOverrideElement::getTransparencyOverride(state)) {
    float t = (255 - (this->orderedRGBA[0] & 0xff)) / 255.0f;
    SoGLPolygonStippleElement::setTransparency(state, t);
    SoGLPolygonStippleElement::set(state, t >= 1.0f/255.0f);
  }

  SoPackedColor::doAction(action);
}

// Doc from superclass.
void
SoPackedColor::doAction(SoAction * action)
{
  this->isTransparent(); // update cached value

  SoState * state = action->getState();
  if (!this->orderedRGBA.isIgnored() &&
      !SoOverrideElement::getDiffuseColorOverride(state)) {
    SoDiffuseColorElement::set(state,
                               this,
                               this->orderedRGBA.getNum(),
                               this->orderedRGBA.getValues(0),
                               this->transparent);
    if (this->isOverride()) {
      SoOverrideElement::setDiffuseColorOverride(state, this, TRUE);
    }
  }
}

// Doc from superclass.
void
SoPackedColor::callback(SoCallbackAction * action)
{
  SoPackedColor::doAction(action);
}

/*!
  Returns \c TRUE if there is at least one RGBA vector in the set
  which is not completely opaque.
 */
SbBool
SoPackedColor::isTransparent(void)
{
  if (this->checktransparent) {
    this->checktransparent = FALSE;
    this->transparent = FALSE;
    int n = this->orderedRGBA.getNum();
    for (int i = 0; i < n; i++) {
      if ((this->orderedRGBA[i] & 0xff) != 0xff) {
        this->transparent = TRUE;
        break;
      }
    }
  }
  return this->transparent;
}

// Documented in superclass.
void
SoPackedColor::notify(SoNotList *list)
{
  // Overridden to check for transparency when orderedRGBA changes.

  SoField *f = list->getLastField();
  if (f == &this->orderedRGBA) {
    this->checktransparent = TRUE;
  }
  SoNode::notify(list);
}
