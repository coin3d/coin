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
  \class SoProfile SoProfile.h Inventor/nodes/SoProfile.h
  \brief The SoProfile class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoProfile.h>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoProfileElement.h>
#include <Inventor/elements/SoProfileCoordinateElement.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>

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

  SO_ENABLE(SoGetBoundingBoxAction, SoProfileElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoProfileCoordinateElement);

  SO_ENABLE(SoGLRenderAction, SoProfileElement);
  SO_ENABLE(SoGLRenderAction, SoProfileCoordinateElement);

  SO_ENABLE(SoPickAction, SoProfileElement);
  SO_ENABLE(SoPickAction, SoProfileCoordinateElement);

  SO_ENABLE(SoCallbackAction, SoProfileElement);
  SO_ENABLE(SoCallbackAction, SoProfileCoordinateElement);

  SO_ENABLE(SoGetPrimitiveCountAction, SoProfileElement);
  SO_ENABLE(SoCallbackAction, SoProfileCoordinateElement);
}

/*!
  FIXME: write function documentation
*/
void
SoProfile::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoProfile::doAction(action);
}

/*!
  FIXME: write doc
 */
void
SoProfile::doAction(SoAction *action)
{
  SoProfileElement::add(action->getState(), this);
}

/*!
  FIXME: write doc
 */
void
SoProfile::callback(SoCallbackAction *action)
{
  SoProfile::doAction(action);
}

/*!
  FIXME: write doc
 */
void
SoProfile::GLRender(SoGLRenderAction * action)
{
  SoProfile::doAction(action);
}

/*!
  FIXME: write doc
 */
void
SoProfile::pick(SoPickAction *action)
{
  SoProfile::doAction(action);
}

/*!
  FIXME: write doc
 */
void
SoProfile::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoProfile::doAction(action);
}

