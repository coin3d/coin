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
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoTransformManip SoTransformManip.h Inventor/manips/SoTransformManip.h
  \brief The SoTransformManip class is used to manipulate transformations.
  \ingroup manips

  A manipulator is used by replacing the node you want to edit in the
  graph with the manipulator. Draggers are used to to manipulate the
  node. When manipulation is finished, the node is put back into the
  graph, replacing the manipulator.
*/

#include <Inventor/manips/SoTransformManip.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/draggers/SoDragger.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/SoNodeKitPath.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SO_NODE_SOURCE(SoTransformManip);


// Documented in superclass
void
SoTransformManip::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTransformManip);
}

/*!
  The constructor.
*/
SoTransformManip::SoTransformManip(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTransformManip);

  this->children = new SoChildList(this);

  this->rotateFieldSensor = new SoFieldSensor(SoTransformManip::fieldSensorCB, this);
  this->rotateFieldSensor->setPriority(0);
  this->translFieldSensor = new SoFieldSensor(SoTransformManip::fieldSensorCB, this);
  this->translFieldSensor->setPriority(0);
  this->scaleFieldSensor = new SoFieldSensor(SoTransformManip::fieldSensorCB, this);
  this->scaleFieldSensor->setPriority(0);
  this->centerFieldSensor = new SoFieldSensor(SoTransformManip::fieldSensorCB, this);
  this->centerFieldSensor->setPriority(0);
  this->scaleOrientFieldSensor = new SoFieldSensor(SoTransformManip::fieldSensorCB, this);
  this->scaleOrientFieldSensor->setPriority(0);

  this->attachSensors(TRUE);

  // should I set a dragger here? probably not, pederb
}

/*!
  The destructor.
*/
SoTransformManip::~SoTransformManip()
{
  this->setDragger(NULL);

  delete this->rotateFieldSensor;
  delete this->translFieldSensor;
  delete this->scaleFieldSensor;
  delete this->centerFieldSensor;
  delete this->scaleOrientFieldSensor;

  delete this->children;
}

/*!
  Sets a dragger for this manipulator. The default dragger is a
  SoTransformerDragger.
*/
void
SoTransformManip::setDragger(SoDragger * newdragger)
{
  SoDragger *olddragger = this->getDragger();
  if (olddragger) {
    olddragger->removeValueChangedCallback(SoTransformManip::valueChangedCB, this);
    this->children->remove(0);
  }
  if (newdragger != NULL) {
    if (this->children->getLength() > 0) {
      this->children->set(0, newdragger);
    }
    else {
      this->children->append(newdragger);
      SoTransformManip::fieldSensorCB(this, NULL);
      newdragger->addValueChangedCallback(SoTransformManip::valueChangedCB, this);
    }
  }
}

/*!
  Returns the dragger used by this manipulator.
*/
SoDragger *
SoTransformManip::getDragger(void)
{
  if (this->children->getLength() > 0) {
    SoNode *node = (*children)[0];
    if (node->isOfType(SoDragger::getClassTypeId()))
      return (SoDragger*)node;
    else {
#if COIN_DEBUG
      SoDebugError::post("SoTransformManip::getDragger",
                         "Child is not a dragger!");
#endif // COIN_DEBUG
    }
  }
  return NULL;
}

/*!
  Replaces the node specified by \a path with this manipulator.
  The manipulator will copy the field data from the node, to make
  it affect the state in the same way as the node.
*/
SbBool
SoTransformManip::replaceNode(SoPath * path)
{
  SoFullPath *fullpath = (SoFullPath*)path;
  SoNode *fulltail = fullpath->getTail();
  if (!fulltail->isOfType(SoTransform::getClassTypeId())) {
#if COIN_DEBUG
    SoDebugError::post("SoTransformManip::replaceNode",
                       "end of path (%p) is not an SoTransform, but an %s",
                       fulltail, fulltail->getTypeId().getName().getString());
#endif // COIN_DEBUG
    return FALSE;
  }
  SoNode *tail = path->getTail();
  if (tail->isOfType(SoBaseKit::getClassTypeId())) {
    SoBaseKit *kit = (SoBaseKit*) ((SoNodeKitPath*)path)->getTail();
    SbString partname = kit->getPartString(path);
    if (partname != "") {
      SoTransform *oldpart = (SoTransform*) kit->getPart(partname, TRUE);
      if (oldpart != NULL) {
        this->attachSensors(FALSE);
        this->transferFieldValues(oldpart, this);
        this->attachSensors(TRUE);
        SoTransformManip::fieldSensorCB(this, NULL);
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
    SoDebugError::post("SoTransformManip::replaceNode",
                       "Path is too short");
#endif // COIN_DEBUG
    return FALSE;
  }
  SoNode *parent = fullpath->getNodeFromTail(1);
  if (!parent->isOfType(SoGroup::getClassTypeId())) {
#if COIN_DEBUG
    SoDebugError::post("SoTransformManip::replaceNode",
                       "Parent node %p is not an SoGroup, but %s",
                       parent, parent->getTypeId().getName().getString());
#endif // COIN_DEBUG
    return FALSE;
  }
  this->ref();
  this->attachSensors(FALSE);
  this->transferFieldValues((SoTransform*)fulltail, this);
  this->attachSensors(TRUE);
  SoTransformManip::fieldSensorCB(this, NULL);

  ((SoGroup*)parent)->replaceChild(fulltail, this);
  this->unrefNoDelete();
  return TRUE;
}

// Documented in superclass
void
SoTransformManip::doAction(SoAction * action)
{
  int numindices;
  const int *indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    this->children->traverse(action, 0, indices[numindices-1]);
  }
  else {
    this->children->traverse(action);
  }
}

// Documented in superclass
void
SoTransformManip::callback(SoCallbackAction * action)
{
  SoTransformManip::doAction(action);
  SoTransform::callback(action);
}

// Documented in superclass
void
SoTransformManip::GLRender(SoGLRenderAction * action)
{
  SoTransformManip::doAction(action);
  SoTransform::GLRender(action);
}

// Documented in superclass
void
SoTransformManip::getBoundingBox(SoGetBoundingBoxAction * action)
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
  SoTransform::getBoundingBox(action);
  if (action->isCenterSet()) {
    center += action->getCenter();
    numcenters++;
    action->resetCenter();
  }
  if (numcenters != 0) {
    action->setCenter(center / numcenters, FALSE);
  }
}

