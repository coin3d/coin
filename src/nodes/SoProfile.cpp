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
  \class SoProfile SoProfile.h Inventor/nodes/SoProfile.h
  \brief The SoProfile class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoProfile.h>
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

#if !defined(COIN_EXCLUDE_SOPROFILEELEMENT)
#include <Inventor/elements/SoProfileElement.h>
#endif // !COIN_EXCLUDE_SOPROFILEELEMENT
#if !defined(COIN_EXCLUDE_SOPROFILECOORDINATEELEMENT)
#include <Inventor/elements/SoProfileCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOPROFILECOORDINATEELEMENT

/*!
  \enum SoProfile::Profile
  FIXME: write documentation for enum
*/
/*!
  \var SoProfile::Profile SoProfile::START_FIRST
  FIXME: write documentation for enum definition
*/
/*!
  \var SoProfile::Profile SoProfile::START_NEW
  FIXME: write documentation for enum definition
*/
/*!
  \var SoProfile::Profile SoProfile::ADD_TO_CURRENT
  FIXME: write documentation for enum definition
*/


/*!
  \var SoMFInt32 SoProfile::index
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoProfile::linkage
  FIXME: write documentation for field
*/


/*!
  \fn void SoProfile::getTrimCurve(SoState * state, int32_t & numPoints, float *& points, int & floatsPerVec, int32_t & numKnots, float *& knotVector)
  FIXME: write doc
*/

/*!
  \fn void SoProfile::getVertices(SoState * state, int32_t & nVertices, SbVec2f *& vertices)
  FIXME: write doc
*/


// *************************************************************************

//$ BEGIN TEMPLATE NodeAbstractSource(SoProfile)
SoType SoProfile::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the SoProfile class.
*/
SoType
SoProfile::getClassTypeId(void)
{
  return SoProfile::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoProfile::getTypeId(void) const
{
  return SoProfile::classTypeId;
}
//$ END TEMPLATE NodeAbstractSource

/*!
  Constructor.
*/
SoProfile::SoProfile()
{
//$ BEGIN TEMPLATE NodeConstructor(SoProfile)
  // Make sure the class has been initialized.
  assert(SoProfile::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(index, (0));
  SO_NODE_ADD_FIELD(linkage, (START_FIRST));
}

/*!
  Destructor.
*/
SoProfile::~SoProfile()
{
}

/*!
  Does initialization common for all objects of the
  SoProfile class. This includes setting up the
  type system, among other things.
*/
void
SoProfile::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeAbstractSource(Profile)
  // Make sure we only initialize once.
  assert(SoProfile::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoProfile::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "Profile",
                       NULL,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeAbstractSource

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  SO_ENABLE(SoGetBoundingBoxAction, SoProfileElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoProfileCoordinateElement);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoProfileElement);
  SO_ENABLE(SoGLRenderAction, SoProfileCoordinateElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoProfile::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void 
SoProfile::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoProfileElement::add(action->getState(), this);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoProfile::doAction(SoAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoProfile::callback(SoCallbackAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
 */
void
SoProfile::GLRender(SoGLRenderAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoProfile::pick(SoPickAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoProfile::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
