/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoMultipleCopy SoMultipleCopy.h Inventor/nodes/SoMultipleCopy.h
  \brief The SoMultipleCopy class redraws it's children multiple times at different transformations.
  \ingroup nodes

  The SoMultipleCopy group node duplicates it's children nodes /
  subgraphs without using additional memory resources.

  It can do general transformations (translations, rotation and
  scaling) for it's children. Apart from transformations, the
  appearance of it's children will be identical.

  \sa SoArray
*/


#include <Inventor/nodes/SoMultipleCopy.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoSwitchElement.h>
#include <Inventor/misc/SoState.h>

/*!
  \var SoMFMatrix SoMultipleCopy::matrix

  A set of geometry transformation matrices.

  The number of duplicated redraws of the child geometry will be the
  same as the number of matrices specified in this field. Ie, each
  duplication will be transformed according to a transformation
  matrix.

  The default value of the field is to contain just a single identity
  matrix.
*/

// *************************************************************************

SO_NODE_SOURCE(SoMultipleCopy);

/*!
  Constructor.
*/
SoMultipleCopy::SoMultipleCopy(void)
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

// Doc in superclass.
void
SoMultipleCopy::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoMultipleCopy, SO_FROM_INVENTOR_1);
}

// Doc in superclass.
void
SoMultipleCopy::getBoundingBox(SoGetBoundingBoxAction * action)
{
  // get reference to the box
  SbXfBox3f & box = action->getXfBoundingBox();

  // store current bbox
  SbXfBox3f incomingbox = box;

  // accumulation variables
  SbVec3f acccenter(0.0f, 0.0f, 0.0f);
  int numCenters = 0;
  
  // make current box empty to calculate bbox of this separator
  box.makeEmpty();
  box.setTransform(SbMatrix::identity());

  // traverse all children, calculate the local bbox
  inherited::getBoundingBox(action);
  SbXfBox3f lbbox = box;

  // Empty the box again
  box.makeEmpty();
  box.setTransform(SbMatrix::identity());

  for (int i=0; i < matrix.getNum(); i++) {
    // Apply transformation to the local bbox
    SbXfBox3f tbbox = lbbox;
    tbbox.transform(matrix[i]);

    // Accumulate center
    acccenter += tbbox.getCenter();
    numCenters++;

    // expand box by transformed bbox
    if (!tbbox.isEmpty()) box.extendBy(tbbox);
  }

  // Extend the bbox by the incoming bbox
  if (!incomingbox.isEmpty()) box.extendBy(incomingbox);

  if (numCenters != 0) {
    action->resetCenter();
    action->setCenter(acccenter / float(numCenters), TRUE);
  }
}

// Doc in superclass.
void
SoMultipleCopy::GLRender(SoGLRenderAction * action)
{
  SoMultipleCopy::doAction((SoAction*)action);
}

// Doc in superclass
void
SoMultipleCopy::audioRender(SoAudioRenderAction * action)
{
  SoMultipleCopy::doAction((SoAction*)action);
}

// Doc in superclass.
SbBool
SoMultipleCopy::affectsState(void) const
{
  // state is pushed/popped for each traversal
  return FALSE;
}

// Doc in superclass.
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

// Doc in superclass.
void
SoMultipleCopy::callback(SoCallbackAction *action)
{
  SoMultipleCopy::doAction((SoAction*)action);
}

// Doc in superclass.
void
SoMultipleCopy::pick(SoPickAction *action)
{
  // We came across what we think is a bug in TGS/SGI OIV when
  // implementing picking for this node and testing against the
  // original Inventor library. The SoPickedPoint class can return the
  // object space point, normal and texture coordinates. TGS/SGI OIV
  // do not consider the translation inside this node before returning
  // the object space data from SoPickedPoint, since the path in
  // SoPickedPoint does not say anything about on which copy the pick
  // occured.
  //
  // We solved this simply by extending SoPickedPoint for storing both
  // world space and object space data.

  SoMultipleCopy::doAction((SoAction*)action);
}

// Doc in superclass.
void
SoMultipleCopy::handleEvent(SoHandleEventAction *action)
{
  inherited::handleEvent(action);
}

// Doc in superclass.
void
SoMultipleCopy::getMatrix(SoGetMatrixAction *action)
{
  // path does not specify which copy to traverse
  inherited::getMatrix(action);
}

// Doc in superclass.
void
SoMultipleCopy::search(SoSearchAction *action)
{
  SoNode::search(action);
  inherited::search(action);
}

// Doc in superclass.
void
SoMultipleCopy::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoMultipleCopy::doAction((SoAction*)action);
}
