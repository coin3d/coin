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
  \class SoTransformSeparator SoTransformSeparator.h Inventor/nodes/SoTransformSeparator.h
  \brief The SoTransformSeparator class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoTransformSeparator.h>


#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
#include <Inventor/actions/SoGetMatrixAction.h>
#endif // ! COIN_EXCLUDE_SOGETMATRIXACTION
#if !defined(COIN_EXCLUDE_SOMODELMATRIXELEMENT)
#include <Inventor/elements/SoModelMatrixElement.h>
#endif
#if !defined(COIN_EXCLUDE_SOMODELMATRIXELEMENT)
#include <Inventor/elements/SoBBoxModelMatrixElement.h>
#endif

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif

// *************************************************************************

SO_NODE_SOURCE(SoTransformSeparator);

/*!
  Constructor.
*/
SoTransformSeparator::SoTransformSeparator()
{
  SO_NODE_CONSTRUCTOR(SoTransformSeparator);
}

/*!
  Destructor.
*/
SoTransformSeparator::~SoTransformSeparator()
{
}

/*!
  Does initialization common for all objects of the
  SoTransformSeparator class. This includes setting up the
  type system, among other things.
*/
void
SoTransformSeparator::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTransformSeparator);
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void 
SoTransformSeparator::getBoundingBox(SoGetBoundingBoxAction * action)
{
#if !defined(COIN_EXCLUDE_SOBBOXMODELMATRIXELMENT)
  SbMatrix matrix, localMatrix;
  SoBBoxModelMatrixElement::pushMatrix(action->getState(),
				       matrix,
				       localMatrix);
#endif
  inherited::getBoundingBox(action);
#if !defined(COIN_EXCLUDE_SOBBOXMODELMATRIXELMENT)
  SoBBoxModelMatrixElement::popMatrix(action->getState(), 
				      matrix,
				      localMatrix);
#endif
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

/*!
  FIXME: write doc
 */
SoTransformSeparator::SoTransformSeparator(int /* nChildren */)
{
  assert(0 && "FIXME: not implemented");
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoTransformSeparator::doAction(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoTransformSeparator::callback(SoCallbackAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
 */
void
SoTransformSeparator::GLRender(SoGLRenderAction * action)
{
#if !defined(COIN_EXCLUDE_SOMODELMATRIXELEMENT)
  SbMatrix matrix = SoModelMatrixElement::pushMatrix(action->getState());
#endif
  inherited::GLRender(action);
#if !defined(COIN_EXCLUDE_SOMODELMATRIXELEMENT)
  SoModelMatrixElement::popMatrix(action->getState(), matrix);
#endif
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoTransformSeparator::pick(SoPickAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
/*!
  FIXME: write doc
 */
void
SoTransformSeparator::getMatrix(SoGetMatrixAction *action)
{
  if (action->getCurPathCode() == SoAction::OFF_PATH) {
    SbMatrix matrix, invmatrix;
    matrix = action->getMatrix();
    invmatrix = action->getInverse();
    inherited::getMatrix(action);
#if _WIN32 // fix for stupid m$ compiler
    action->getMatrix() = matrix;
	action->getInverse() = invmatrix;
#else // normal compilers use this code 
    action->getMatrix().setValue(matrix);
    action->getInverse().setValue(invmatrix);
#endif // fix for vc6 compiler
  }
  else {
    assert(action->getCurPathCode() == SoAction::IN_PATH);
    inherited::getMatrix(action);
  }
}
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoTransformSeparator::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
