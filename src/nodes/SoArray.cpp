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
  \class SoArray SoArray.h Inventor/nodes/SoArray.h
  \brief The SoArray class is a group node for setting up regular arrays of subgraphs.
  \ingroup nodes

  SoArray presents a convenient way of duplicating a node (typically a
  shape node) or a complete subgraph in 1 to 3 dimensions.

  The child node or subgraph can only be translated by regular offsets
  for all dimensions.

  For more flexible functionality for duplication of geometry, see the
  SoMultipleCopy group node, which can do general transformations
  (including rotation and scaling) for it's child.

  \sa SoMultipleCopy
*/



#include <Inventor/nodes/SoArray.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoSwitchElement.h>
#include <Inventor/misc/SoState.h>

/*!
  \enum SoArray::Origin

  The possible settings for the SoArray::origin field.
*/


/*!
  \var SoSFEnum SoArray::origin

  Where the origin of the array should be set, ie how the array
  elements will be distributed from the local origo.

  Default value is SoArray::FIRST.
*/

/*!
  \var SoSFShort SoArray::numElements1
  Number of duplicates for each X axis row. Default 1.
*/
/*!
  \var SoSFShort SoArray::numElements2
  Number of duplicates for each Y axis row. Default 1.
*/
/*!
  \var SoSFShort SoArray::numElements3
  Number of duplicates for each Z axis row. Default 1.
*/

/*!
  \var SoSFVec3f SoArray::separation1

  Distance in current units between the center point of each element
  along the X axis. Default [1.0, 0.0, 0.0].
*/
/*!
  \var SoSFVec3f SoArray::separation2

  Distance in current units between the center point of each element
  along the Y axis. Default [0.0, 1.0, 0.0].
*/
/*!
  \var SoSFVec3f SoArray::separation3

  Distance in current units between the center point of each element
  along the Z axis. Default [0.0, 0.0, 1.0].
*/

// *************************************************************************

SO_NODE_SOURCE(SoArray);

/*!
  Constructor.
*/
SoArray::SoArray(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoArray);

  SO_NODE_ADD_FIELD(origin, (SoArray::FIRST));
  SO_NODE_ADD_FIELD(numElements1, (1));
  SO_NODE_ADD_FIELD(numElements2, (1));
  SO_NODE_ADD_FIELD(numElements3, (1));
  SO_NODE_ADD_FIELD(separation1, (SbVec3f(1, 0, 0)));
  SO_NODE_ADD_FIELD(separation2, (SbVec3f(0, 1, 0)));
  SO_NODE_ADD_FIELD(separation3, (SbVec3f(0, 0, 1)));


  SO_NODE_DEFINE_ENUM_VALUE(Origin, FIRST);
  SO_NODE_DEFINE_ENUM_VALUE(Origin, CENTER);
  SO_NODE_DEFINE_ENUM_VALUE(Origin, LAST);
  SO_NODE_SET_SF_ENUM_TYPE(origin, Origin);
}

/*!
  Destructor.
*/
SoArray::~SoArray()
{
}

// Doc in superclass.
void
SoArray::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoArray, SO_FROM_INVENTOR_1);
}

