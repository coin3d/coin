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
  \class SoClipPlaneManip SoClipPlaneManip.h Inventor/manips/SoClipPlaneManip.h
  \brief The SoClipPlaneManip class is used to manipulate clip planes.
  \ingroup manips

  <center>
  <img src="http://doc.coin3d.org/images/Coin/draggers/jack.png">
  </center>

  A manipulator is used by replacing the node you want to edit in the
  graph with the manipulator. Draggers are used to manipulate the
  node. When manipulation is finished, the node is put back into the
  graph, replacing the manipulator.

  An SoJackDragger is used by instances of this manipulator class as
  the default dragger when manipulating SoClipPlane nodes. See the
  documentation of SoJackDragger for details about how to control the
  manipulator.

  This manipulator is an extension versus the original SGI Inventor
  v2.1 API.  In addition to being a Coin extension, it is also present
  in TGS' Inventor implementation (with the same API).

  \since TGS Inventor 2.5
  \since Coin 1.0
*/


#include <Inventor/manips/SoClipPlaneManip.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/draggers/SoJackDragger.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/SoNodeKitPath.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SO_NODE_SOURCE(SoClipPlaneManip);

/*!
  \var SoSFVec3f * SoClipPlaneManip::draggerPosition
  \COININTERNAL
*/

/*!
  \var SoFieldSensor * SoClipPlaneManip::planeFieldSensor
  \COININTERNAL
*/
/*!
  \var SoFieldSensor * SoClipPlaneManip::onFieldSensor
  \COININTERNAL
*/
/*!
  \var SoFieldSensor * SoClipPlaneManip::draggerPosFieldSensor
  \COININTERNAL
*/
/*!
  \var SoChildList * SoClipPlaneManip::children
  \COININTERNAL
*/


// Documented in superclass
void
SoClipPlaneManip::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoClipPlaneManip, SO_FROM_INVENTOR_1);
}

/*!
  An SoJackDragger is set up here as the internal dragger used for
  manipulation of an SoClipPlane node.
*/
SoClipPlaneManip::SoClipPlaneManip(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoClipPlaneManip);
  SO_NODE_ADD_FIELD(draggerPosition, (0.0f, 0.0f, 0.0f));

  this->children = new SoChildList(this);

  this->planeFieldSensor =
    new SoFieldSensor(SoClipPlaneManip::fieldSensorCB, this);
  this->planeFieldSensor->setPriority(0);

  this->onFieldSensor =
    new SoFieldSensor(SoClipPlaneManip::fieldSensorCB, this);
  this->onFieldSensor->setPriority(0);

  this->draggerPosFieldSensor =
    new SoFieldSensor(SoClipPlaneManip::fieldSensorCB, this);
  this->draggerPosFieldSensor->setPriority(0);

  this->setDragger(new SoJackDragger);
}

/*!
  Destructor.
 */
SoClipPlaneManip::~SoClipPlaneManip()
{
  this->setDragger(NULL);

  delete this->draggerPosFieldSensor;
  delete this->onFieldSensor;
  delete this->planeFieldSensor;

  delete this->children;
}

/*!
  Sets a dragger to use for this manipulator. The default dragger is
  an SoJackDragger.
 */
void
SoClipPlaneManip::setDragger(SoDragger * newdragger)
{
  SoDragger * olddragger = this->getDragger();
  if (olddragger) {
    olddragger->removeValueChangedCallback(SoClipPlaneManip::valueChangedCB, this);
    this->children->remove(0);
  }
  if (newdragger != NULL) {
    if (this->children->getLength() > 0) {
      this->children->set(0, newdragger);
    }
    else {
      this->children->append(newdragger);
    }
    SoClipPlaneManip::fieldSensorCB(this, NULL);
    newdragger->addValueChangedCallback(SoClipPlaneManip::valueChangedCB, this);
  }
}

/*!
  Returns pointer to dragger used for interaction.
 */
SoDragger *
SoClipPlaneManip::getDragger(void)
{
  if (this->children->getLength() > 0) {
    SoNode *node = (*children)[0];
    if (node->isOfType(SoDragger::getClassTypeId()))
      return (SoDragger*)node;
    else {
#if COIN_DEBUG
      SoDebugError::post("SoClipPlaneManip::getDragger",
                         "Child is not a dragger!");
#endif // debug
    }
  }
  return NULL;
}

