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
  \class SoProfileCoordinate3 SoProfileCoordinate3.h Inventor/nodes/SoProfileCoordinate3.h
  \brief The SoProfileCoordinate3 class is a node specifying a set of 3D coordinates for profiles.
  \ingroup nodes

  Use nodes of this type to provide coordinates to profiles.

  The third element of the coordinate vectors is used for
  normalization. A node of this type where all the normalization
  values are equal to 1.0 is the equivalent of setting up an
  SoProfileCoordinate2 node.

  Note that an SoProfileCoordinate3 node will \e replace the profile
  coordinates already present in the state (if any).

  \sa SoProfile, SoProfileCoordinate2
*/

#include <Inventor/nodes/SoProfileCoordinate3.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoProfileCoordinateElement.h>

/*!
  \var SoMFVec3f SoProfileCoordinate3::point

  Pool of coordinate points for the traversal state.
*/

// *************************************************************************

SO_NODE_SOURCE(SoProfileCoordinate3);

/*!
  Constructor.
*/
SoProfileCoordinate3::SoProfileCoordinate3(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoProfileCoordinate3);

  SO_NODE_ADD_FIELD(point, (0.0f, 0.0f, 1.0f));
}

/*!
  Destructor.
*/
SoProfileCoordinate3::~SoProfileCoordinate3()
{
}

void
SoProfileCoordinate3::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoProfileCoordinate3);

  SO_ENABLE(SoGLRenderAction, SoProfileCoordinateElement);
  SO_ENABLE(SoPickAction, SoProfileCoordinateElement);
  SO_ENABLE(SoCallbackAction, SoProfileCoordinateElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoProfileCoordinateElement);
}

void
SoProfileCoordinate3::doAction(SoAction * action)
{
  SoProfileCoordinateElement::set3(action->getState(), this,
                                   this->point.getNum(),
                                   this->point.getValues(0));
}

void
SoProfileCoordinate3::GLRender(SoGLRenderAction * action)
{
  SoProfileCoordinate3::doAction(action);
}

void
SoProfileCoordinate3::callback(SoCallbackAction * action)
{
  SoProfileCoordinate3::doAction(action);
}

void
SoProfileCoordinate3::pick(SoPickAction * action)
{
  SoProfileCoordinate3::doAction(action);
}

void
SoProfileCoordinate3::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoProfileCoordinate3::doAction(action);
}
