/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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
  \class SoTransparencyType SoTransparencyType.h Inventor/nodes/SoTransparencyType.h
  \brief The SoTransparencyType class is a node for setting the transparency type for shapes.
  \ingroup nodes

  In earlier versions of Coin/Open Inventor it was only possible to
  set the transparency mode globally for an entire scene graph, which
  could be inconvienient if different transparency types was wanted
  for different shapes.

  This node was not part of the Open Inventor 2.1 API, and is an
  extension specific to Coin.

  \since 2001-11-05
*/

#include <Inventor/nodes/SoTransparencyType.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoGLPolygonStippleElement.h>

/*!
  \enum SoTransparencyType::Type
  Enumeration of available transparency types. See documentation in
  SoGLRenderAction for a description of the different types.
*/

/*!
  \var SoSFEnum SoTransparencyType::value

  The transparency type to use for subsequent shape nodes in the scene
  graph.
*/


// *************************************************************************

SO_NODE_SOURCE(SoTransparencyType);

/*!
  Constructor.
*/
SoTransparencyType::SoTransparencyType(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTransparencyType);

  SO_NODE_ADD_FIELD(value, (SCREEN_DOOR));

  SO_NODE_DEFINE_ENUM_VALUE(Type, SCREEN_DOOR);
  SO_NODE_DEFINE_ENUM_VALUE(Type, ADD);
  SO_NODE_DEFINE_ENUM_VALUE(Type, DELAYED_ADD);
  SO_NODE_DEFINE_ENUM_VALUE(Type, BLEND);
  SO_NODE_DEFINE_ENUM_VALUE(Type, DELAYED_BLEND);
  SO_NODE_DEFINE_ENUM_VALUE(Type, SORTED_OBJECT_ADD);
  SO_NODE_DEFINE_ENUM_VALUE(Type, SORTED_OBJECT_BLEND);
  SO_NODE_DEFINE_ENUM_VALUE(Type, SORTED_OBJECT_SORTED_TRIANGLE_ADD);
  SO_NODE_DEFINE_ENUM_VALUE(Type, SORTED_OBJECT_SORTED_TRIANGLE_BLEND);
  SO_NODE_DEFINE_ENUM_VALUE(Type, NONE);

  SO_NODE_SET_SF_ENUM_TYPE(value, Type);
}


/*!
  Destructor.
*/
SoTransparencyType::~SoTransparencyType()
{
}

// Doc from superclass.
void
SoTransparencyType::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTransparencyType);
}

// Doc from superclass.
void
SoTransparencyType::GLRender(SoGLRenderAction * action)
{
  SoTransparencyType::doAction(action);
  if (this->value.getValue() != (int) SoGLRenderAction::SCREEN_DOOR) {
    SoGLPolygonStippleElement::set(action->getState(), FALSE);
  }
}

// Doc from superclass.
void
SoTransparencyType::doAction(SoAction * action)
{
  if (!this->value.isIgnored()
      && !SoOverrideElement::getTransparencyTypeOverride(action->getState())) {
    SoShapeStyleElement::setTransparencyType(action->getState(),
                                             this->value.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setTransparencyTypeOverride(action->getState(), this, TRUE);
    }
  }
}

// Doc from superclass.
void
SoTransparencyType::callback(SoCallbackAction * action)
{
  SoTransparencyType::doAction((SoAction *)action);
}
