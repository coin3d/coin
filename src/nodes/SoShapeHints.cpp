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
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT)
#include <Inventor/elements/SoGLShapeHintsElement.h>
#endif // !COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT
#if !defined(COIN_EXCLUDE_SOSHAPEHINTSELEMENT)
#include <Inventor/elements/SoShapeHintsElement.h>
#endif // !COIN_EXCLUDE_SOSHAPEHINTSELEMENT
#if !defined(COIN_EXCLUDE_SOCREASEANGLEELEMENT)
#include <Inventor/elements/SoCreaseAngleElement.h>
#endif // !COIN_EXCLUDE_SOCREASEANGLEELEMENT

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

//$ BEGIN TEMPLATE NodeSource(SoShapeHints)
SoType SoShapeHints::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoShapeHints node class.
*/
void *
SoShapeHints::createInstance(void)
{
  return new SoShapeHints;
}

/*!
  Returns the unique type identifier for the SoShapeHints class.
*/
SoType
SoShapeHints::getClassTypeId(void)
{
  return SoShapeHints::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoShapeHints::getTypeId(void) const
{
  return SoShapeHints::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoShapeHints::SoShapeHints()
{
//$ BEGIN TEMPLATE NodeConstructor(SoShapeHints)
  // Make sure the class has been initialized.
  assert(SoShapeHints::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

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
//$ BEGIN TEMPLATE InitNodeSource(ShapeHints)
  // Make sure we only initialize once.
  assert(SoShapeHints::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoShapeHints::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "ShapeHints",
                       &SoShapeHints::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  SO_ENABLE(SoGetBoundingBoxAction, SoShapeHintsElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoCreaseAngleElement);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoGLShapeHintsElement);
  SO_ENABLE(SoGLRenderAction, SoCreaseAngleElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoShapeHints::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write function documentation
*/
void 
SoShapeHints::doAction(SoAction * action)
{
  // FIXME: this is not in accordance with OIV. 19990315 mortene.
  SoShapeHintsElement::set(action->getState(), this,
			   (SoShapeHintsElement::VertexOrdering)
			   vertexOrdering.getValue(),
			   (SoShapeHintsElement::ShapeType)
			   shapeType.getValue(),
			   (SoShapeHintsElement::FaceType)
			   faceType.getValue());

  SoCreaseAngleElement::set(action->getState(), this,
			    creaseAngle.getValue());
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
