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
  \class SoProfileCoordinate2 SoProfileCoordinate2.h Inventor/nodes/SoProfileCoordinate2.h
  \brief The SoProfileCoordinate2 class is a node specifying a set of 2D coordinates for profiles.
  \ingroup nodes

  Use nodes of this type to provide coordinates to profiles.

  Note that an SoProfileCoordinate2 node will \e replace the profile
  coordinates already present in the state (if any).

  \sa SoProfile, SoProfileCoordinate3
*/

#include <Inventor/nodes/SoProfileCoordinate2.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoProfileCoordinateElement.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>

/*!
  \var SoMFVec2f SoProfileCoordinate2::point

  Pool of coordinate points for the traversal state.
*/

// *************************************************************************

SO_NODE_SOURCE(SoProfileCoordinate2);

/*!
  Constructor.
*/
SoProfileCoordinate2::SoProfileCoordinate2(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoProfileCoordinate2);

  SO_NODE_ADD_FIELD(point, (0.0f, 0.0f));
}

/*!
  Destructor.
*/
SoProfileCoordinate2::~SoProfileCoordinate2()
{
}

// doc in super
void
SoProfileCoordinate2::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoProfileCoordinate2, SO_FROM_INVENTOR_1);

  SO_ENABLE(SoCallbackAction, SoProfileCoordinateElement);
  SO_ENABLE(SoGLRenderAction, SoProfileCoordinateElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoProfileCoordinateElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoProfileCoordinateElement);
  SO_ENABLE(SoPickAction, SoProfileCoordinateElement);
}

void
SoProfileCoordinate2::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoProfileCoordinate2::doAction(action);
}

void
SoProfileCoordinate2::doAction(SoAction * action)
{
  SoProfileCoordinateElement::set2(action->getState(), this,
                                   this->point.getNum(),
                                   this->point.getValues(0));
}

void
SoProfileCoordinate2::GLRender(SoGLRenderAction * action)
{
  SoProfileCoordinate2::doAction(action);
}

void
SoProfileCoordinate2::callback(SoCallbackAction * action)
{
  SoProfileCoordinate2::doAction(action);
}

void
SoProfileCoordinate2::pick(SoPickAction * action)
{
  SoProfileCoordinate2::doAction(action);
}

void
SoProfileCoordinate2::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoProfileCoordinate2::doAction(action);
}
