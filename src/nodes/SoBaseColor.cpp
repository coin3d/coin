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
  \class SoBaseColor SoBaseColor.h Inventor/nodes/SoBaseColor.h
  \brief The SoBaseColor class provides a node type for convenient setting of the base material color.
  \ingroup nodes

  If you want to just set the diffuse color of the following geometry,
  you can use this node for simplicity.

  \sa SoMaterial
*/

#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoGLDiffuseColorElement.h>
#include <Inventor/elements/SoOverrideElement.h>


/*!
  \var SoMFColor SoBaseColor::rgb

  Color values. Default value of field is to have a single grey color.
*/

// *************************************************************************

SO_NODE_SOURCE(SoBaseColor);

/*!
  Constructor.
*/
SoBaseColor::SoBaseColor()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoBaseColor);

  SO_NODE_ADD_FIELD(rgb, (SbColor(0.8f, 0.8f, 0.8f)));
}

/*!
  Destructor.
*/
SoBaseColor::~SoBaseColor()
{
}

// Doc from superclass.
void
SoBaseColor::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoBaseColor);

  SO_ENABLE(SoGLRenderAction, SoGLDiffuseColorElement);

  SO_ENABLE(SoCallbackAction, SoDiffuseColorElement);
  SO_ENABLE(SoPickAction, SoDiffuseColorElement);
  SO_ENABLE(SoCallbackAction, SoDiffuseColorElement);
}

// Doc from superclass.
void
SoBaseColor::GLRender(SoGLRenderAction * action)
{
  SoBaseColor::doAction(action);
}

// Doc from superclass.
void
SoBaseColor::doAction(SoAction * action)
{
  SoState * state = action->getState();

  if (!rgb.isIgnored() && !SoOverrideElement::getDiffuseColorOverride(state)) {
    SoDiffuseColorElement::set(state,
                               this,
                               rgb.getNum(),
                               rgb.getValues(0));
    if (this->isOverride()) {
      SoOverrideElement::setDiffuseColorOverride(state, this, TRUE);
    }
  }
}

// Doc from superclass.
void
SoBaseColor::callback(SoCallbackAction * action)
{
  SoBaseColor::doAction(action);
}

// Doc from superclass.
void
SoBaseColor::pick(SoPickAction * action)
{
  SoBaseColor::doAction(action);
}
