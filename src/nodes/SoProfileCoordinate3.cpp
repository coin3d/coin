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
  \class SoProfileCoordinate3 SoProfileCoordinate3.h Inventor/nodes/SoProfileCoordinate3.h
  \brief The SoProfileCoordinate3 class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoProfileCoordinate3.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
#include <Inventor/actions/SoPickAction.h>
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOPROFILECOORDINATEELEMENT)
#include <Inventor/elements/SoProfileCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOPROFILECOORDINATEELEMENT

/*!
  \var SoMFVec3f SoProfileCoordinate3::point
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoProfileCoordinate3)
SoType SoProfileCoordinate3::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoProfileCoordinate3 node class.
*/
void *
SoProfileCoordinate3::createInstance(void)
{
  return new SoProfileCoordinate3;
}

/*!
  Returns the unique type identifier for the SoProfileCoordinate3 class.
*/
SoType
SoProfileCoordinate3::getClassTypeId(void)
{
  return SoProfileCoordinate3::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoProfileCoordinate3::getTypeId(void) const
{
  return SoProfileCoordinate3::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoProfileCoordinate3::SoProfileCoordinate3()
{
//$ BEGIN TEMPLATE NodeConstructor(SoProfileCoordinate3)
  // Make sure the class has been initialized.
  assert(SoProfileCoordinate3::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor
  
  SO_NODE_ADD_FIELD(point, (0.0f, 0.0f, 1.0f));
}

/*!
  Destructor.
*/
SoProfileCoordinate3::~SoProfileCoordinate3()
{
}

/*!
  Does initialization common for all objects of the
  SoProfileCoordinate3 class. This includes setting up the
  type system, among other things.
*/
void
SoProfileCoordinate3::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(ProfileCoordinate3)
  // Make sure we only initialize once.
  assert(SoProfileCoordinate3::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoProfileCoordinate3::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "ProfileCoordinate3",
                       &SoProfileCoordinate3::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoProfileCoordinateElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
  SO_ENABLE(SoPickAction, SoProfileCoordinateElement);
#endif // !COIN_EXCLUDE_SOPICKACTION

}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoProfileCoordinate3::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoProfileCoordinate3::doAction(SoAction *action)
{
  SoProfileCoordinateElement::set3(action->getState(), this,
				   point.getNum(), point.getValues(0));
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
 */
void
SoProfileCoordinate3::GLRender(SoGLRenderAction *action)
{
  SoProfileCoordinate3::doAction(action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoProfileCoordinate3::callback(SoCallbackAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoProfileCoordinate3::pick(SoPickAction *action)
{
  SoProfileCoordinate3::doAction(action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoProfileCoordinate3::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
