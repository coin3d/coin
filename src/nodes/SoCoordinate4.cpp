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
  \class SoCoordinate4 SoCoordinate4.h Inventor/nodes/SoCoordinate4.h
  \brief The SoCoordinate4 class is a node for providing coordinates to shape nodes.
  \ingroup nodes

  When encountering nodes of this type during traversal, the
  coordinates it contains will be put on the statestack for later use
  by shape nodes of types which needs coordinate sets (like SoFaceSet
  nodes or SoPointSet nodes).

  The fourth element of the coordinate vectors is used for
  normalization. A node of this type where all the normalization
  values are equal to 1.0 is the equivalent of setting up an
  SoCoordinate3 node.

  Note that an SoCoordinate4 node will \e replace the coordinates
  already present in the state (if any).

  \sa SoCoordinate3
*/

#include <Inventor/nodes/SoCoordinate4.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoGLCoordinateElement.h>

/*!
  \var SoMFVec4f SoCoordinate4::point

  Coordinate set of 3D points as 4D vectors (each vector contains a 3D
  coordinate plus normalization value).
*/


// *************************************************************************

SO_NODE_SOURCE(SoCoordinate4);

/*!
  Constructor.
*/
SoCoordinate4::SoCoordinate4(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoCoordinate4);

  SO_NODE_ADD_FIELD(point, (SbVec4f(0.0f, 0.0f, 0.0f, 1.0f)));
}

/*!
  Destructor.
*/
SoCoordinate4::~SoCoordinate4()
{
}

// Doc from superclass.
void
SoCoordinate4::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoCoordinate4, SO_FROM_INVENTOR_1);

  SO_ENABLE(SoGetBoundingBoxAction, SoCoordinateElement);
  SO_ENABLE(SoGLRenderAction, SoGLCoordinateElement);
  SO_ENABLE(SoPickAction, SoCoordinateElement);
  SO_ENABLE(SoCallbackAction, SoCoordinateElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoCoordinateElement);
}

// Doc from superclass.
void
SoCoordinate4::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoCoordinate4::doAction(action);
}

// Doc from superclass.
void
SoCoordinate4::doAction(SoAction * action)
{
  SoCoordinateElement::set4(action->getState(), this,
                            point.getNum(), point.getValues(0));
}

// Doc from superclass.
void
SoCoordinate4::GLRender(SoGLRenderAction * action)
{
  SoCoordinate4::doAction(action);
}

// Doc from superclass.
void
SoCoordinate4::callback(SoCallbackAction * action)
{
  SoCoordinate4::doAction(action);
}

// Doc from superclass.
void
SoCoordinate4::pick(SoPickAction * action)
{
  SoCoordinate4::doAction(action);
}

// Doc from superclass.
void
SoCoordinate4::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoCoordinate4::doAction(action);
}
