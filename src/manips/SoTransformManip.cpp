/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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

  The SoTransformManip class is an abstract class which should not be
  used directly -- use one of it's subclasses.
*/

/*!
  \var SoFieldSensor * SoTransformManip::rotateFieldSensor
  \COININTERNAL
*/

/*!
  \var SoFieldSensor * SoTransformManip::translFieldSensor
  \COININTERNAL
*/

/*!
  \var SoFieldSensor * SoTransformManip::scaleFieldSensor
  \COININTERNAL
*/

/*!
  \var SoFieldSensor * SoTransformManip::centerFieldSensor
  \COININTERNAL
*/

/*!
  \var SoFieldSensor * SoTransformManip::scaleOrientFieldSensor
  \COININTERNAL
*/

/*!
  \var SoChildList * SoTransformManip::children
  \COININTERNAL
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
  SO_NODE_INTERNAL_INIT_CLASS(SoTransformManip, SO_FROM_INVENTOR_1);
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

// Note: this documentation is also shared by some of the subclasses,
// so keep it general enough to also be valid for those.
/*!
  Sets up the dragger instance to be controlled by this manipulator.
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
    }
    SoTransformManip::fieldSensorCB(this, NULL);
    newdragger->addValueChangedCallback(SoTransformManip::valueChangedCB, this);
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
  Replaces a node at the tail end of \a path with this manipulator.
  The node must be an instance of a node class derived from
  SoTransform.

  The manipulator will when inserted automatically copy the field data
  from the node, to make it initially affect the state in the same way
  as the node.

  Returns \c TRUE if successful, and \c FALSE if the given node could
  not be replaced with the manipulator.  The operation will for
  instance fail if the tail node of the given path is not of
  SoTransform type, or for any other condition where it is not
  possible to replace the node with this manipulator.
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
    if (partname != "") {  // FIXME: isn't this an assert condition? 20010909 mortene.
      SoTransform *oldpart = (SoTransform*) kit->getPart(partname, TRUE);
      if (oldpart != NULL) {  // FIXME: isn't this an assert condition? 20010909 mortene.
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

  // This will happen if the path contains nothing but the single
  // SoTransform node (ie, the node is root, head and tail of the
  // path).
  if (fullpath->getLength() < 2) {
#if COIN_DEBUG
    SoDebugError::post("SoTransformManip::replaceNode", "Path is too short");
#endif // COIN_DEBUG
    return FALSE;
  }

  SoNode *parent = fullpath->getNodeFromTail(1);
  // This could at least happen if the parent of the SoTransform is an
  // user-extension node that is "SoGroup-like", but does not actually
  // inherit SoGroup.  Would be an immensely silly thing to do, but
  // it's good to be robust. :-}
  //
  // FIXME: are there any other conditions where this could hit?
  // Please elaborate in a code comment.  20010909 mortene.
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

  // FIXME: this looks too simple -- what if there's more than one
  // parent SoGroup for the SoTransform we're replacing.  Looks to me
  // like that could trigger a serious bug.
  //
  // (Should we allow and handle that case, or just detect and return
  // FALSE?)  20010909 mortene.
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
    this->children->traverseInPath(action, numindices, indices);
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
    this->children->traverseInPath(action, numindices, indices);
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
  \COININTERNAL
  Callback to update field values when motion matrix changes.
*/
void
SoTransformManip::valueChangedCB(void * m, SoDragger * dragger)
{
  if (dragger == NULL) return;

  SoTransformManip * thisp = (SoTransformManip*)m;

  SbMatrix matrix = dragger->getMotionMatrix();

  SbVec3f t, s, c = thisp->center.getValue();

  // get center from dragger if dragger has this field
  SoField * field = dragger->getField("center");
  if (field && field->isOfType(SoSFVec3f::getClassTypeId())) {
    c = ((SoSFVec3f*)field)->getValue();
  }

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
  if (thisp->center.getValue() != c) {
    thisp->center = c;
  }
  thisp->attachSensors(TRUE);
}

/*!
  \COININTERNAL
  Callback to update motion matrix when a field is modified.
*/
void
SoTransformManip::fieldSensorCB(void * m, SoSensor *)
{
  SoTransformManip *thisp = (SoTransformManip*)m;
  SoDragger *dragger = thisp->getDragger();
  if (dragger != NULL) {
    SbBool wasenabled = dragger->enableValueChangedCallbacks(FALSE);
    SbMatrix matrix;
    SbVec3f center = thisp->center.getValue();
    // test for dragger center field, and set it if it exists
    SoField * field = dragger->getField("center");
    if (field && field->isOfType(SoSFVec3f::getClassTypeId())) {
      ((SoSFVec3f*)field)->setValue(center);
    }
    matrix.setTransform(thisp->translation.getValue(),
                        thisp->rotation.getValue(),
                        thisp->scaleFactor.getValue(),
                        thisp->scaleOrientation.getValue(),
                        center);
    dragger->setMotionMatrix(matrix);
    if (wasenabled) {
      dragger->enableValueChangedCallbacks(TRUE);
      dragger->valueChanged();
    }
  }
}

// Documented in superclass. Overridden to copy the internal dragger
// instance.
void
SoTransformManip::copyContents(const SoFieldContainer * fromfc, SbBool copyconnections)
{
  assert(fromfc->isOfType(SoTransformManip::getClassTypeId()));
  SoDragger *dragger = ((SoTransformManip*)fromfc)->getDragger();
  this->setDragger(dragger ? (SoDragger*) dragger->copy() : NULL);
  inherited::copyContents(fromfc, copyconnections);
}

/*!
  \COININTERNAL
  Copies field values from one node to the other.
*/
void
SoTransformManip::transferFieldValues(const SoTransform * from, SoTransform * to)
{
  SoTransformManip * tomanip = 
    to->isOfType(SoTransformManip::getClassTypeId()) ?
    (SoTransformManip*) to : NULL;
  
  if (tomanip) tomanip->attachSensors(FALSE);    
  
  to->translation = from->translation.getValue();
  to->rotation = from->rotation.getValue();
  to->scaleFactor = from->scaleFactor.getValue();
  to->scaleOrientation = from->scaleOrientation.getValue();
  to->center = from->center.getValue();

  if (tomanip) {
    SoTransformManip::fieldSensorCB(tomanip, NULL);
    tomanip->attachSensors(TRUE);
  }
}

/*!
  Can be used by subclasses to attach/detach field sensors.
*/
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