// Documented in superclass
void
SoTransformManip::getMatrix(SoGetMatrixAction * action)
{  
  int numindices;
  const int *indices;
  switch (action->getPathCode(numindices, indices)) {
  case SoAction::NO_PATH:
    SoTransform::getMatrix(action);
    break;
  case SoAction::BELOW_PATH:
    SoTransform::getMatrix(action);
    break;
  case SoAction::IN_PATH:
    this->children->traverse(action, 0, indices[numindices-1]);
    break;
  case SoAction::OFF_PATH:
    this->children->traverse(action);
    SoTransform::getMatrix(action);
    break;
  default:
    assert(0 && "unknown path code");
    break;
  }
}

// Documented in superclass
void
SoTransformManip::handleEvent(SoHandleEventAction * action)
{
  SoTransformManip::doAction(action);
  SoTransform::handleEvent(action);
}

// Documented in superclass
void
SoTransformManip::pick(SoPickAction * action)
{
  SoTransformManip::doAction(action);
  SoTransform::pick(action);
}

// Documented in superclass
void
SoTransformManip::search(SoSearchAction * action)
{
  inherited::search(action);
  if (action->isFound()) return;
  SoTransformManip::doAction(action);
}

/*!
  Returns the children of this node. This node only has the dragger
  as a child.
*/
SoChildList *
SoTransformManip::getChildren(void) const
{
  return this->children;
}

/*!
  \internal
  Callback to update field values when motion matrix changes.
*/
void
SoTransformManip::valueChangedCB(void * m, SoDragger * dragger)
{
  SoTransformManip * thisp = (SoTransformManip*)m;

  SbMatrix matrix = dragger->getMotionMatrix();

  SbVec3f t, s, c = thisp->center.getValue();
  SbRotation r, so;
  matrix.getTransform(t, r, s, so, c);

  thisp->attachSensors(FALSE);
  if (thisp->translation.getValue() != t) {
    thisp->translation = t;
  }
  if (thisp->scaleFactor.getValue() != s) {
    thisp->scaleFactor = s;
  }
  if (thisp->rotation.getValue() != r) {
    thisp->rotation = r;
  }
  if (thisp->scaleOrientation.getValue() != so) {
    thisp->scaleOrientation = so;
  }
  //
  // center will never be affected by motion matrix.
  //
  thisp->attachSensors(TRUE);
}

/*!
  \internal
  Callback to update motion matrix when a field is modified.
*/
void
SoTransformManip::fieldSensorCB(void * m, SoSensor *)
{
  SoTransformManip *thisp = (SoTransformManip*)m;
  SoDragger *dragger = thisp->getDragger();
  if (dragger != NULL) {
    SbMatrix matrix;
    matrix.setTransform(thisp->translation.getValue(),
                        thisp->rotation.getValue(),
                        thisp->scaleFactor.getValue(),
                        thisp->scaleOrientation.getValue(),
                        thisp->center.getValue());
    dragger->setMotionMatrix(matrix);
  }
}

/*!
  \internal
  Overloaded to copy the dragger.
*/
void
SoTransformManip::copyContents(const SoFieldContainer * fromfc, SbBool copyconnections)
{
  assert(fromfc->isOfType(SoTransformManip::getClassTypeId()));
  SoDragger *dragger = ((SoTransformManip*)fromfc)->getDragger();
  this->setDragger(dragger ? (SoDragger*) dragger->copy() : NULL);
  inherited::copyContents(fromfc, copyconnections);
}

/*!
  \internal
  Copies field values from one node to the other.
*/
void
SoTransformManip::transferFieldValues(const SoTransform * from, SoTransform * to)
{
  to->translation = from->translation;
  to->rotation = from->rotation;
  to->scaleFactor = from->scaleFactor;
  to->scaleOrientation = from->scaleOrientation;
  to->center = from->center;
}

void
SoTransformManip::attachSensors(const SbBool onoff)
{
  if (onoff) {
    this->rotateFieldSensor->attach(&this->rotation);
    this->translFieldSensor->attach(&this->translation);
    this->scaleFieldSensor->attach(&this->scaleFactor);
    this->centerFieldSensor->attach(&this->center);
    this->scaleOrientFieldSensor->attach(&this->scaleOrientation);
  }
  else {
    this->rotateFieldSensor->detach();
    this->translFieldSensor->detach();
    this->scaleFieldSensor->detach();
    this->centerFieldSensor->detach();
    this->scaleOrientFieldSensor->detach();
  }
}
