/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoTransformSeparator SoTransformSeparator.h Inventor/nodes/SoTransformSeparator.h
  \brief The SoTransformSeparator class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoTransformSeparator.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <coindefs.h> // COIN_STUB()
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoPickAction.h>
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
