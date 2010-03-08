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
  \class SoShapeHintsV10 SoShapeHintsV10.h
  \brief The SoShapeHintsV10 class is a node is for Inventor V1.0 support only.
  \ingroup nodes

  \sa SoShapeHints
*/

#include "upgraders/SoShapeHintsV10.h"

#include <Inventor/nodes/SoShapeHints.h>

#include "nodes/SoSubNodeP.h"

// *************************************************************************

SO_NODE_SOURCE(SoShapeHintsV10);

/*!
  Constructor.
*/
SoShapeHintsV10::SoShapeHintsV10()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoShapeHintsV10);

  SO_NODE_ADD_FIELD(hints, (SURFACE | UNORDERED | CONCAVE));
  SO_NODE_ADD_FIELD(creaseAngle, (0.0));

  SO_NODE_DEFINE_ENUM_VALUE(Hint, SURFACE);
  SO_NODE_DEFINE_ENUM_VALUE(Hint, UNORDERED);
  SO_NODE_DEFINE_ENUM_VALUE(Hint, CONCAVE);
  SO_NODE_DEFINE_ENUM_VALUE(Hint, SOLID);
  SO_NODE_DEFINE_ENUM_VALUE(Hint, ORDERED);
  SO_NODE_DEFINE_ENUM_VALUE(Hint, CONVEX);

  SO_NODE_SET_SF_ENUM_TYPE(hints, Hint);
}

/*!
  Destructor.
*/
SoShapeHintsV10::~SoShapeHintsV10()
{
}

// Doc from superclass.
void
SoShapeHintsV10::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoShapeHintsV10, SoNode::INVENTOR_1);
}

SoShapeHints *
SoShapeHintsV10::createUpgrade(void) const
{
  SoShapeHints * pp = new SoShapeHints;
  pp->ref();

  Hint hint = (Hint) this->hints.getValue();

  pp->shapeType = (hint & SOLID) ?
    SoShapeHintsElement::SOLID :
    SoShapeHintsElement::UNKNOWN_SHAPE_TYPE;
  pp->vertexOrdering = (hint & ORDERED) ?
    SoShapeHintsElement::COUNTERCLOCKWISE :
    SoShapeHintsElement::UNKNOWN_ORDERING;
  pp->faceType = (hint & CONVEX) ?
    SoShapeHintsElement::CONVEX :
    SoShapeHintsElement::UNKNOWN_FACE_TYPE;
  
  if (this->hints.isIgnored()) {
    pp->shapeType.setIgnored(TRUE);
    pp->vertexOrdering.setIgnored(TRUE);
    pp->faceType.setIgnored(TRUE);
  }

  pp->creaseAngle = this->creaseAngle.getValue();
  if (this->creaseAngle.isIgnored()) {
    pp->creaseAngle.setIgnored(TRUE);
  }

  pp->unrefNoDelete();
  return pp;
}
