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
  \class SoShapeHints SoShapeHints.h Inventor/nodes/SoShapeHints.h
  \brief The SoShapeHints class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoShapeHints.h>



#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
#include <Inventor/actions/SoPickAction.h>
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT)
#include <Inventor/elements/SoGLShapeHintsElement.h>
#endif // !COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT
#if !defined(COIN_EXCLUDE_SOSHAPEHINTSELEMENT)
#include <Inventor/elements/SoShapeHintsElement.h>
#endif // !COIN_EXCLUDE_SOSHAPEHINTSELEMENT
#if !defined(COIN_EXCLUDE_SOCREASEANGLEELEMENT)
#include <Inventor/elements/SoCreaseAngleElement.h>
#endif // !COIN_EXCLUDE_SOCREASEANGLEELEMENT
#if !defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
#include <Inventor/elements/SoOverrideElement.h>
#endif // !COIN_EXCLUDE_SOOVERRIDEELEMENT

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

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  SO_ENABLE(SoGetBoundingBoxAction, SoShapeHintsElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoCreaseAngleElement);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoGLShapeHintsElement);
  SO_ENABLE(SoGLRenderAction, SoCreaseAngleElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
  SO_ENABLE(SoPickAction, SoShapeHintsElement);
  SO_ENABLE(SoPickAction, SoCreaseAngleElement);
#endif // !COIN_EXCLUDE_SOPICKACTION
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write function documentation
*/
void 
SoShapeHints::doAction(SoAction * action)
{
  SoState *state = action->getState();

#if !defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
  uint32_t flags = SoOverrideElement::getFlags(state);
#define TEST_OVERRIDE(bit) ((SoOverrideElement::bit & flags) != 0)
#else // COIN_EXCLUDE_SOOVERRIDEELEMENT
#define TEST_OVERRIDE(x,y) FALSE // a neat little trick (don't nag, Morten :-)
#endif // COIN_EXCLUDE_SOOVERRIDEELEMENT
  
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
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoShapeHints::GLRender(SoGLRenderAction * action)
{
  SoShapeHints::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write function documentation
*/
void 
SoShapeHints::callback(SoCallbackAction * action)
{
  SoShapeHints::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write function documentation
*/
void 
SoShapeHints::pick(SoPickAction * action)
{
  SoShapeHints::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void 
SoShapeHints::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoShapeHints::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
