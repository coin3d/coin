/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoComplexity SoComplexity.h Inventor/nodes/SoComplexity.h
  \brief The SoComplexity class is a node type which is used to set the tradeoff between quality and performance.
  \ingroup nodes

  By inserting SoComplexity nodes in the scene graph, you can control
  the accuracy by which complex shapes are rendered and the quality of
  the texture mapping used for geometry in the scene.

  Shape nodes like SoCone, SoSphere, SoCylinder and others, will
  render with fewer polygons and thereby improve performance, if the
  complexity value of the traversal state is set to a lower value.

  By using the SoComplexity::type field, you may also choose to render
  the scene graph (or parts of it) just as wireframe bounding
  boxes. This will improve rendering performance \e a \e lot, and can
  sometimes be used in particular situations where responsiveness is
  more important than appearance.

  Texture mapping can be done in an expensive but attractive looking
  manner, or in a quick way which doesn't look as appealing by
  modifying the value of the SoComplexity::textureQuality field. By
  setting the SoComplexity::textureQuality field to a value of 0.0,
  you can also turn texturemapping completely off.
*/

#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoTextureOverrideElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/errors/SoDebugError.h>

/*!
  \enum SoComplexity::Type

  The available values for the SoComplexity::type field.
*/
/*!
  \var SoComplexity::Type SoComplexity::OBJECT_SPACE

  Use the SoComplexity::value in calculations based on the geometry's
  size in world-space 3D.
*/
/*!
  \var SoComplexity::Type SoComplexity::SCREEN_SPACE

  Use the SoComplexity::value in calculations based on the geometry's
  size when projected onto the rendering area. This is often a good
  way to make sure that objects are rendered with as low complexity as
  possible while still retaining their appearance for the user.
*/
/*!
  \var SoComplexity::Type SoComplexity::BOUNDING_BOX

  Render further geometry in the scene graph as bounding boxes only
  for superfast rendering.
*/


/*!
  \var SoSFEnum SoComplexity::type
  Set rendering type. Default value is SoComplexity::OBJECT_SPACE.
*/
/*!
  \var SoSFFloat SoComplexity::value

  Complexity value, valid settings range from 0.0 (worst appearance,
  best perfomance) to 1.0 (optimal appearance, lowest rendering
  speed). Default value for the field is 0.5.

  Note that without any SoComplexity nodes in the scene graph,
  geometry will render as if there was a SoComplexity node present
  with SoComplexity::value set to 1.0.
*/
/*!
  \var SoSFFloat SoComplexity::textureQuality

  Sets the quality value for texturemapping. Valid range is from 0.0
  (texturemapping off, rendering will be much faster for most
  platforms) to 1.0 (best quality, rendering might be slow).

  The same value for this field on different platforms can yield
  varying results, depending on the quality of the underlying
  rendering hardware.

  Note that this field influences the behavior of the SoTexture2 node,
  \e not the shape nodes. There is an important consequence of this
  that the application programmer need to know about: you need to
  insert your SoComplexity node(s) \e before the SoTexture2 node(s) in
  the scenegraph for them to have any influence on the textured
  shapes.
*/


// *************************************************************************

SO_NODE_SOURCE(SoComplexity);

/*!
  Constructor.
*/
SoComplexity::SoComplexity(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoComplexity);

  SO_NODE_ADD_FIELD(type, (SoComplexity::OBJECT_SPACE));
  SO_NODE_ADD_FIELD(value, (0.5f));
  SO_NODE_ADD_FIELD(textureQuality, (0.5f));

  SO_NODE_DEFINE_ENUM_VALUE(Type, SCREEN_SPACE);
  SO_NODE_DEFINE_ENUM_VALUE(Type, OBJECT_SPACE);
  SO_NODE_DEFINE_ENUM_VALUE(Type, BOUNDING_BOX);
  SO_NODE_SET_SF_ENUM_TYPE(type, Type);
}

/*!
  Destructor.
*/
SoComplexity::~SoComplexity()
{
}

// Doc from superclass.
void
SoComplexity::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoComplexity, SO_FROM_INVENTOR_1);

  SO_ENABLE(SoGetBoundingBoxAction, SoComplexityElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoComplexityTypeElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoShapeStyleElement);

  SO_ENABLE(SoGLRenderAction, SoComplexityElement);
  SO_ENABLE(SoGLRenderAction, SoComplexityTypeElement);
  SO_ENABLE(SoGLRenderAction, SoShapeStyleElement);
  SO_ENABLE(SoGLRenderAction, SoTextureQualityElement);

  SO_ENABLE(SoCallbackAction, SoComplexityElement);
  SO_ENABLE(SoCallbackAction, SoComplexityTypeElement);
  SO_ENABLE(SoCallbackAction, SoShapeStyleElement);
  SO_ENABLE(SoCallbackAction, SoTextureQualityElement);

  SO_ENABLE(SoPickAction, SoComplexityElement);
  SO_ENABLE(SoPickAction, SoComplexityTypeElement);
  SO_ENABLE(SoPickAction, SoShapeStyleElement);

  SO_ENABLE(SoGetPrimitiveCountAction, SoComplexityElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoComplexityTypeElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoShapeStyleElement);
}

// Doc from superclass.
void
SoComplexity::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoComplexity::doAction(action);
}

// Doc from superclass.
void
SoComplexity::GLRender(SoGLRenderAction * action)
{
  SoComplexity::doAction(action);

  SoState * state = action->getState();
  if (!this->textureQuality.isIgnored() &&
      !SoTextureOverrideElement::getQualityOverride(state)) {
    SoTextureQualityElement::set(state, this,
                                 this->textureQuality.getValue());
    if (this->isOverride()) {
      SoTextureOverrideElement::setQualityOverride(state, TRUE);
    }
  }
}

// Doc from superclass.
void
SoComplexity::doAction(SoAction * action)
{
  SoState * state = action->getState();
  if (!value.isIgnored() && !SoOverrideElement::getComplexityOverride(state)) {
    SoComplexityElement::set(state, value.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setComplexityOverride(state, this, TRUE);
    }
  }
  if (!type.isIgnored() && !SoOverrideElement::getComplexityTypeOverride(state)) {
    SoComplexityTypeElement::set(state, (SoComplexityTypeElement::Type)
                                 type.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setComplexityTypeOverride(state, this, TRUE);
    }
  }
}

// Doc from superclass.
void
SoComplexity::callback(SoCallbackAction * action)
{
  SoComplexity::doAction((SoAction *)action);
  if (!this->textureQuality.isIgnored()) {
    SoTextureQualityElement::set(action->getState(), this,
                                 this->textureQuality.getValue());
  }
}

// Doc from superclass.
void
SoComplexity::pick(SoPickAction * action)
{
  SoComplexity::doAction(action);
}

// Doc from superclass.
void
SoComplexity::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoState * state = action->getState();
  if (!value.isIgnored() && !SoOverrideElement::getComplexityOverride(state))
    SoComplexityElement::set(state, value.getValue());

  // complexity type element is always OBJECT_SPACE for this action.
  // this is somewhat odd. If it had been possible to supply a
  // viewport in the action constructor, it would be possible
  // to also calculate SCREEN_SPACE complexity.
  // pederb, 1999-11-25
}
