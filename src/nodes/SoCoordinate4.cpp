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
  \class SoCoordinate4 SoCoordinate4.h Inventor/nodes/SoCoordinate4.h
  \brief The SoCoordinate4 class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoCoordinate4.h>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoGLCoordinateElement.h>

/*!
  \var SoMFVec4f SoCoordinate4::point
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoCoordinate4);

/*!
  Constructor.
*/
SoCoordinate4::SoCoordinate4()
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

/*!
  Does initialization common for all objects of the
  SoCoordinate4 class. This includes setting up the
  type system, among other things.
*/
void
SoCoordinate4::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoCoordinate4);

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
SoCoordinate4::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoCoordinate4::doAction(action);
}

/*!
  FIXME: write doc
*/
void
SoCoordinate4::doAction(SoAction *action)
{
  SoCoordinateElement::set4(action->getState(), this,
                            point.getNum(), point.getValues(0));
}

/*!
  FIXME: write doc
*/
void
SoCoordinate4::GLRender(SoGLRenderAction *action)
{
  SoCoordinate4::doAction(action);
}

/*!
  FIXME: write doc
*/
void
SoCoordinate4::callback(SoCallbackAction *action)
{
  SoCoordinate4::doAction(action);
}

/*!
  FIXME: write doc
*/
void
SoCoordinate4::pick(SoPickAction *action)
{
  SoCoordinate4::doAction(action);
}

/*!
  FIXME: write doc
*/
void
SoCoordinate4::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoCoordinate4::doAction((SoAction*)action);
}
