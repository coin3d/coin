/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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
  \class SoLightModel SoLightModel.h Inventor/nodes/SoLightModel.h
  \brief The SoLightModel class is a node for specifying the model for geometry lighting.
  \ingroup nodes

  Use nodes of this type to set up how lighting should affect
  subsequent geometry in the scene.
*/

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLLightModelElement.h>
#include <Inventor/elements/SoOverrideElement.h>

/*!
  \enum SoLightModel::Model
  Enumerates available light models.
*/
/*!
  \var SoLightModel::Model SoLightModel::BASE_COLOR

  Use the current diffuse material color for subsequent geometry, and
  do not let any lightsources influence the appearance of the
  rendering primitives.
*/
/*!
  \var SoLightModel::Model SoLightModel::PHONG

  Use Phong-style shading for the geometry.
*/


/*!
  \var SoSFEnum SoLightModel::model

  Lightmodel to use. Defaults to SoLightModel::PHONG.
*/

// *************************************************************************

SO_NODE_SOURCE(SoLightModel);

/*!
  Constructor.
*/
SoLightModel::SoLightModel(void)
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

// Doc from superclass.
void
SoLightModel::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoLightModel);

  SO_ENABLE(SoCallbackAction, SoLightModelElement);
  SO_ENABLE(SoGLRenderAction, SoGLLightModelElement);
}

// Doc from superclass.
void
SoLightModel::GLRender(SoGLRenderAction * action)
{
  SoLightModel::doAction(action);
}

// Doc from superclass.
void
SoLightModel::doAction(SoAction * action)
{
  if (!model.isIgnored()
      && !SoOverrideElement::getLightModelOverride(action->getState())) {
    SoLightModelElement::set(action->getState(), this,
                             (SoLightModelElement::Model)model.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setLightModelOverride(action->getState(), this, TRUE);
    }
  }
}

// Doc from superclass.
void
SoLightModel::callback(SoCallbackAction * action)
{
  SoLightModel::doAction(action);
}