// Documented in superclass. Overridden to copy the internal dragger
// instance.
void
SoClipPlaneManip::copyContents(const SoFieldContainer * fromfc, SbBool copyconnections)
{
  assert(fromfc->isOfType(SoClipPlaneManip::getClassTypeId()));
  this->setDragger(((SoClipPlaneManip*)fromfc)->getDragger());
  inherited::copyContents(fromfc, copyconnections);
}

/*!
  Sets the clip plane based on the center of \a box and \a normal.
  The size of \a box is used as a scale factor to the dragger,
  multiplied with \a draggerscalefactor.
*/
void
SoClipPlaneManip::setValue(const SbBox3f & box, const SbVec3f & planenormal, float draggerscalefactor)
{
  this->attachSensors(FALSE);
  SbPlane newplane(planenormal, box.getCenter());
  this->plane = newplane;
  // FIXME: investigate how dragger should be scaled (pederb, 20000606)
  this->attachSensors(TRUE);
  SoClipPlaneManip::fieldSensorCB(this, NULL);
}

/*!
  Replaces the node specified by \a path with this manipulator.
  The manipulator will copy the field data from the node, to make
  it affect the state in the same way as the node.
*/
SbBool
SoClipPlaneManip::replaceNode(SoPath * path)
{
  SoFullPath *fullpath = (SoFullPath*)path;
  SoNode *fulltail = fullpath->getTail();
  if (!fulltail->isOfType(SoClipPlane::getClassTypeId())) {
#if COIN_DEBUG
    SoDebugError::post("SoClipPlaneManip::replaceNode",
                       "End of path is not a SoClipPlane");
#endif // debug
    return FALSE;
  }
  SoNode *tail = path->getTail();
  if (tail->isOfType(SoBaseKit::getClassTypeId())) {
    SoBaseKit *kit = (SoBaseKit*) ((SoNodeKitPath*)path)->getTail();
    SbString partname = kit->getPartString(path);
    if (partname != "") {
      SoClipPlane *oldpart = (SoClipPlane*) kit->getPart(partname, TRUE);
      if (oldpart != NULL) {
        this->attachSensors(FALSE);
        this->transferFieldValues(oldpart, this);
        this->attachSensors(TRUE);
        SoClipPlaneManip::fieldSensorCB(this, NULL);
        kit->setPart(partname, this);
        return TRUE;
      }
      else {
        return FALSE;
      }
    }
  }
  if (fullpath->getLength() < 2) {
#if COIN_DEBUG
    SoDebugError::post("SoClipPlaneManip::replaceNode",
                       "Path is too short");
#endif // debug
    return FALSE;
  }
  SoNode *parent = fullpath->getNodeFromTail(1);
  if (!parent->isOfType(SoGroup::getClassTypeId())) {
#if COIN_DEBUG
    SoDebugError::post("SoClipPlaneManip::replaceNode",
                       "Parent node is not a group");
#endif // debug
    return FALSE;
  }
  this->ref();
  this->attachSensors(FALSE);
  this->transferFieldValues((SoClipPlane*)fulltail, this);
  this->attachSensors(TRUE);
  SoClipPlaneManip::fieldSensorCB(this, NULL);

  ((SoGroup*)parent)->replaceChild(fulltail, this);
  this->unrefNoDelete();
  return TRUE;
}

// doc from parent
void
SoClipPlaneManip::doAction(SoAction * action)
{
  int numindices;
  const int *indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    this->children->traverseInPath(action, numindices, indices);
  }
  else {
    this->children->traverse(action);
  }
}

// doc from parent
void
SoClipPlaneManip::callback(SoCallbackAction * action)
{
  SoClipPlaneManip::doAction(action);
  SoClipPlane::callback(action);
}

// doc from parent
void
SoClipPlaneManip::GLRender(SoGLRenderAction * action)
{
  SoClipPlaneManip::doAction(action);
  SoClipPlane::GLRender(action);
}

// doc from parent
void
SoClipPlaneManip::getBoundingBox(SoGetBoundingBoxAction * action)
{
  int numindices;
  const int *indices;
  int lastchild;
  SbVec3f center(0.0f, 0.0f, 0.0f);
  int numcenters = 0;

  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    lastchild  = indices[numindices-1];
  }
  else {
    lastchild = this->children->getLength() - 1;
  }
  for (int i = 0; i <= lastchild; i++) {
    this->children->traverse(action, i, i);
    if (action->isCenterSet()) {
      center += action->getCenter();
      numcenters++;
      action->resetCenter();
    }
  }
  SoClipPlane::getBoundingBox(action);
  if (action->isCenterSet()) {
    center += action->getCenter();
    numcenters++;
    action->resetCenter();
  }
  if (numcenters != 0) {
    action->setCenter(center / numcenters, FALSE);
  }
}

