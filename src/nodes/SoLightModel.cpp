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
  \class SoLightModel SoLightModel.h Inventor/nodes/SoLightModel.h
  \brief The SoLightModel class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoLightModel.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLLightModelElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/actions/SoCallbackAction.h>

/*!
  \enum SoLightModel::Model
  FIXME: write documentation for enum
*/
/*!
  \var SoLightModel::Model SoLightModel::BASE_COLOR
  FIXME: write documentation for enum definition
*/
/*!
  \var SoLightModel::Model SoLightModel::PHONG
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoLightModel::model
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoLightModel);

/*!
  Constructor.
*/
SoLightModel::SoLightModel()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoLightModel);

  SO_NODE_ADD_FIELD(model, (SoLightModel::PHONG));
  SO_NODE_DEFINE_ENUM_VALUE(Model, BASE_COLOR);
  SO_NODE_DEFINE_ENUM_VALUE(Model, PHONG);
  SO_NODE_SET_SF_ENUM_TYPE(model, Model);
}

/*!
  Destructor.
*/
SoLightModel::~SoLightModel()
{
}

/*!
  Does initialization common for all objects of the
  SoLightModel class. This includes setting up the
  type system, among other things.
*/
void
SoLightModel::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoLightModel);

  SO_ENABLE(SoGLRenderAction, SoGLLightModelElement);

  SO_ENABLE(SoCallbackAction, SoLightModelElement);
}

/*!
  FIXME: write function documentation
*/
void
SoLightModel::GLRender(SoGLRenderAction * action)
{
  SoLightModel::doAction(action);
}

/*!
  FIXME: write doc
 */
void
SoLightModel::doAction(SoAction *action)
{
  if (!model.isIgnored()
      && !SoOverrideElement::getLightModelOverride(action->getState())) {
    SoLightModelElement::set(action->getState(), this,
                             (SoLightModelElement::Model)model.getValue());
  }
}

/*!
  FIXME: write doc
 */
void
SoLightModel::callback(SoCallbackAction *action)
{
  SoLightModel::doAction(action);
}
