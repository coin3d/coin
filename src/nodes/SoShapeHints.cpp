/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoShapeHints SoShapeHints.h Inventor/nodes/SoShapeHints.h
  \brief The SoShapeHints class is a node containing hints about how to render geometry.
  \ingroup nodes

  The SoShapeHints node is used to set up clues to the rendering
  subsystem about how particular aspects of the subsequent geometry in
  the scene graph should be drawn.

  The default settings of the rendering system is tuned towards
  programmer convenience. For instance, the default is to render both
  sides of all polygons to make sure we avoid any "holes" in the
  geometry if the vertex ordering should happen to differ for
  different polygons.

  If the programmer gives up some of this convenience and uses
  SoShapeHints nodes to more closely specify information about the
  scene graph geometry, the clues given by the SoShapeHints node(s)
  will then be used by the rendering subsystem to avoid certain costly
  operations. Significant gains in rendering speed could be seen as a
  result.


  Be aware that the way backface culling and two-sided lighting is
  decided by the rendering system is not at all intuitive.  Here are
  the common rules of how primitive shapes will render themselves with
  regard to how the SoShapeHints::vertexOrdering and
  SoShapeHints::shapeType fields are set:

  <ul>

  <li>vertexOrdering == CLOCKWISE | COUNTERCLOCKWISE, shapeType ==
      SOLID: causes primitives to be backface culled and rendered with
      one-sided lighting.
  
  <li>vertexOrdering == CLOCKWISE | COUNTERCLOCKWISE, shapeType ==
      UNKNOWN_SHAPE_TYPE: primitives are \e not backface culled and
      rendered with two-sided lighting.
  
  <li>vertexOrdering == UNKNOWN_ORDERING, any shapeType: primitives
      are \e not backface culled and rendered with one-sided lighting.
  
  </ul>

  (We find this strategy quite odd. It seems more sensible behavior
  that when vertexordering is unknown, two-sided lighting should be
  enabled, since it will be difficult to know if a normal points in or
  out of a polygon.  But for compatibility reasons we are of course
  sticking to the same behavior as the original SGI Inventor library.)
*/


#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoCreaseAngleElement.h>
#include <Inventor/elements/SoGLShapeHintsElement.h>
#include <Inventor/elements/SoOverrideElement.h>


/*!
  \enum SoShapeHints::VertexOrdering

  Enumeration of available ways to specify ordering of vertices for a
  polygon.
*/
/*!
  \var SoShapeHints::VertexOrdering SoShapeHints::UNKNOWN_ORDERING

  Ordering not known, render both sides of the polygon.
*/
/*!
  \var SoShapeHints::VertexOrdering SoShapeHints::CLOCKWISE

  Vertices are specified in a clockwise order.
*/
/*!
  \var SoShapeHints::VertexOrdering SoShapeHints::COUNTERCLOCKWISE

  Vertices are specified in a counter-clockwise order.
*/

/*!
  \enum SoShapeHints::ShapeType
  Enumeration of different shape types.
*/
/*!
  \var SoShapeHints::ShapeType SoShapeHints::UNKNOWN_SHAPE_TYPE
  Nothing known about the shape, be conservative when rendering.
*/
/*!
  \var SoShapeHints::ShapeType SoShapeHints::SOLID

  The subsequent shapes in the graph are all known to be completely
  "closed", solid 3D shapes.
*/

/*!
  \enum SoShapeHints::FaceType
  Enumeration of polygon face types.
*/
/*!
  \var SoShapeHints::FaceType SoShapeHints::UNKNOWN_FACE_TYPE

  Nothing known about subsequent polygon data, be conservative when
  rendering.
*/
/*!
  \var SoShapeHints::FaceType SoShapeHints::CONVEX

  Subsequent faces are all convex, so turn off the check for and
  tessellation of inconvex faces.
*/


/*!
  \var SoSFEnum SoShapeHints::vertexOrdering

  Specifies how vertices are ordered for polygon faces.

  Set this field to SoShapeHints::CLOCKWISE or
  SoShapeHints::COUNTERCLOCKWISE if possible to turn on backface
  culling and thereby optimize rendering.

  Default value is SoShapeHints::UNKNOWN_ORDERING.
*/
/*!
  \var SoSFEnum SoShapeHints::shapeType

  Hint about whether or not shapes are known to be "closed".  Default
  value is SoShapeHints::UNKNOWN_SHAPE_TYPE.
*/
/*!
  \var SoSFEnum SoShapeHints::faceType

  Hint about whether or not polygon faces are known to be convex.
  Default value is SoShapeHints::CONVEX.
*/
/*!
  \var SoSFFloat SoShapeHints::creaseAngle

  When normals are automatically generated by Coin (i.e. SoNormal
  nodes are not used), this is the smallest angle between two faces
  where we would still calculate normals to do flatshading.

  If the angle between the normals of two neighboring faces is less
  than the value of this field, the faces will be smoothshaded around
  their common edge.
*/

