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
  \class SoCoordinate3 SoCoordinate3.h Inventor/nodes/SoCoordinate3.h
  \brief The SoCoordinate3 class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoCoordinate3.h>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/elements/SoGLCoordinateElement.h>


/*!
  \var SoMFVec3f SoCoordinate3::point
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoCoordinate3);

/*!
  Constructor.
*/
SoCoordinate3::SoCoordinate3()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoCoordinate3);

  SO_NODE_ADD_FIELD(point, (0.0f, 0.0f, 0.0f));
}

/*!
  Destructor.
*/
SoCoordinate3::~SoCoordinate3()
{
}

/*!
  Does initialization common for all objects of the
  SoCoordinate3 class. This includes setting up the
  type system, among other things.
*/
void
SoCoordinate3::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoCoordinate3);

  SO_ENABLE(SoGetBoundingBoxAction, SoCoordinateElement);
  SO_ENABLE(SoGLRenderAction, SoGLCoordinateElement);
  SO_ENABLE(SoPickAction, SoCoordinateElement);
  SO_ENABLE(SoCallbackAction, SoCoordinateElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoCoordinateElement);
}

/*!
  FIXME: write function documentation
*/
void
SoCoordinate3::doAction(SoAction * action)
{
  SoCoordinateElement::set3(action->getState(), this,
                            point.getNum(), point.getValues(0));
}

/*!
  FIXME: write function documentation
*/
void
SoCoordinate3::GLRender(SoGLRenderAction * action)
{
  SoCoordinate3::doAction((SoAction *)action);
}

/*!
  FIXME: write function documentation
*/
void
SoCoordinate3::callback(SoCallbackAction * action)
{
  SoCoordinate3::doAction((SoAction *)action);
}

/*!
  FIXME: write function documentation
*/
void
SoCoordinate3::pick(SoPickAction * action)
{
  SoCoordinate3::doAction((SoAction *)action);
}

/*!
  FIXME: write function documentation
*/
void
SoCoordinate3::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoCoordinate3::doAction((SoAction *)action);
}

/*!
  FIXME: write doc
*/
void
SoCoordinate3::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoCoordinate3::doAction((SoAction*)action);
}
