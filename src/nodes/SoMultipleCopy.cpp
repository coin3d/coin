/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoMultipleCopy SoMultipleCopy.h Inventor/nodes/SoMultipleCopy.h
  \brief The SoMultipleCopy class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoMultipleCopy.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/misc/SoState.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/elements/SoSwitchElement.h>
#include <Inventor/elements/SoBBoxModelMatrixElement.h>
#include <Inventor/actions/SoCallbackAction.h>

/*!
  \var SoMFMatrix SoMultipleCopy::matrix
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoMultipleCopy);

/*!
  Constructor.
*/
SoMultipleCopy::SoMultipleCopy()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoMultipleCopy);

  SO_NODE_ADD_FIELD(matrix, (SbMatrix::identity()));
}

/*!
  Destructor.
*/
SoMultipleCopy::~SoMultipleCopy()
{
}

/*!
  Does initialization common for all objects of the
  SoMultipleCopy class. This includes setting up the
  type system, among other things.
*/
void
SoMultipleCopy::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoMultipleCopy);
}

/*!
  FIXME: write function documentation
*/
void
SoMultipleCopy::getBoundingBox(SoGetBoundingBoxAction * action)
{
  // store incoming modelmatrix
  SbMatrix mat = SoModelMatrixElement::get(action->getState());

  // get reference to the box
  SbXfBox3f & box = action->getXfBoundingBox();

  // store current bbox
  SbXfBox3f incomingbox = box;

  // accumulation variables
  SbVec3f acccenter(0.0f, 0.0f, 0.0f);
  int numCenters = 0;
  SbXfBox3f totalbox;

  for (int i=0; i < matrix.getNum(); i++) {
    action->getState()->push();

    SoSwitchElement::set(action->getState(), i);

    // make current box empty to calculate bbox of this separator
    box.makeEmpty();
    box.setTransform(SbMatrix::identity());

    // set bbox matrix
    SoBBoxModelMatrixElement::set(action->getState(), this, matrix[i]);

    // traverse all children, calculate the local bbox
    inherited::getBoundingBox(action);

    // If center point is set, accumulate center.
    if (action->isCenterSet()) {
      acccenter += action->getCenter();
      numCenters++;
      action->resetCenter();
    }

    // expand box by stored bbox
    if (!totalbox.isEmpty()) box.extendBy(totalbox);
    totalbox = box;

    action->getState()->pop();
  }

  // transform the local bbox by stored model matrix
  if (!box.isEmpty()) box.transform(mat);
  if (!incomingbox.isEmpty()) box.extendBy(incomingbox);

  if (numCenters != 0)
    action->setCenter(acccenter / float(numCenters), FALSE);
}

/*!
  FIXME: write function documentation
*/
void
SoMultipleCopy::GLRender(SoGLRenderAction * action)
{
  SoMultipleCopy::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
 */
SbBool
SoMultipleCopy::affectsState(void) const
{
  // state is pushed/popped for each traversal
  return FALSE;
}

/*!
  FIXME: write doc
 */
void
SoMultipleCopy::doAction(SoAction *action)
{
  for (int i=0; i < matrix.getNum(); i++) {
    action->getState()->push();
    SoSwitchElement::set(action->getState(), i);
    SoModelMatrixElement::mult(action->getState(), this, matrix[i]);
    inherited::doAction(action);
    action->getState()->pop();
  }
}

/*!
  FIXME: write doc
 */
void
SoMultipleCopy::callback(SoCallbackAction *action)
{
  action->invokePreCallbacks(this);
  if (action->getCurrentResponse() == SoCallbackAction::CONTINUE) {
    SoMultipleCopy::doAction((SoAction*)action);
    action->invokePostCallbacks(this);
  }
}

/*!
  We came across what we think is a bug in TGS/SGI OIV when
  implementing picking for this node. The SoPickedPoint class can
  return the object space point, normal and texture
  coordinates. TGS/SGI OIV do not consider the transformation inside
  this node before returning the object space data from SoPickedPoint,
  since the path in SoPickedPoint does not say anything about on which
  copy the pick occured. We solved this simply by storing both world
  space and object space data in SoPickedPoint.
*/
void
SoMultipleCopy::pick(SoPickAction *action)
{
  SoMultipleCopy::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
 */
void
SoMultipleCopy::handleEvent(SoHandleEventAction *action)
{
  inherited::handleEvent(action);
}

/*!
  FIXME: write doc
 */
void
SoMultipleCopy::getMatrix(SoGetMatrixAction *action)
{
  // path does not specify which copy to traverse
  inherited::getMatrix(action);
}

/*!
  FIXME: write doc
 */
void
SoMultipleCopy::search(SoSearchAction *action)
{
  SoNode::search(action);
  inherited::search(action);
}

/*!
  FIXME: write doc
*/
void
SoMultipleCopy::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoMultipleCopy::doAction((SoAction*)action);
}
