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
  \class SoPickStyle SoPickStyle.h Inventor/nodes/SoPickStyle.h
  \brief The SoPickStyle class is a node for setting up how to do picking.
  \ingroup nodes

  By default, all geometry in a scene is available for picking. Upon
  writing applications with interaction possibilities, this is often
  \e not what you want. To exclude parts of the scene graph from pick
  actions, use the SoPickStyle::UNPICKABLE.

  You can also optimize pick operations by using the
  SoPickStyle::BOUNDING_BOX pickstyle.

  \sa SoRayPickAction
*/


#include <Inventor/actions/SoPickAction.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/actions/SoCallbackAction.h>

/*!
  \enum SoPickStyle::Style

  Enumeration of the available picking strategies.
*/
/*!
  \var SoPickStyle::Style SoPickStyle::SHAPE

  Do "exact" picks, finding the correct intersection point(s), etc.
*/
/*!
  \var SoPickStyle::Style SoPickStyle::BOUNDING_BOX

  Only compare pick intersection with the bounding boxes of
  shapes. This is usually much faster than SoPickStyle::SHAPE.
*/
/*!
  \var SoPickStyle::Style SoPickStyle::UNPICKABLE

  The geometry following this node in the scene will not be available
  for picking.
*/


/*!
  \var SoSFEnum SoPickStyle::style

  Which strategy to use for the picking actions for subsequent shapes
  in the scene graph.
*/

// *************************************************************************

SO_NODE_SOURCE(SoPickStyle);

/*!
  Constructor.
*/
SoPickStyle::SoPickStyle(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoPickStyle);

  SO_NODE_ADD_FIELD(style, (SoPickStyle::SHAPE));

  SO_NODE_DEFINE_ENUM_VALUE(Style, SHAPE);
  SO_NODE_DEFINE_ENUM_VALUE(Style, BOUNDING_BOX);
  SO_NODE_DEFINE_ENUM_VALUE(Style, UNPICKABLE);
  SO_NODE_SET_SF_ENUM_TYPE(style, Style);
}

/*!
  Destructor.
*/
SoPickStyle::~SoPickStyle()
{
}

// doc in super
void
SoPickStyle::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoPickStyle, SO_FROM_INVENTOR_2_0);

  SO_ENABLE(SoCallbackAction, SoPickStyleElement);
  SO_ENABLE(SoPickAction, SoPickStyleElement);
}

void
SoPickStyle::doAction(SoAction * action)
{
  if (!this->style.isIgnored()
      && !SoOverrideElement::getPickStyleOverride(action->getState())) {
    SoPickStyleElement::set(action->getState(), this,
                            (int32_t) this->style.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setPickStyleOverride(action->getState(), this, TRUE);
    }
  }
}

void
SoPickStyle::callback(SoCallbackAction * action)
{
  SoPickStyle::doAction(action);
}

void
SoPickStyle::pick(SoPickAction * action)
{
  SoPickStyle::doAction(action);
}
