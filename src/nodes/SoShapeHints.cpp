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
  \class SoShapeHints SoShapeHints.h Inventor/nodes/SoShapeHints.h
  \brief The SoShapeHints class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoShapeHints.h>



#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>

#include <Inventor/elements/SoGLShapeHintsElement.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/elements/SoCreaseAngleElement.h>
#include <Inventor/elements/SoOverrideElement.h>

#include <Inventor/actions/SoCallbackAction.h>

/*!
  \enum SoShapeHints::VertexOrdering
  FIXME: write documentation for enum
*/
/*!
  \var SoShapeHints::VertexOrdering SoShapeHints::UNKNOWN_ORDERING
  FIXME: write documentation for enum definition
*/
/*!
  \var SoShapeHints::VertexOrdering SoShapeHints::CLOCKWISE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoShapeHints::VertexOrdering SoShapeHints::COUNTERCLOCKWISE
  FIXME: write documentation for enum definition
*/

/*!
  \enum SoShapeHints::ShapeType
  FIXME: write documentation for enum
*/
/*!
  \var SoShapeHints::ShapeType SoShapeHints::UNKNOWN_SHAPE_TYPE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoShapeHints::ShapeType SoShapeHints::SOLID
  FIXME: write documentation for enum definition
*/

/*!
  \enum SoShapeHints::FaceType
  FIXME: write documentation for enum
*/
/*!
  \var SoShapeHints::FaceType SoShapeHints::UNKNOWN_FACE_TYPE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoShapeHints::FaceType SoShapeHints::CONVEX
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoShapeHints::vertexOrdering
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoShapeHints::shapeType
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoShapeHints::faceType
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoShapeHints::creaseAngle
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoShapeHints);

/*!
  Constructor.
*/
SoShapeHints::SoShapeHints()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoShapeHints);

  SO_NODE_ADD_FIELD(vertexOrdering,(UNKNOWN_ORDERING));
  SO_NODE_ADD_FIELD(shapeType,(UNKNOWN_SHAPE_TYPE));
  SO_NODE_ADD_FIELD(faceType,(CONVEX));
  SO_NODE_ADD_FIELD(creaseAngle,(0.0f));

  SO_NODE_DEFINE_ENUM_VALUE(VertexOrdering, UNKNOWN_ORDERING);
  SO_NODE_DEFINE_ENUM_VALUE(VertexOrdering, CLOCKWISE);
  SO_NODE_DEFINE_ENUM_VALUE(VertexOrdering, COUNTERCLOCKWISE);

  SO_NODE_DEFINE_ENUM_VALUE(ShapeType, UNKNOWN_SHAPE_TYPE);
  SO_NODE_DEFINE_ENUM_VALUE(ShapeType, SOLID);

  SO_NODE_DEFINE_ENUM_VALUE(FaceType, UNKNOWN_FACE_TYPE);
  SO_NODE_DEFINE_ENUM_VALUE(FaceType, CONVEX);

  SO_NODE_SET_SF_ENUM_TYPE(vertexOrdering, VertexOrdering);
  SO_NODE_SET_SF_ENUM_TYPE(shapeType, ShapeType);
  SO_NODE_SET_SF_ENUM_TYPE(faceType, FaceType);

}

/*!
  Destructor.
*/
SoShapeHints::~SoShapeHints()
{
}

/*!
  Does initialization common for all objects of the
  SoShapeHints class. This includes setting up the
  type system, among other things.
*/
void
SoShapeHints::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoShapeHints);

  SO_ENABLE(SoGetBoundingBoxAction, SoShapeHintsElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoCreaseAngleElement);

  SO_ENABLE(SoGLRenderAction, SoGLShapeHintsElement);
  SO_ENABLE(SoGLRenderAction, SoCreaseAngleElement);

  SO_ENABLE(SoPickAction, SoShapeHintsElement);
  SO_ENABLE(SoPickAction, SoCreaseAngleElement);

  SO_ENABLE(SoCallbackAction, SoShapeHintsElement);
  SO_ENABLE(SoCallbackAction, SoCreaseAngleElement);
}

/*!
  FIXME: write function documentation
*/
void
SoShapeHints::doAction(SoAction * action)
{
  SoState *state = action->getState();

  uint32_t flags = SoOverrideElement::getFlags(state);
#define TEST_OVERRIDE(bit) ((SoOverrideElement::bit & flags) != 0)

  // store current values, in case some are overridden or ignored
  SoShapeHintsElement::VertexOrdering vo;
  SoShapeHintsElement::ShapeType st;
  SoShapeHintsElement::FaceType ft;
  SoShapeHintsElement::get(state, vo, st, ft);

  if (!vertexOrdering.isIgnored() && !TEST_OVERRIDE(SHAPE_HINTS))
    vo = (SoShapeHintsElement::VertexOrdering) vertexOrdering.getValue();
  if (!shapeType.isIgnored() && !TEST_OVERRIDE(SHAPE_HINTS))
    st = (SoShapeHintsElement::ShapeType) shapeType.getValue();
  if (!faceType.isIgnored() && !TEST_OVERRIDE(SHAPE_HINTS))
    ft = (SoShapeHintsElement::FaceType) faceType.getValue();

  SoShapeHintsElement::set(action->getState(), this,
                           vo, st, ft);

  if (!creaseAngle.isIgnored() && !TEST_OVERRIDE(CREASE_ANGLE))
    SoCreaseAngleElement::set(state, this,
                              creaseAngle.getValue());
#undef TEST_OVERRIDE
}

/*!
  FIXME: write function documentation
*/
void
SoShapeHints::GLRender(SoGLRenderAction * action)
{
  SoShapeHints::doAction((SoAction *)action);
}

/*!
  FIXME: write function documentation
*/
void
SoShapeHints::callback(SoCallbackAction * action)
{
  SoShapeHints::doAction((SoAction *)action);
}

/*!
  FIXME: write function documentation
*/
void
SoShapeHints::pick(SoPickAction * action)
{
  SoShapeHints::doAction((SoAction *)action);
}

/*!
  FIXME: write function documentation
*/
void
SoShapeHints::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoShapeHints::doAction((SoAction *)action);
}
