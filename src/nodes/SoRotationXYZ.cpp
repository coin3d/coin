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
  \class SoRotationXYZ SoRotationXYZ.h Inventor/nodes/SoRotationXYZ.h
  \brief The SoRotationXYZ class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoRotationXYZ.h>


#include <Inventor/SbRotation.h>
#include <Inventor/SbMatrix.h>

#if !defined(COIN_EXCLUDE_SOACTION)
#include <Inventor/actions/SoAction.h>
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
#include <Inventor/actions/SoGetMatrixAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOMODELMATRIXELEMENT)
#include <Inventor/elements/SoModelMatrixElement.h>
#endif // !COIN_EXCLUDE_SOMODELMATRIXELEMENT

/*!
  \enum SoRotationXYZ::Axis
  FIXME: write documentation for enum
*/
/*!
  \var SoRotationXYZ::Axis SoRotationXYZ::X
  FIXME: write documentation for enum definition
*/
/*!
  \var SoRotationXYZ::Axis SoRotationXYZ::Y
  FIXME: write documentation for enum definition
*/
/*!
  \var SoRotationXYZ::Axis SoRotationXYZ::Z
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoRotationXYZ::axis
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoRotationXYZ::angle
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoRotationXYZ);

/*!
  Constructor.
*/
SoRotationXYZ::SoRotationXYZ()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoRotationXYZ);

  SO_NODE_ADD_FIELD(angle, (0.0f));
  SO_NODE_ADD_FIELD(axis, (SoRotationXYZ::X));

  SO_NODE_DEFINE_ENUM_VALUE(Axis, X);
  SO_NODE_DEFINE_ENUM_VALUE(Axis, Y);
  SO_NODE_DEFINE_ENUM_VALUE(Axis, Z);
  SO_NODE_SET_SF_ENUM_TYPE(axis, Axis);
}

/*!
  Destructor.
*/
SoRotationXYZ::~SoRotationXYZ()
{
}

/*!
  Does initialization common for all objects of the
  SoRotationXYZ class. This includes setting up the
  type system, among other things.
*/
void
SoRotationXYZ::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoRotationXYZ);
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write function documentation
*/
void 
SoRotationXYZ::doAction(SoAction * action)
{
  SbVec3f rotvec;
  if (this->getVector(rotvec)) {
    SoModelMatrixElement::rotateBy(action->getState(), this,
				   SbRotation(rotvec, angle.getValue()));
  }
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write function documentation
*/
void 
SoRotationXYZ::callback(SoCallbackAction * action)
{
  SoRotationXYZ::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoRotationXYZ::GLRender(SoGLRenderAction * action)
{
  SoRotationXYZ::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void 
SoRotationXYZ::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoRotationXYZ::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
/*!
  FIXME: write function documentation
*/
void 
SoRotationXYZ::getMatrix(SoGetMatrixAction * action)
{
  SbVec3f rotvec;
  if (getVector(rotvec)) {
    action->rotateBy(SbRotation(rotvec, angle.getValue()));
  }
}
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write function documentation
*/
void 
SoRotationXYZ::pick(SoPickAction * action)
{
  SoRotationXYZ::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION

/*!
  FIXME: write function documentation
*/
SbBool 
SoRotationXYZ::getVector(SbVec3f & rotvec)
{  
  assert((int)axis.getValue() >= 0 && (int)axis.getValue() <= 2);
  
  rotvec.setValue(0.0f, 0.0f, 0.0f);
  rotvec[(int)axis.getValue()] = 1.0f;
  return TRUE;
}


/*!
  FIXME: write doc
 */
SbRotation
SoRotationXYZ::getRotation(void) const
{
  assert(0 && "FIXME: not implemented");
  return SbRotation(SbVec3f(1,0,0), 0);
}

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoRotationXYZ::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
