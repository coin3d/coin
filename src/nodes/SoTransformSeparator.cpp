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
  \class SoTransformSeparator SoTransformSeparator.h Inventor/nodes/SoTransformSeparator.h
  \brief The SoTransformSeparator class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoTransformSeparator.h>

#include <coindefs.h> // COIN_STUB()
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoBBoxModelMatrixElement.h>


SO_NODE_SOURCE(SoTransformSeparator);

/*!
  Constructor.
*/
SoTransformSeparator::SoTransformSeparator()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTransformSeparator);
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

/*!
  FIXME: write function documentation
*/
void
SoTransformSeparator::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SbMatrix matrix, localMatrix;
  SoBBoxModelMatrixElement::pushMatrix(action->getState(),
                                       matrix,
                                       localMatrix);
  inherited::getBoundingBox(action);
  SoBBoxModelMatrixElement::popMatrix(action->getState(),
                                      matrix,
                                      localMatrix);
}

/*!
  FIXME: write doc
 */
SoTransformSeparator::SoTransformSeparator(int /* nChildren */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoTransformSeparator::doAction(SoAction *action)
{
  SbMatrix matrix = SoModelMatrixElement::pushMatrix(action->getState());
  inherited::doAction(action);
  SoModelMatrixElement::popMatrix(action->getState(), matrix);
}

/*!
  FIXME: write doc
 */
void
SoTransformSeparator::callback(SoCallbackAction *action)
{
  SbMatrix matrix = SoModelMatrixElement::pushMatrix(action->getState());
  inherited::callback(action);
  SoModelMatrixElement::popMatrix(action->getState(), matrix);
}

/*!
  FIXME: write doc
 */
void
SoTransformSeparator::GLRender(SoGLRenderAction * action)
{
  SbMatrix matrix = SoModelMatrixElement::pushMatrix(action->getState());
  inherited::GLRender(action);
  SoModelMatrixElement::popMatrix(action->getState(), matrix);
}

/*!
  FIXME: write doc
 */
void
SoTransformSeparator::pick(SoPickAction *action)
{
  SbMatrix matrix = SoModelMatrixElement::pushMatrix(action->getState());
  inherited::pick(action);
  SoModelMatrixElement::popMatrix(action->getState(), matrix);
}

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
    // Note: don't use ..->getMatrix().setValue(...) here, as that
    // won't work (for some weird reason) with certain compilers (like
    // MSVC++ 6.0 and AIX xlc).
    action->getMatrix() = matrix;
    action->getInverse() = invmatrix;
  }
  else {
    assert(action->getCurPathCode() == SoAction::IN_PATH);
    inherited::getMatrix(action);
  }
}

/*!
  FIXME: write doc
 */
void
SoTransformSeparator::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoTransformSeparator::doAction((SoAction*)action);
}
