/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoVRMLColor SoVRMLColor.h Inventor/VRMLnodes/SoVRMLColor.h
  \brief The SoVRMLColor class is used to specify multiple colors for a single shape.
*/

/*!
  \var SoMFColor SoVRMLColor::color
  The color field. Empty by default.
*/

#include <Inventor/VRMLnodes/SoVRMLColor.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoGLDiffuseColorElement.h>
#include <Inventor/elements/SoOverrideElement.h>

SO_NODE_SOURCE(SoVRMLColor);

// doc in parent
void
SoVRMLColor::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLColor, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLColor::SoVRMLColor(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLColor);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(color);
}

/*!
  Destructor.
*/
SoVRMLColor::~SoVRMLColor() // virtual, protected
{
}

// Doc in parent
void
SoVRMLColor::doAction(SoAction * action)
{
  SoState * state = action->getState();
  if (this->color.getNum() &&
      !this->color.isIgnored() &&
      !SoOverrideElement::getDiffuseColorOverride(state)) {
    SoDiffuseColorElement::set(state,
                               this,
                               this->color.getNum(),
                               this->color.getValues(0));
    if (this->isOverride()) {
      SoOverrideElement::setDiffuseColorOverride(state, this, TRUE);
    }
  }
}

// Doc in parent
void
SoVRMLColor::GLRender(SoGLRenderAction * action)
{
  SoVRMLColor::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLColor::callback(SoCallbackAction * action)
{
  SoVRMLColor::doAction((SoAction*) action);
}