// doc from parent
void
SoClipPlaneManip::getMatrix(SoGetMatrixAction * action)
{
  int numindices;
  const int *indices;
  switch (action->getPathCode(numindices, indices)) {
  case SoAction::NO_PATH:
  case SoAction::BELOW_PATH:
    break;
  case SoAction::IN_PATH:
    this->children->traverseInPath(action, numindices, indices);
    break;
  case SoAction::OFF_PATH:
    this->children->traverse(action);
    break;
  default:
    assert(0 && "unknown path code");
    break;
  }
}

// doc from parent
void
SoClipPlaneManip::handleEvent(SoHandleEventAction * action)
{
  SoClipPlaneManip::doAction(action);
  SoClipPlane::handleEvent(action);
}

// doc from parent
void
SoClipPlaneManip::pick(SoPickAction * action)
{
  SoClipPlaneManip::doAction(action);
  SoClipPlane::pick(action);
}

// doc from parent
void
SoClipPlaneManip::search(SoSearchAction * action)
{
  inherited::search(action);
  if (action->isFound()) return;
  SoClipPlaneManip::doAction(action);
}

/*!
  Returns the children of this node.
*/
SoChildList *
SoClipPlaneManip::getChildren(void) const
{
  return this->children;
}

/*!
  \COININTERNAL
  Called whenever dragger is touched.
*/
void
SoClipPlaneManip::valueChangedCB(void * m, SoDragger * dragger)
{
  SoClipPlaneManip * thisp = (SoClipPlaneManip*)m;

  SbMatrix matrix = dragger->getMotionMatrix();
  SbVec3f t, s;
  SbRotation r, so;
  matrix.getTransform(t, r, s, so);

  SbVec3f direction(0.0f, 1.0f, 0.0f);
  r.multVec(direction, direction);
  direction.normalize();
  SbPlane plane(direction, t);

  thisp->attachSensors(FALSE);
  if (thisp->plane.getValue() != plane) {
    thisp->plane = plane;
  }
  if (t != thisp->draggerPosition.getValue()) {
    thisp->draggerPosition = t;
  }
  thisp->attachSensors(TRUE);
}

/*!
  \COININTERNAL
  Called whenever one of the fields changes value.
*/
void
SoClipPlaneManip::fieldSensorCB(void * m, SoSensor * d)
{
  SoClipPlaneManip *thisp = (SoClipPlaneManip*)m;
  SoDragger *dragger = thisp->getDragger();
  if (dragger != NULL) {
    SbPlane plane = thisp->plane.getValue();
    SbVec3f normal = plane.getNormal();
    float dist = plane.getDistanceFromOrigin();
    SbMatrix matrix = dragger->getMotionMatrix();
    SbVec3f t, s;
    SbRotation r, so;
    matrix.getTransform(t, r, s, so);
    r.setValue(SbVec3f(0.0f, 1.0f, 0.0f), normal);
    t = normal*dist;
    matrix.setTransform(t, r, s, so);
    dragger->setMotionMatrix(matrix);

    // make sure draggerPosition field is up-to-date
    thisp->attachSensors(FALSE);
    if (t != thisp->draggerPosition.getValue()) {
      thisp->draggerPosition = t;
    }
    thisp->attachSensors(TRUE);
  }
}

// Convenience method used to attach and detach field sensors.
void
SoClipPlaneManip::attachSensors(const SbBool onoff)
{
  if (onoff) {
    this->planeFieldSensor->attach(&this->plane);
    this->onFieldSensor->attach(&this->on);
    this->draggerPosFieldSensor->attach(&this->draggerPosition);
  }
  else {
    this->planeFieldSensor->detach();
    this->onFieldSensor->detach();
    this->draggerPosFieldSensor->detach();
  }
}

/*!
  Copies field values from one clip plane node to another.
*/
void
SoClipPlaneManip::transferFieldValues(const SoClipPlane * from, SoClipPlane * to)
{
  to->plane = from->plane;
  to->on = from->on;
}
