/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoComplexity SoComplexity.h Inventor/nodes/SoComplexity.h
  \brief The SoComplexity class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoComplexity.h>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/elements/SoOverrideElement.h>

/*!
  \enum SoComplexity::Type
  FIXME: write documentation for enum
*/
/*!
  \var SoComplexity::Type SoComplexity::OBJECT_SPACE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoComplexity::Type SoComplexity::SCREEN_SPACE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoComplexity::Type SoComplexity::BOUNDING_BOX
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoComplexity::type
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoComplexity::value
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoComplexity::textureQuality
  FIXME: write documentation for field
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

/*!
  Does initialization common for all objects of the
  SoComplexity class. This includes setting up the
  type system, among other things.
*/
void
SoComplexity::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoComplexity);

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

/*!
  FIXME: write function documentation
*/
void
SoComplexity::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoComplexity::doAction(action);
}

/*!
  FIXME: write function documentation
*/
void
SoComplexity::GLRender(SoGLRenderAction * action)
{
  SoComplexity::doAction(action);
  if (!textureQuality.isIgnored()) {
    SoTextureQualityElement::set(action->getState(), this,
                                 textureQuality.getValue());
  }
}

/*!
  FIXME: write doc
*/
void
SoComplexity::doAction(SoAction *action)
{
  SoState * state = action->getState();
  if (!value.isIgnored() && !SoOverrideElement::getComplexityOverride(state))
    SoComplexityElement::set(state, value.getValue());
  if (!type.isIgnored() && !SoOverrideElement::getComplexityTypeOverride(state))
    SoComplexityTypeElement::set(state, (SoComplexityTypeElement::Type)
                                 type.getValue());
}

/*!
  FIXME: write doc
*/
void
SoComplexity::callback(SoCallbackAction *action)
{
  SoComplexity::doAction((SoAction*)action);
  if (!textureQuality.isIgnored()) {
    SoTextureQualityElement::set(action->getState(), this,
                                 textureQuality.getValue());
  }
}

/*!
  FIXME: write doc
*/
void
SoComplexity::pick(SoPickAction *action)
{
  SoComplexity::doAction(action);
}

/*!
  FIXME: write doc
*/
void
SoComplexity::getPrimitiveCount(SoGetPrimitiveCountAction *action)
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
