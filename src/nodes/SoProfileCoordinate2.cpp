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
  \class SoProfileCoordinate2 SoProfileCoordinate2.h Inventor/nodes/SoProfileCoordinate2.h
  \brief The SoProfileCoordinate2 class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoProfileCoordinate2.h>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoProfileCoordinateElement.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>

/*!
  \var SoMFVec2f SoProfileCoordinate2::point
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoProfileCoordinate2);

/*!
  Constructor.
*/
SoProfileCoordinate2::SoProfileCoordinate2()
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

/*!
  Does initialization common for all objects of the
  SoProfileCoordinate2 class. This includes setting up the
  type system, among other things.
*/
void
SoProfileCoordinate2::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoProfileCoordinate2);

  SO_ENABLE(SoGetBoundingBoxAction, SoProfileCoordinateElement);
  SO_ENABLE(SoGLRenderAction, SoProfileCoordinateElement);
  SO_ENABLE(SoPickAction, SoProfileCoordinateElement);
  SO_ENABLE(SoCallbackAction, SoProfileCoordinateElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoProfileCoordinateElement);
}

/*!
  FIXME: write function documentation
*/
void
SoProfileCoordinate2::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoProfileCoordinate2::doAction(action);
}

/*!
  FIXME: write doc
 */
void
SoProfileCoordinate2::doAction(SoAction *action )
{
  SoProfileCoordinateElement::set2(action->getState(), this,
                                   point.getNum(), point.getValues(0));
}

/*!
  FIXME: write doc
 */
void
SoProfileCoordinate2::GLRender(SoGLRenderAction *action)
{
  SoProfileCoordinate2::doAction(action);
}

/*!
  FIXME: write doc
 */
void
SoProfileCoordinate2::callback(SoCallbackAction *action)
{
  SoProfileCoordinate2::doAction(action);
}

/*!
  FIXME: write doc
 */
void
SoProfileCoordinate2::pick(SoPickAction *action)
{
  SoProfileCoordinate2::doAction(action);
}

/*!
  FIXME: write doc
 */
void
SoProfileCoordinate2::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoProfileCoordinate2::doAction(action);
}
