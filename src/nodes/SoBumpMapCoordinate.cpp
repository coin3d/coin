/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoBumpMapCoordinate SoBumpMapCoordinate.h Inventor/nodes/SoBumpMapCoordinate.h
  \brief The SoBumpMapCoordinate class is a node for providing bump map coordinates to shape nodes.
  \ingroup nodes

  When encountering nodes of this type during traversal, the
  coordinates it contains will be put on the state stack for later use
  by shape nodes. The bump map coordinates can be used to specify
  explicit coordinates for a bump map. The SoBumpMap node is used to
  specify a bump map for the shape nodes.

  Note that an SoBumpMapCoordinate node will \e replace the bump map
  coordinates already present in the state (if any).

  \since Coin 2.2
*/

#include <Inventor/nodes/SoBumpMapCoordinate.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoBumpMapCoordinateElement.h>


/*!
  \var SoMFVec3f SoBumpMapCoordinate::point
  Set of 2D points. Contains no points by default.
*/


// *************************************************************************

SO_NODE_SOURCE(SoBumpMapCoordinate);

/*!
  Constructor.
*/
SoBumpMapCoordinate::SoBumpMapCoordinate(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoBumpMapCoordinate);

  SO_NODE_ADD_FIELD(point, (0.0f, 0.0f));
  this->point.setNum(0);
  this->point.setDefault(TRUE);
}

/*!
  Destructor.
*/
SoBumpMapCoordinate::~SoBumpMapCoordinate()
{
}

// Doc from superclass.
void
SoBumpMapCoordinate::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoBumpMapCoordinate, SO_FROM_COIN_2_2);

  SO_ENABLE(SoGLRenderAction, SoBumpMapCoordinateElement);
}

// Doc from superclass.
void
SoBumpMapCoordinate::doAction(SoAction * action)
{
  // only SoGLRenderAction has activated the element for now
}

// Doc from superclass.
void
SoBumpMapCoordinate::GLRender(SoGLRenderAction * action)
{
  SoBumpMapCoordinateElement::set(action->getState(), this,
                                  point.getNum(), point.getValues(0));
}

// Doc from superclass.
void
SoBumpMapCoordinate::callback(SoCallbackAction * action)
{
  SoBumpMapCoordinate::doAction((SoAction*)action);
}

// Doc from superclass.
void
SoBumpMapCoordinate::pick(SoPickAction * action)
{
  SoBumpMapCoordinate::doAction((SoAction*) action);
}