// *************************************************************************

SO_NODE_SOURCE(SoShapeHints);

/*!
  Constructor.
*/
SoShapeHints::SoShapeHints(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoShapeHints);

  SO_NODE_ADD_FIELD(vertexOrdering, (UNKNOWN_ORDERING));
  SO_NODE_ADD_FIELD(shapeType, (UNKNOWN_SHAPE_TYPE));
  SO_NODE_ADD_FIELD(faceType, (CONVEX));
  SO_NODE_ADD_FIELD(creaseAngle, (0.0f));

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

// doc in super
void
SoShapeHints::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoShapeHints);

  SO_ENABLE(SoCallbackAction, SoCreaseAngleElement);
  SO_ENABLE(SoCallbackAction, SoShapeHintsElement);
  SO_ENABLE(SoGLRenderAction, SoCreaseAngleElement);
  SO_ENABLE(SoGLRenderAction, SoGLShapeHintsElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoCreaseAngleElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoShapeHintsElement);
  SO_ENABLE(SoPickAction, SoCreaseAngleElement);
  SO_ENABLE(SoPickAction, SoShapeHintsElement);
}

void
SoShapeHints::doAction(SoAction * action)
{
  SoState * state = action->getState();

  uint32_t flags = SoOverrideElement::getFlags(state);
#define TEST_OVERRIDE(bit) ((SoOverrideElement::bit & flags) != 0)

  // store current values, in case some are overridden or ignored
  SoShapeHintsElement::VertexOrdering vo;
  SoShapeHintsElement::ShapeType st;
  SoShapeHintsElement::FaceType ft;
  SoShapeHintsElement::get(state, vo, st, ft);

  if (!this->vertexOrdering.isIgnored() && !TEST_OVERRIDE(SHAPE_HINTS)) {
    vo = (SoShapeHintsElement::VertexOrdering) this->vertexOrdering.getValue();
    if (this->isOverride()) {
      SoOverrideElement::setShapeHintsOverride(state, this, TRUE);
    }
  }
  if (!this->shapeType.isIgnored() && !TEST_OVERRIDE(SHAPE_HINTS)) {
    st = (SoShapeHintsElement::ShapeType) this->shapeType.getValue();
    if (this->isOverride()) {
      SoOverrideElement::setShapeHintsOverride(state, this, TRUE);
    }
  }
  if (!this->faceType.isIgnored() && !TEST_OVERRIDE(SHAPE_HINTS)) {
    ft = (SoShapeHintsElement::FaceType) this->faceType.getValue();
    if (this->isOverride()) {
      SoOverrideElement::setShapeHintsOverride(state, this, TRUE);
    }
  }
  SoShapeHintsElement::set(action->getState(), this,
                           vo, st, ft);

  if (!this->creaseAngle.isIgnored() && !TEST_OVERRIDE(CREASE_ANGLE)) {
    float ca = this->creaseAngle.getValue();
    // Fix to handle VRML1 ShapeHints nodes correctly. The default
    // creaseAngle value for VRML1 is 0.5, while it's 0.0 for
    // Inventor 2.1
    if (this->creaseAngle.isDefault() &&
        (this->getNodeType() == SoNode::VRML1) &&
        (ca == 0.0f)) {
      ca = 0.5f;
    }
    SoCreaseAngleElement::set(state, this, ca);
    if (this->isOverride()) {
      SoOverrideElement::setCreaseAngleOverride(state, this, TRUE);
    }
  }
#undef TEST_OVERRIDE
}

void
SoShapeHints::GLRender(SoGLRenderAction * action)
{
  SoShapeHints::doAction(action);
}

void
SoShapeHints::callback(SoCallbackAction * action)
{
  SoShapeHints::doAction(action);
}

void
SoShapeHints::pick(SoPickAction * action)
{
  SoShapeHints::doAction(action);
}

void
SoShapeHints::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoShapeHints::doAction(action);
}
