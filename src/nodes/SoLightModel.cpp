/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoLightModel SoLightModel.h Inventor/nodes/SoLightModel.h
  \brief The SoLightModel class is a node for specifying the model for geometry lighting.
  \ingroup nodes

  Use nodes of this type to set up how lighting should affect
  subsequent geometry in the scene.

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    LightModel {
        model PHONG
    }
  \endcode
*/

// *************************************************************************

#include <Inventor/actions/SoCallbackAction.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/elements/SoLightModelElement.h>

#include "nodes/SoSubNodeP.h"

// *************************************************************************

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
  SO_NODE_INTERNAL_INIT_CLASS(SoLightModel, SO_FROM_INVENTOR_1);

  SO_ENABLE(SoCallbackAction, SoLazyElement);
  SO_ENABLE(SoGLRenderAction, SoGLLazyElement);

  SO_ENABLE(SoCallbackAction, SoLightModelElement);
  SO_ENABLE(SoGLRenderAction, SoLightModelElement);
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
    SoLazyElement::setLightModel(action->getState(),
                                 (int32_t) this->model.getValue());
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
