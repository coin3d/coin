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
  \class SoArray SoArray.h Inventor/nodes/SoArray.h
  \brief The SoArray class ...
  \ingroup nodes

  FIXME: write class doc
*/



#include <Inventor/nodes/SoArray.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#if !defined(COIN_EXCLUDE_SOSTATE)
#include <Inventor/misc/SoState.h>
#endif // !COIN_EXCLUDE_SOSTATE

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOMODELMATRIXELEMENT)
#include <Inventor/elements/SoModelMatrixElement.h>
#endif // !COIN_EXCLUDE_SOMODELMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOBBOXMODELMATRIXELEMENT)
#include <Inventor/elements/SoBBoxModelMatrixElement.h>
#endif // !COIN_EXCLUDE_SOBBOXMODELMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOSWITCHELEMENT)
#include <Inventor/elements/SoSwitchElement.h>
#endif // !COIN_EXCLUDE_SOSWITCHELEMENT

#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/misc/SoChildList.h>

/*!
  \enum SoArray::Origin
  FIXME: write documentation for enum
*/
/*!
  \var SoArray::Origin SoArray::FIRST
  FIXME: write documentation for enum definition
*/
/*!
  \var SoArray::Origin SoArray::CENTER
  FIXME: write documentation for enum definition
*/
/*!
  \var SoArray::Origin SoArray::LAST
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoArray::origin
  FIXME: write documentation for field
*/
/*!
  \var SoSFShort SoArray::numElements1
  FIXME: write documentation for field
*/
/*!
  \var SoSFShort SoArray::numElements2
  FIXME: write documentation for field
*/
/*!
  \var SoSFShort SoArray::numElements3
  FIXME: write documentation for field
*/
/*!
  \var SoSFVec3f SoArray::separation1
  FIXME: write documentation for field
*/
/*!
  \var SoSFVec3f SoArray::separation2
  FIXME: write documentation for field
*/
/*!
  \var SoSFVec3f SoArray::separation3
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoArray);

/*!
  Constructor.
*/
SoArray::SoArray()
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

/*!
  Does initialization common for all objects of the
  SoArray class. This includes setting up the
  type system, among other things.
*/
void
SoArray::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoArray);
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoArray::getBoundingBox(SoGetBoundingBoxAction * action)
{
#if 0 // mortene's old (buggy ?) code (removed 19990423, pederb)
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

          // FIXME: catch w/SoDebugError. 19990324 mortene.
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
        SoBBoxModelMatrixElement::set(action->getState(), this, mat);

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
    // FIXME: catch w/SoDebugError. 19990324 mortene.
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
        SoBBoxModelMatrixElement::translateBy(action->getState(),
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
    action->setCenter(acccenter / numCenters, FALSE);
#endif // end of new code by pederb
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void
SoArray::GLRender(SoGLRenderAction * action)
{
  SoArray::doAction(action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION


/*!
  FIXME: write doc
*/
SbBool
SoArray::affectsState(void) const
{
  return FALSE; // state is pushed/popped for each traversal
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
*/
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

          // FIXME: catch w/SoDebugError. 19990324 mortene.
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
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
*/
void
SoArray::callback(SoCallbackAction *action)
{
  SoArray::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  We came across what we think is a bug in TGS/SGI OIV when
  implementing picking for this node. The SoPickedPoint class can
  return the object space point, normal and texture
  coordinates. TGS/SGI OIV do not consider the translation inside this
  node before returning the object space data from SoPickedPoint,
  since the path in SoPickedPoint does not say anything about on which
  copy the pick occured. We solved this simply by storing both world
  space and object space data in SoPickedPoint.
*/
void
SoArray::pick(SoPickAction *action)
{
  SoArray::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
/*!
  FIXME: write doc
*/
void
SoArray::handleEvent(SoHandleEventAction *action)
{
  SoNode::handleEvent(action);
  inherited::handleEvent(action);
}
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION

#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
/*!
  FIXME: write doc
*/
void
SoArray::getMatrix(SoGetMatrixAction *action)
{
  // path does not specify which copy to traverse
  inherited::getMatrix(action);
}
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION

#if !defined(COIN_EXCLUDE_SOSEARCHACTION)
/*!
  FIXME: write doc
*/
void
SoArray::search(SoSearchAction * action)
{
  SoNode::search(action);
  inherited::search(action);
}
#endif // !COIN_EXCLUDE_SOSEARCHACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
*/
void
SoArray::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoArray::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
