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
  \class SoCoordinate4 SoCoordinate4.h Inventor/nodes/SoCoordinate4.h
  \brief The SoCoordinate4 class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoCoordinate4.h>



#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
#include <Inventor/actions/SoPickAction.h>
#endif // !COIN_EXCLUDE_SOPICKACTION

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>

#if !defined(COIN_EXCLUDE_SOCOORDINATEELEMENT)
#include <Inventor/elements/SoCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOCOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOGLCOORDINATEELEMENT)
#include <Inventor/elements/SoGLCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOGLCOORDINATEELEMENT

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

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  SO_ENABLE(SoGetBoundingBoxAction, SoCoordinateElement);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoGLCoordinateElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
  SO_ENABLE(SoPickAction, SoCoordinateElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

  SO_ENABLE(SoCallbackAction, SoCoordinateElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoCoordinateElement);
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoCoordinate4::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoCoordinate4::doAction(action);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
*/
void
SoCoordinate4::doAction(SoAction *action)
{
  SoCoordinateElement::set4(action->getState(), this,
                            point.getNum(), point.getValues(0));
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
*/
void
SoCoordinate4::GLRender(SoGLRenderAction *action)
{
  SoCoordinate4::doAction(action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
*/
void
SoCoordinate4::callback(SoCallbackAction *action)
{
  SoCoordinate4::doAction(action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
*/
void
SoCoordinate4::pick(SoPickAction *action)
{
  SoCoordinate4::doAction(action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
*/
void
SoCoordinate4::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoCoordinate4::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
