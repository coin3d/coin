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
  \class SoCoordinate3 SoCoordinate3.h Inventor/nodes/SoCoordinate3.h
  \brief The SoCoordinate3 class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOACTION)
#include <Inventor/actions/SoAction.h>
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOCOORDINATEELEMENT)
#include <Inventor/elements/SoCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOCOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOGLCOORDINATEELEMENT)
#include <Inventor/elements/SoGLCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOGLCOORDINATEELEMENT


/*!
  \var SoMFVec3f SoCoordinate3::point
  FIXME: write documentation for field
*/


// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoCoordinate3)
SoType SoCoordinate3::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoCoordinate3 node class.
*/
void *
SoCoordinate3::createInstance(void)
{
  return new SoCoordinate3;
}

/*!
  Returns the unique type identifier for the SoCoordinate3 class.
*/
SoType
SoCoordinate3::getClassTypeId(void)
{
  return SoCoordinate3::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoCoordinate3::getTypeId(void) const
{
  return SoCoordinate3::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoCoordinate3::SoCoordinate3()
{
//$ BEGIN TEMPLATE NodeConstructor(SoCoordinate3)
  // Make sure the class has been initialized.
  assert(SoCoordinate3::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

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
//$ BEGIN TEMPLATE InitNodeSource(Coordinate3)
  // Make sure we only initialize once.
  assert(SoCoordinate3::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoCoordinate3::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "Coordinate3",
                       &SoCoordinate3::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  SO_ENABLE(SoGetBoundingBoxAction, SoCoordinateElement);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoGLCoordinateElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoCoordinate3::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write function documentation
*/
void 
SoCoordinate3::doAction(SoAction * action)
{
  SoCoordinateElement::set3(action->getState(), this,
			    point.getNum(), point.getValues(0));
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoCoordinate3::GLRender(SoGLRenderAction * action)
{
  SoCoordinate3::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write function documentation
*/
void 
SoCoordinate3::callback(SoCallbackAction * action)
{
  SoCoordinate3::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write function documentation
*/
void 
SoCoordinate3::pick(SoPickAction * action)
{
  SoCoordinate3::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void 
SoCoordinate3::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoCoordinate3::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION


#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
*/
void
SoCoordinate3::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