// Doc in superclass.
void
SoArray::getBoundingBox(SoGetBoundingBoxAction * action)
{
#if 0 // OBSOLETED: mortene's old (buggy ?) code (removed 19990423, pederb)
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

  for (int i=0; i < numElements3.getValue(); i++) {
    for (int j=0; j < numElements2.getValue(); j++) {
      for (int k=0; k < numElements1.getValue(); k++) {

        float multfactor_i = float(i);
        float multfactor_j = float(j);
        float multfactor_k = float(k);

        switch (origin.getValue()) {
        case SoArray::FIRST:
          break;
        case SoArray::CENTER:
          multfactor_i = -float(numElements3.getValue()-1.0f)/2.0f + float(i);
          multfactor_j = -float(numElements2.getValue()-1.0f)/2.0f + float(j);
          multfactor_k = -float(numElements1.getValue()-1.0f)/2.0f + float(k);
          break;
        case SoArray::LAST:
          multfactor_i = -multfactor_i;
          multfactor_j = -multfactor_j;
          multfactor_k = -multfactor_k;
          break;

        default: assert(0); break;
        }

        SbVec3f instance_pos =
          separation3.getValue() * multfactor_i +
          separation2.getValue() * multfactor_j +
          separation1.getValue() * multfactor_k;

#if 0 // debug
        SoDebugError::postInfo("SoArray::getBoundingBox",
                               "instance_pos: <%f, %f, %f>",
                               instance_pos[0],
                               instance_pos[1],
                               instance_pos[2]);
#endif // debug

        SbMatrix mat;
        mat.setTranslate(instance_pos);

        action->getState()->push();

        SoSwitchElement::set(action->getState(),
                             i * numElements2.getValue() *
                             numElements1.getValue() +
                             j * numElements1.getValue() + k);

        // make current box empty to calculate bbox of this separator
        box.makeEmpty();
        box.setTransform(SbMatrix::identity());

        // set local matrix to identity
        SoModelMatrixElement::set(action->getState(), this, mat);

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
    }
  }

  // transform the local bbox by stored model matrix
  if (!box.isEmpty()) box.transform(mat);
  if (!incomingbox.isEmpty()) box.extendBy(incomingbox);

  if (numCenters != 0)
    action->setCenter(acccenter / numCenters, FALSE);

#else // "new" code, 19990423, pederb

  float curri = 0.0f;
  float currj = 0.0f;
  float currk = 0.0f;

  float inci = 1.0f;
  float incj = 1.0f;
  float inck = 1.0f;

  // accumulation variables
  SbVec3f acccenter(0.0f, 0.0f, 0.0f);
  int numCenters = 0;

  switch (origin.getValue()) {
  case SoArray::FIRST:
    break;
  case SoArray::CENTER:
    curri = -(numElements3.getValue()-1.0f)/2.0f;
    currj = -(numElements2.getValue()-1.0f)/2.0f;
    currk = -(numElements1.getValue()-1.0f)/2.0f;
    break;
  case SoArray::LAST:
    inci = -1.0f;
    incj = -1.0f;
    inck = -1.0f;
    break;
  default: assert(0); break;
  }

  float initj = currj;
  float initk = currk;

  int N = 0;
  for (int i=0; i < numElements3.getValue(); i++) {
    currj = initj;
    for (int j=0; j < numElements2.getValue(); j++) {
      currk = initk;
      for (int k=0; k < numElements1.getValue(); k++) {

        SbVec3f instance_pos =
          separation3.getValue() * curri +
          separation2.getValue() * currj +
          separation1.getValue() * currk;

        action->getState()->push();

        // translate bbox matrix
        SoModelMatrixElement::translateBy(action->getState(),
                                              this, instance_pos);
        SoSwitchElement::set(action->getState(),++N);

        inherited::getBoundingBox(action);

        // If center point is set, accumulate center.
        if (action->isCenterSet()) {
          acccenter += action->getCenter();
          numCenters++;
          action->resetCenter();
        }
        // pop back to the original bboxmatrix
        action->getState()->pop();
        currk += inck;
      }
      currj += incj;
    }
    curri += inci;
  }

  if (numCenters != 0)
    action->setCenter(acccenter / float(numCenters), FALSE);
#endif // end of new code by pederb
}

// Doc in superclass.
void
SoArray::GLRender(SoGLRenderAction * action)
{
  SoArray::doAction(action);
}

// Doc in superclass.
SbBool
SoArray::affectsState(void) const
{
  return FALSE; // state is pushed/popped for each traversal
}

// Doc in superclass.
void
SoArray::doAction(SoAction *action)
{
  int N = 0;
  for (int i=0; i < numElements3.getValue(); i++) {
    for (int j=0; j < numElements2.getValue(); j++) {
      for (int k=0; k < numElements1.getValue(); k++) {

        float multfactor_i = float(i);
        float multfactor_j = float(j);
        float multfactor_k = float(k);

        switch (origin.getValue()) {
        case SoArray::FIRST:
          break;
        case SoArray::CENTER:
          multfactor_i = -float(numElements3.getValue()-1.0f)/2.0f + float(i);
          multfactor_j = -float(numElements2.getValue()-1.0f)/2.0f + float(j);
          multfactor_k = -float(numElements1.getValue()-1.0f)/2.0f + float(k);
          break;
        case SoArray::LAST:
          multfactor_i = -multfactor_i;
          multfactor_j = -multfactor_j;
          multfactor_k = -multfactor_k;
          break;

        default: assert(0); break;
        }

        SbVec3f instance_pos =
          separation3.getValue() * multfactor_i +
          separation2.getValue() * multfactor_j +
          separation1.getValue() * multfactor_k;

        action->getState()->push();

        SoSwitchElement::set(action->getState(),
                             ++N);

        SoModelMatrixElement::translateBy(action->getState(), this,
                                          instance_pos);

        inherited::doAction(action);
        action->getState()->pop();
      }
    }
  }
}

// Doc in superclass.
void
SoArray::callback(SoCallbackAction *action)
{
  SoArray::doAction((SoAction*)action);
}

// Doc in superclass.
void
SoArray::pick(SoPickAction *action)
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

  SoArray::doAction((SoAction*)action);
}

// Doc in superclass.
void
SoArray::handleEvent(SoHandleEventAction *action)
{
  SoNode::handleEvent(action);
  inherited::handleEvent(action);
}

// Doc in superclass
void
SoArray::audioRender(SoAudioRenderAction * action)
{
  SoArray::doAction((SoAction*)action);
}

// Doc in superclass.
void
SoArray::getMatrix(SoGetMatrixAction *action)
{
  // path does not specify which copy to traverse
  inherited::getMatrix(action);
}

// Doc in superclass.
void
SoArray::search(SoSearchAction * action)
{
  SoNode::search(action);
  inherited::search(action);
}

// Doc in superclass.
void
SoArray::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoArray::doAction((SoAction*)action);
}
