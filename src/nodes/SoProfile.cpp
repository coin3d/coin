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



#if !defined(COIN_EXCLUDE_SOACTION)
#include <Inventor/actions/SoAction.h>
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
#include <Inventor/actions/SoPickAction.h>
#endif // !COIN_EXCLUDE_SOPICKACTION

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

SO_NODE_ABSTRACT_SOURCE(SoProfile);

/*!
  Constructor.
*/
SoProfile::SoProfile()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoProfile);

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
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoProfile);

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  SO_ENABLE(SoGetBoundingBoxAction, SoProfileElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoProfileCoordinateElement);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoProfileElement);
  SO_ENABLE(SoGLRenderAction, SoProfileCoordinateElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
  SO_ENABLE(SoPickAction, SoProfileElement);
  SO_ENABLE(SoPickAction, SoProfileCoordinateElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void 
SoProfile::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoProfile::doAction(action);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoProfile::doAction(SoAction *action)
{
  SoProfileElement::add(action->getState(), this);
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoProfile::callback(SoCallbackAction * /* action */)
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
  SoProfile::doAction(action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoProfile::pick(SoPickAction *action)
{
  SoProfile::doAction(action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoProfile::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
