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
  \class SoResetTransform SoResetTransform.h Inventor/nodes/SoResetTransform.h
  \brief The SoResetTransform class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoResetTransform.h>



#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#if !defined(COIN_EXCLUDE_SOGLNORMALIZEELEMENT)
#include <Inventor/elements/SoGLNormalizeElement.h>
#endif // ! COIN_EXCLUDE_SOGLNORMALIZEELEMENT
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
#include <Inventor/actions/SoGetMatrixAction.h>
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION

#if !defined(COIN_EXCLUDE_SOMODELMATRIXELEMENT)
#include <Inventor/elements/SoModelMatrixElement.h>
#endif // !COIN_EXCLUDE_SOMODELMATRIXELEMENT

/*!
  \enum SoResetTransform::ResetType
  FIXME: write documentation for enum
*/
/*!
  \var SoResetTransform::ResetType SoResetTransform::TRANSFORM
  FIXME: write documentation for enum definition
*/
/*!
  \var SoResetTransform::ResetType SoResetTransform::BBOX
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFBitMask SoResetTransform::whatToReset
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoResetTransform);

/*!
  Constructor.
*/
SoResetTransform::SoResetTransform()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoResetTransform);

  SO_NODE_ADD_FIELD(whatToReset, (SoResetTransform::TRANSFORM));

  SO_NODE_DEFINE_ENUM_VALUE(ResetType, TRANSFORM);
  SO_NODE_DEFINE_ENUM_VALUE(ResetType, BBOX);
  SO_NODE_SET_SF_ENUM_TYPE(whatToReset, ResetType);
}

/*!
  Destructor.
*/
SoResetTransform::~SoResetTransform()
{
}

/*!
  Does initialization common for all objects of the
  SoResetTransform class. This includes setting up the
  type system, among other things.
*/
void
SoResetTransform::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoResetTransform);
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoResetTransform::GLRender(SoGLRenderAction * action)
{
  if (this->whatToReset.getValue() & SoResetTransform::TRANSFORM) {
    SoModelMatrixElement::set(action->getState(), this,
			      SbMatrix::identity());
#if !defined(COIN_EXCLUDE_SOGLNORMALIZEELEMENT)
    SoGLNormalizeElement::setMatrixState(action->getState(), TRUE);
#endif // ! COIN_EXCLUDE_SOGLNORMALIZEELEMENT  

  }
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void 
SoResetTransform::getBoundingBox(SoGetBoundingBoxAction * action)
{
  if (this->whatToReset.getValue() & SoResetTransform::BBOX)
    action->getXfBoundingBox().makeEmpty();

  if (this->whatToReset.getValue() & SoResetTransform::TRANSFORM)
    SoModelMatrixElement::set(action->getState(), this,
			      SbMatrix::identity());
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoResetTransform::doAction(SoAction *action)
{
  if (this->whatToReset.getValue() & SoResetTransform::TRANSFORM)
    SoModelMatrixElement::set(action->getState(), this,
			      SbMatrix::identity());
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoResetTransform::callback(SoCallbackAction *action)
{
  SoResetTransform::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
/*!
  FIXME: write doc
*/
void
SoResetTransform::getMatrix(SoGetMatrixAction *action)
{  
  if (this->whatToReset.getValue() & SoResetTransform::TRANSFORM) {
    action->getMatrix().makeIdentity();
    action->getInverse().makeIdentity();
  }
}
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoResetTransform::pick(SoPickAction *action)
{
  SoResetTransform::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoResetTransform::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoResetTransform::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
