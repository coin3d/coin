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

void
SoProfileCoordinate2::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoProfileCoordinate2);

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
