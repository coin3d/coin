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
  \class SoDragger SoDragger.h Inventor/draggers/SoDragger.h
  \brief The SoDragger class is the base class for all draggers.
  \ingroup draggers

  In holds the motion matrix, and offers lots of convenience methods
  for subdraggers. The motion matrix is used to modify the model matrix
  during traversal, and all draggers should update this during dragging.
*/

#include <Inventor/draggers/SoDragger.h>
#include <Inventor/draggers/SoCenterballDragger.h>
#include <Inventor/draggers/SoDirectionalLightDragger.h>
#include <Inventor/draggers/SoDragPointDragger.h>
#include <Inventor/draggers/SoHandleBoxDragger.h>
#include <Inventor/draggers/SoJackDragger.h>
#include <Inventor/draggers/SoPointLightDragger.h>
#include <Inventor/draggers/SoRotateCylindricalDragger.h>
#include <Inventor/draggers/SoRotateDiscDragger.h>
#include <Inventor/draggers/SoRotateSphericalDragger.h>
#include <Inventor/draggers/SoScale1Dragger.h>
#include <Inventor/draggers/SoScale2Dragger.h>
#include <Inventor/draggers/SoScale2UniformDragger.h>
#include <Inventor/draggers/SoScaleUniformDragger.h>
#include <Inventor/draggers/SoSpotLightDragger.h>
#include <Inventor/draggers/SoTabBoxDragger.h>
#include <Inventor/draggers/SoTabPlaneDragger.h>
#include <Inventor/draggers/SoTrackballDragger.h>
#include <Inventor/draggers/SoTransformBoxDragger.h>
#include <Inventor/draggers/SoTransformerDragger.h>
#include <Inventor/draggers/SoTranslate1Dragger.h>
#include <Inventor/draggers/SoTranslate2Dragger.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>

#include <Inventor/SbViewportRegion.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/SoPickedPoint.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <coindefs.h> // COIN_STUB()


#ifndef DOXYGEN_SKIP_THIS // Don't document internal classes.

class SoDraggerCache {
public:
  SoDraggerCache(SoDragger *parent) : path(4), dragger(parent) {

  }

  void checkUpdate(const SoFullPath *newpath, const int draggeridx) {
    if (path.getHead() != newpath->getHead()) this->update(newpath, draggeridx);
    else {
      int i;
      for (i = 1; i <= draggeridx; i++) {
        if (this->path.getIndex(i) != newpath->getIndex(i)) break;
        if (this->path.getNode(i) != newpath->getNode(i)) break;
      }
      if (i <= draggeridx) this->update(newpath, draggeridx);
    }
    this->updateMatrix();
  }

  void updateMatrix() {
    SoGetMatrixAction action(this->dragger->getViewportRegion());
    action.apply(&this->path);
    this->draggerToWorld = action.getMatrix();
    this->worldToDragger = action.getInverse();
  }

  SoTempPath path;
  SoDragger *dragger;
  SbMatrix draggerToWorld;
  SbMatrix worldToDragger;

  //
  // FIXME: cache more matrices here, pederb
  //

private:
  void update(const SoFullPath *newpath, const int draggeridx) {
    this->path.setHead(newpath->getHead());
    for (int i = 1; i <= draggeridx; i++) {
      this->path.append(newpath->getNode(i));
    }
  }
};

#endif // DOXYGEN_SKIP_THIS

SO_KIT_SOURCE(SoDragger);

float SoDragger::minScale = 0.001f;

/*!
  A protected constructor.
*/
SoDragger::SoDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoDragger);

  SO_KIT_ADD_CATALOG_ENTRY(motionMatrix, SoMatrixTransform, FALSE, topSeparator, geomSeparator, FALSE);

  SO_NODE_ADD_FIELD(isActive, (FALSE));

  SO_KIT_INIT_INSTANCE();

  this->minGesture = 8;
  this->eventAction = NULL;
  this->frontOnProjector = FRONT; // FIXME: ??
  this->valueChangedCBEnabled = TRUE;
  this->ignoreInBBox = FALSE;
  this->currentEvent = NULL;
  this->pickedPath = NULL;
  this->draggerCache = NULL;
}

/*!
  The destructor.
*/
SoDragger::~SoDragger()
{
  if (this->pickedPath) this->pickedPath->unref();
  delete this->draggerCache;
}

/*!
  Initializes this class and all built-in draggers.
*/
void
SoDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoDragger);

  SoDragger::initClasses();

  SoType type = SoDragger::getClassTypeId();
  SoRayPickAction::addMethod(type, SoNode::rayPickS);
}

/*!
  Initializes all built-in draggers.
*/
void
SoDragger::initClasses(void)
{
  SoCenterballDragger::initClass();
  SoDirectionalLightDragger::initClass();
  SoDragPointDragger::initClass();
  SoHandleBoxDragger::initClass();
  SoJackDragger::initClass();
  SoPointLightDragger::initClass();
  SoRotateCylindricalDragger::initClass();
  SoRotateDiscDragger::initClass();
  SoRotateSphericalDragger::initClass();
  SoScale1Dragger::initClass();
  SoScale2Dragger::initClass();
  SoScale2UniformDragger::initClass();
  SoScaleUniformDragger::initClass();
  SoSpotLightDragger::initClass();
  SoTabBoxDragger::initClass();
  SoTabPlaneDragger::initClass();
  SoTrackballDragger::initClass();
  SoTransformBoxDragger::initClass();
  SoTransformerDragger::initClass();
  SoTranslate1Dragger::initClass();
  SoTranslate2Dragger::initClass();
}

/*!
  Adds a callback which is called at the start of a drag, after
  the mouse butten 1 is pressed, and dragger is picked.
*/
void
SoDragger::addStartCallback(SoDraggerCB * func, void * data)
{
  this->startCB.addCallback((SoCallbackListCB*)func, data);
}

/*
  Removes a start callback,
  \sa addStartCallback()
*/
void
SoDragger::removeStartCallback(SoDraggerCB * func, void * data)
{
  this->startCB.removeCallback((SoCallbackListCB*)func, data);
}

/*!
  Adds a callback which is called for each mouse movement during
  dragging.
*/
void
SoDragger::addMotionCallback(SoDraggerCB * func, void * data)
{
  this->motionCB.addCallback((SoCallbackListCB*)func, data);
}

/*!
  Removes a motion callback.
  \sa addMotionCallback()
*/
void
SoDragger::removeMotionCallback(SoDraggerCB * func, void * data)
{
  this->motionCB.removeCallback((SoCallbackListCB*)func, data);
}

/*!
  Adds a callback which is called after dragging is finished.
*/
void
SoDragger::addFinishCallback(SoDraggerCB * func, void * data)
{
  this->finishCB.addCallback((SoCallbackListCB*)func, data);
}

/*!
  Removes a finish callback.
  \sa addFinishCallback()
*/
void
SoDragger::removeFinishCallback(SoDraggerCB * func, void * data)
{
  this->finishCB.removeCallback((SoCallbackListCB*)func, data);
}

/*!
  Adds a callback which is called after a dragger has changed a field.
  It is not called if the \e SoDragger::isActive field is changed.
  \sa enableValueChangedCallback
*/
void
SoDragger::addValueChangedCallback(SoDraggerCB * func, void * data)
{
  this->valueChangedCB.addCallback((SoCallbackListCB*)func, data);
}

/*!
  Remoes a value changed callback.
  \sa addValueChangedCallback()
*/
void
SoDragger::removeValueChangedCallback(SoDraggerCB * func, void * data)
{
  this->valueChangedCB.removeCallback((SoCallbackListCB*)func, data);
}

/*!
  Sets the number of pixel movement needed to trigger a constraint gesture.
  Default is 8 pixels.
*/
void
SoDragger::setMinGesture(int pixels)
{
  this->minGesture = pixels;
}

/*!
  Returns the minimum gesture pixels.
  \sa setMinGesture()
*/
int
SoDragger::getMinGesture(void) const
{
  return this->minGesture;
}

/*!
  Enable/disable value changed callbacks.
  \sa addValueChangedCallback()
*/
SbBool
SoDragger::enableValueChangedCallbacks(SbBool val)
{
  SbBool oldval = this->valueChangedCBEnabled;
  this->valueChangedCBEnabled = val;
  return oldval;
}

/*!
  Returns the motion matrix for this dragger.
*/
const SbMatrix &
SoDragger::getMotionMatrix(void)
{
  SoMatrixTransform *node = SO_GET_ANY_PART(this, "motionMatrix", SoMatrixTransform);
  assert(node);
  return node->matrix.getValue();
}

/*!
  Adds an event callback for events other then drag events.
  As soon as dragging starts, the dragger grabs all events (until
  mouse button is released). This method can be used to handle
  other events during dragging.
*/
void
SoDragger::addOtherEventCallback(SoDraggerCB * func, void * data)
{
  this->otherEventCB.addCallback((SoCallbackListCB*)func, data);
}

/*!
  Removes a other event callback.
  \sa addOtherEventCallback()
*/
void
SoDragger::removeOtherEventCallback(SoDraggerCB * func, void * data)
{
  this->otherEventCB.removeCallback((SoCallbackListCB*)func, data);
}

/*!
  Should be called by compound draggers to register child draggers.
*/
void
SoDragger::registerChildDragger(SoDragger * child)
{
  child->addStartCallback(SoDragger::childStartCB, this);
  child->addMotionCallback(SoDragger::childMotionCB, this);
  child->addFinishCallback(SoDragger::childFinishCB, this);
  child->addOtherEventCallback(SoDragger::childOtherEventCB, this);
  child->addValueChangedCallback(SoDragger::childTransferMotionAndValueChangedCB, this);
}

/*!
  Should be called by compound draggers to unregister child draggers.
*/
void
SoDragger::unregisterChildDragger(SoDragger * child)
{
  child->removeStartCallback(SoDragger::childStartCB, this);
  child->removeMotionCallback(SoDragger::childMotionCB, this);
  child->removeFinishCallback(SoDragger::childFinishCB, this);
  child->removeOtherEventCallback(SoDragger::childOtherEventCB, this);
  child->removeValueChangedCallback(SoDragger::childTransferMotionAndValueChangedCB, this);
}

/*!
  Should be called by compund draggers to register child draggers that
  should move independently of their parent.
*/
void
SoDragger::registerChildDraggerMovingIndependently(SoDragger * child)
{
  child->addStartCallback(SoDragger::childStartCB, this);
  child->addMotionCallback(SoDragger::childMotionCB, this);
  child->addFinishCallback(SoDragger::childFinishCB, this);
  child->addOtherEventCallback(SoDragger::childOtherEventCB, this);
  child->addValueChangedCallback(SoDragger::childValueChangedCB, this);
}

/*!
  Should be called by compund draggers to unregister child draggers.
  \sa registerChildDraggerMovingIndependently()
*/
void
SoDragger::unregisterChildDraggerMovingIndependently(SoDragger * child)
{
  child->removeStartCallback(SoDragger::childStartCB, this);
  child->removeMotionCallback(SoDragger::childMotionCB, this);
  child->removeFinishCallback(SoDragger::childFinishCB, this);
  child->removeOtherEventCallback(SoDragger::childOtherEventCB, this);
  child->removeValueChangedCallback(SoDragger::childValueChangedCB, this);
}

/*!
  Returns a matrix that converts from local to world space.
*/
SbMatrix
SoDragger::getLocalToWorldMatrix(void)
{
  assert(this->draggerCache);
  SbMatrix m = this->draggerCache->draggerToWorld;
  m.multLeft(this->getMotionMatrix());
  return m;
}


/*!
  Returns a matrix that converts from world to local space.
*/
SbMatrix
SoDragger::getWorldToLocalMatrix(void)
{
  // FIXME: cache the inverse motion matrix
  assert(this->draggerCache);
  SbMatrix m = this->draggerCache->worldToDragger;
  m.multRight(this->getMotionMatrix().inverse());
  return m;
}

/*!
  Returns the drag starting point in the local coordinate system.
*/
SbVec3f
SoDragger::getLocalStartingPoint(void)
{
  SbVec3f res;
  this->getWorldToLocalMatrix().multVecMatrix(this->startingPoint, res);
  return res;
}

/*!
  Returns the drag starting point in the world coordinate system.
*/
SbVec3f
SoDragger::getWorldStartingPoint(void)
{
  return this->startingPoint;
}

/*!
  Returns matrices that will convert between local space and the space in
  which \a partname lies in.
*/
void
SoDragger::getPartToLocalMatrix(const SbName & partname, SbMatrix & parttolocalmatrix, SbMatrix & localtopartmatrix)
{
  SoPath *path = (SoPath*)this->createPathToAnyPart(partname, FALSE, FALSE, FALSE, NULL);
  assert(path);
  path->ref();
  SoGetMatrixAction action(this->viewport);
  action.apply(path);
  parttolocalmatrix = action.getMatrix();
  localtopartmatrix = action.getInverse();
  path->unref();
}

/*!
  Convenience method that transforms the local \a frommatrix to a world
  coordinate systems matrix.
*/
void
SoDragger::transformMatrixLocalToWorld(const SbMatrix & frommatrix, SbMatrix & tomatrix)
{
  if (&tomatrix != &frommatrix) tomatrix = frommatrix;
  tomatrix.multRight(this->getLocalToWorldMatrix());
  tomatrix.multLeft(this->getWorldToLocalMatrix());
}

/*!
  Convenience method that transforms the world \a frommatrix to a local
  coordinate systems matrix.
*/
void
SoDragger::transformMatrixWorldToLocal(const SbMatrix & frommatrix, SbMatrix & tomatrix)
{
  if (&tomatrix != &frommatrix) tomatrix = frommatrix;
  tomatrix.multRight(this->getWorldToLocalMatrix());
  tomatrix.multLeft(this->getLocalToWorldMatrix());
}

/*!
  Transforms a matrix that lies in the \a frompartname coordinate system into
  the local coordinate system.
*/
void
SoDragger::transformMatrixToLocalSpace(const SbMatrix & frommatrix, SbMatrix & tomatrix, const SbName & fromspacepartname)
{
  if (&tomatrix != &frommatrix) tomatrix = frommatrix;
  SbMatrix parttolocal, localtopart;
  this->getPartToLocalMatrix(fromspacepartname, parttolocal, localtopart);
  tomatrix.multRight(parttolocal);
  tomatrix.multLeft(localtopart);
}

/*!
  Sets the motion matrix. Triggers value changes callbacks if
  \a matrix != oldmatrix.
*/
void
SoDragger::setMotionMatrix(const SbMatrix & matrix)
{
  SoMatrixTransform *node = SO_GET_ANY_PART(this, "motionMatrix", SoMatrixTransform);
  if (matrix != node->matrix.getValue()) {
    node->matrix = matrix;
    this->valueChanged();
  }
}

/*!
  Can be called by subclasses to trigger value changed callbacks. This might
  be needed if a field is changed without changing the motion matrix.
*/
void
SoDragger::valueChanged(void)
{
  if (this->valueChangedCBEnabled) {
    this->valueChangedCB.invokeCallbacks(this);
  }
}

/*!
  Returns the motion matrix as it was when saveStartParameters() was called.
*/
const SbMatrix &
SoDragger::getStartMotionMatrix(void)
{
  return this->startMotionMatrix;
}

/*!
  Is called to save start parameters to enable draggers to calculate
  relative motion. Default method just saves the motion matrix, but
  subclasses should overload this method if other data needs to be
  saved.
*/
void
SoDragger::saveStartParameters(void)
{
  this->startMotionMatrix = this->getMotionMatrix();
}

/*!
  Returns the picked path.
*/
const SoPath *
SoDragger::getPickPath(void) const
{
  return this->pickedPath;
}

/*!
  Returns the current event.
*/
const SoEvent *
SoDragger::getEvent(void) const
{
  return this->currentEvent;
}

/*!
  Creates a new path to this dragger. Don't forget to ref() and unref() since this
  method creates a fresh copy for you.
*/
SoPath *
SoDragger::createPathToThis(void)
{
  if (this->draggerCache == NULL) return NULL; // should not happen
  SoPath *orgpath = (SoPath*)&this->draggerCache->path;
  return new SoPath(*orgpath);
}

/*!
  Returns the path to the SoInteractionKit that holds the current surrogate
  path.
*/
const SoPath *
SoDragger::getSurrogatePartPickedOwner(void) const
{
  COIN_STUB();
  return NULL;
}

/*!
  Returns the name of the path in the SoInteractionKit that holds the current
  surrogate path.
*/
const SbName &
SoDragger::getSurrogatePartPickedName(void) const
{
  COIN_STUB();
  static SbName n;
  return n;
}

/*!
  Returns the current surrogate path.
*/
const SoPath *
SoDragger::getSurrogatePartPickedPath(void) const
{
  COIN_STUB();
  return NULL;
}

/*!
  Sets the staring point for the drag. \a point is usually a
  picked point from a SoRayPickAction.
*/
void
SoDragger::setStartingPoint(const SoPickedPoint *point)
{
  this->startingPoint = point->getPoint();
}

/*!
  Sets the starting point for a drag.
*/
void
SoDragger::setStartingPoint(const SbVec3f &point)
{
  this->startingPoint = point;
}

/*!
  Return the current view volume.
*/
const SbViewVolume &
SoDragger::getViewVolume(void)
{
  return this->viewVolume;
}

/*!
  Sets the current view volume.
*/
void
SoDragger::setViewVolume(const SbViewVolume & vv)
{
  this->viewVolume = vv;
}

/*!
  Returns the current viewport region.
*/
const SbViewportRegion &
SoDragger::getViewportRegion(void)
{
  return this->viewport;
}

/*!
  Sets the current viewport region.
*/
void
SoDragger::setViewportRegion(const SbViewportRegion & vp)
{
  this->viewport = vp;
}

/*!
  Return the current (most recent) SoHandleEventAction.
*/
SoHandleEventAction *
SoDragger::getHandleEventAction(void) const
{
  return this->eventAction;
}

/*!
  Stores a handle event action.
*/
void
SoDragger::setHandleEventAction(SoHandleEventAction * action)
{
  this->eventAction = action;
}

/*!
  Unimplemented. Should probably have been private in OIV.
*/
void
SoDragger::setTempPathToThis(const SoPath *)
{
  COIN_STUB();
}

/*!
  Called when dragger starts grabbing events (mouse button down).
  Overload if toy need to do something extra in your dragger.
  \sa grabEventCleanup()
*/
void
SoDragger::grabEventsSetup(void)
{
  assert(this->eventAction);
  this->eventAction->setGrabber(this);

  this->updateDraggerCache(this->eventAction->getCurPath());
}

/*!
  Called when dragger stops grabbing events (mouse button up).
  \sa grabEventSetup()
*/
void
SoDragger::grabEventsCleanup(void)
{
  assert(this->eventAction);
  this->eventAction->releaseGrabber();
}

/*!
  Examines the fields of the dragger, changes the matrix according
  to those fields and leaves the rest of the matrix as it was.
  The following field names  are supported: translation, scaleFactor,
  rotation and scaleOrientation.

  Not implemented.
*/
void
SoDragger::workFieldsIntoTransform(SbMatrix & matrix)
{
  SoSFVec3f *vecfield;
  SoSFRotation *rotfield;
  const SbVec3f *translation = NULL;
  const SbVec3f *scaleFactor = NULL;
  const SbRotation *rotation = NULL;
  const SbRotation *scaleOrientation = NULL;
  const SbVec3f *center = NULL;

  vecfield = (SoSFVec3f*)this->getField("translation");
  if (vecfield) translation = &vecfield->getValue();

  vecfield = (SoSFVec3f*)this->getField("scaleFactor");
  if (vecfield) scaleFactor = &vecfield->getValue();

  vecfield = (SoSFVec3f*)this->getField("center");
  if (vecfield) center = &vecfield->getValue();

  rotfield = (SoSFRotation*)this->getField("rotation");
  if (rotfield) rotation = &rotfield->getValue();

  rotfield = (SoSFRotation*)this->getField("scaleOrientation");
  if (rotfield) scaleOrientation = &rotfield->getValue();

  this->workValuesIntoTransform(matrix, translation, rotation,
                                scaleFactor, scaleOrientation, center);
}

/*!
  Controls the behaviour of the SbProjector.
*/
void
SoDragger::setFrontOnProjector(ProjectorFrontSetting val)
{
  this->frontOnProjector = val;
}

/*!
  Returns the behaviour of the SbProjector.
*/
SoDragger::ProjectorFrontSetting
SoDragger::getFrontOnProjector(void) const
{
  return this->frontOnProjector;
}

/*!
  Sets the minimum scale value all scale factors are clamped against.
  This is used in workFieldsIntoTransform(). The default value is 0.01
*/
void
SoDragger::setMinScale(float minscale)
{
  SoDragger::minScale = minscale;
}

/*!
  Returns the minimum scale value.
  \sa setMinScale()
*/
float
SoDragger::getMinScale(void)
{
  return SoDragger::minScale;
}

/*!
  Same as above, but pointers to values are supplied. If a pointer is NULL, the
  matrix value for that argument is used when reconstructing the matrix.

  Not implemented.
*/
void
SoDragger::workValuesIntoTransform(SbMatrix & matrix, const SbVec3f * translationptr, const SbRotation * rotationptr, const SbVec3f * scalefactorptr, const SbRotation * scaleorientationptr, const SbVec3f * centerptr)
{
  SbVec3f t, s;
  SbRotation r, so;
  if (centerptr) matrix.getTransform(t, r, s, so, *centerptr);
  else matrix.getTransform(t, r, s, so);

  if (translationptr) t = *translationptr;
  if (rotationptr) r = *rotationptr;
  if (scalefactorptr) s = *scalefactorptr;
  if (scaleorientationptr) so = *scaleorientationptr;

  if (centerptr) matrix.setTransform(t, r, s, so, *centerptr);
  else matrix.setTransform(t, r, s, so);
}

/*!
  Use when there is no scaleorientation. Faster than workFieldsIntoTransform().
*/
void
SoDragger::getTransformFast(SbMatrix & matrix, SbVec3f & translation, SbRotation & rotation, SbVec3f & scalefactor, SbRotation & scaleorientation, const SbVec3f & center)
{
  COIN_STUB();
}

/*!
  \overload
*/
void
SoDragger::getTransformFast(SbMatrix & matrix, SbVec3f & translation, SbRotation & rotation, SbVec3f & scalefactor, SbRotation & scaleorientation)
{
  COIN_STUB();
}

/*!
  Returns \a matrix after \a translation has been appended.
  If \a conversion != NULL it is used to transform \a translation into
  the space \matrix is defined.
*/
SbMatrix
SoDragger::appendTranslation(const SbMatrix & matrix, const SbVec3f & translation, const SbMatrix * conversion)
{
  SbMatrix transform;
  transform.setTranslate(translation);
  if (conversion) {
    transform.multRight(*conversion);
    transform.multLeft(conversion->inverse());
  }
  SbMatrix res = matrix;
  return res.multLeft(transform);
}

/*!
  Returns \a matrix after \a scale and \a scalecenter has been appended.
  If \a conversion != NULL it is used to transform scale into
  the space \matrix is defined.
*/

SbMatrix
SoDragger::appendScale(const SbMatrix & matrix, const SbVec3f & scale, const SbVec3f & scalecenter, const SbMatrix * conversion)
{
  SbVec3f clampedscale;
  // The explicit casts are done to humour the HPUX aCC compiler,
  // which will otherwise say ``Template deduction failed to find a
  // match for the call to 'SbMax'''. mortene.
  clampedscale[0] = SbMax((float)scale[0], SoDragger::minScale);
  clampedscale[1] = SbMax((float)scale[1], SoDragger::minScale);
  clampedscale[2] = SbMax((float)scale[2], SoDragger::minScale);

  SbMatrix transform, tmp;
  transform.setTranslate(-scalecenter);
  tmp.setScale(clampedscale);
  transform.multRight(tmp);
  tmp.setTranslate(scalecenter);
  transform.multRight(tmp);

  if (conversion) {
    transform.multRight(*conversion);
    transform.multLeft(conversion->inverse());
  }
  SbMatrix res = matrix;
  return res.multLeft(transform);
}

SbMatrix
SoDragger::appendRotation(const SbMatrix & matrix, const SbRotation & rot, const SbVec3f & rotcenter, const SbMatrix * conversion)
{
  SbMatrix transform, tmp;
  transform.setTranslate(-rotcenter);
  tmp.setRotate(rot);
  transform.multRight(tmp);
  tmp.setTranslate(rotcenter);
  transform.multRight(tmp);
  if (conversion) {
    transform.multRight(*conversion);
    transform.multLeft(conversion->inverse());
  }
#if 1 // this code should be used
  SbMatrix res = matrix;
  return res.multLeft(transform);
#else // just testing something, ignore (pederb, 20000225)
  SbRotation r, so;
  SbVec3f t, s;
  matrix.getTransform(t, r, s, so);
  SbMatrix rotmatrix;
  r.getValue(rotmatrix);
  rotmatrix.multLeft(transform);

  SbMatrix res;
  res.setRotate(so.inverse());

  tmp.setScale(s);
  res.multRight(tmp);

  tmp.setRotate(so);
  res.multRight(tmp);

  res.multRight(rotmatrix);

  tmp.setTranslate(t);
  res.multRight(tmp);
  return res;
#endif // test code
}

/*!
  Returns the position of the locater
*/
SbVec2f
SoDragger::getNormalizedLocaterPosition(void)
{
  if (this->currentEvent) {
    return this->currentEvent->getNormalizedPosition(this->viewport);
  }
#if COIN_DEBUG && 1 // debug
  SoDebugError::postInfo("SoDragger::getLocaterPosition",
                         "current event is not set");
#endif // debug
  return SbVec2f(0,0);
}

/*!
  \overload
*/
SbVec2s
SoDragger::getLocaterPosition(void)
{
  if (this->currentEvent) {
    return this->currentEvent->getPosition();
  }
#if COIN_DEBUG && 1 // debug
  SoDebugError::postInfo("SoDragger::getLocaterPosition",
                         "current event is not set");
#endif // debug
  return SbVec2s(0,0);
}

/*!
  Returns the position when mouse button 1 was pressed.
*/
SbVec2s
SoDragger::getStartLocaterPosition(void) const
{
  return this->startLocaterPos;
}

/*!
  The start locater position is automatically set when mouse button 1 goes
  down, but subclasses can use this method to reset the value.
*/
void
SoDragger::setStartLocaterPosition(SbVec2s pos)
{
  this->startLocaterPos = pos;
}

/*!
  FIXME: doc
*/
SbBool
SoDragger::isAdequateConstraintMotion(void)
{
  SbVec2s delta =
    this->getStartLocaterPosition() -
    this->getLocaterPosition();

  // The cast is done to avoid ambigouity error from HPUX aCC, as
  // sqrt() can be either "long double sqrt(long double)" or "float
  // sqrt(float)". mortene.
  double len = sqrt(double(delta[0]*delta[0] + delta[1]*delta[1]));

  if (len >= (double) this->minGesture) return TRUE;
  return FALSE;
}

/*!
  FIXME: doc
*/
SbBool
SoDragger::shouldGrabBasedOnSurrogate(const SoPath * pickpath, const SoPath * surrogatepath)
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: doc
*/
void
SoDragger::setCameraInfo(SoAction * action)
{
  SoState *state = action->getState();
  this->viewVolume = SoViewVolumeElement::get(state);
  this->viewport = SoViewportRegionElement::get(state);;
}

/*!
 */
void
SoDragger::handleEvent(SoHandleEventAction * action)
{
  const SoEvent *event = action->getEvent();

  //
  // this is a special case, to be able to detect when somebody
  // clicks ctrl over a dragger. This has a special meaning for
  // some draggers, and it's the only time the otherEvent callbacks
  // are called when the dragger is not active.
  //
  if (!this->isActive.getValue() &&
      (SO_KEY_PRESS_EVENT(event, LEFT_CONTROL) ||
       SO_KEY_PRESS_EVENT(event, RIGHT_CONTROL))) {
    const SoPickedPoint *pp = action->getPickedPoint();
    if (pp && this->isPicked(pp->getPath())) {
      this->eventHandled(event, action);
      this->otherEventCB.invokeCallbacks(this);
    }
  }
  else if (SO_MOUSE_PRESS_EVENT(event, BUTTON1)) {
    const SoPickedPoint *pp = action->getPickedPoint();

    if (pp && this->isPicked(pp->getPath())) {
      this->isActive = TRUE;
      this->setCameraInfo(action);
      this->setStartingPoint(pp);
      this->eventHandled(event, action);
      if (this->pickedPath) this->pickedPath->unref();
      this->pickedPath = pp->getPath();
      this->pickedPath->ref();

      this->startLocaterPos = event->getPosition();
      this->grabEventsSetup();
      this->saveStartParameters();
      this->startCB.invokeCallbacks(this);
    }
  }
  else if (this->isActive.getValue() && SO_MOUSE_RELEASE_EVENT(event, BUTTON1)) {
    this->isActive = FALSE;
    this->eventHandled(event, action);
    this->grabEventsCleanup();
    if (this->pickedPath) {
      this->pickedPath->unref();
      this->pickedPath = NULL;
    }
    this->finishCB.invokeCallbacks(this);
  }
  else if (this->isActive.getValue() && event->isOfType(SoLocation2Event::getClassTypeId())) {
    this->eventHandled(event, action);
    this->updateDraggerCache(NULL);
    this->motionCB.invokeCallbacks(this);
  }
  else if (this->isActive.getValue()) {
    this->eventAction = action;
    this->currentEvent = event;
    this->otherEventCB.invokeCallbacks(this);
  }

  if (!action->isHandled())
    inherited::handleEvent(action);
}

/*!
 */
void
SoDragger::transferMotion(SoDragger * child)
{
  // FIXME: maybe I should use the picked path to get partname instead?
  SbString partname = this->getPartString(child);
  SbMatrix childmatrix = child->getMotionMatrix();
  SbMatrix parttolocal, localtopart;

  this->transformMatrixToLocalSpace(childmatrix, childmatrix, SbName(partname.getString()));
  SbMatrix mat = this->startMotionMatrix;
  mat.multRight(childmatrix);
  this->setMotionMatrix(mat);
  child->setMotionMatrix(SbMatrix::identity());
}

/*!
  Sets whether dragger geometry should be ignored when calculating bbox.
*/
void
SoDragger::setIgnoreInBbox(SbBool val)
{
  this->ignoreInBBox = val;
}

/*!
  Returns whether dragger geometry should be ignored when calculating bbox.
*/
SbBool
SoDragger::isIgnoreInBbox(void)
{
  return this->ignoreInBBox;
}

void
SoDragger::getBoundingBox(SoGetBoundingBoxAction * action)
{
  if (!this->ignoreInBBox) inherited::getBoundingBox(action);

}

/*!
  FIXME: doc
*/
void
SoDragger::setActiveChildDragger(SoDragger * childdragger)
{
  this->activeChildDragger = childdragger;
}

SoDragger *
SoDragger::getActiveChildDragger(void) const
{
  return this->activeChildDragger;
}

void
SoDragger::setDefaultOnNonWritingFields(void)
{
  // check standard fields
  inherited::setDefaultOnNonWritingFields();
}

void
SoDragger::childTransferMotionAndValueChangedCB(void *data, SoDragger *child)
{
  SoDragger *thisp = (SoDragger*)data;
  child->removeValueChangedCallback(SoDragger::childTransferMotionAndValueChangedCB, thisp);
  thisp->transferMotion(child);
  child->addValueChangedCallback(SoDragger::childTransferMotionAndValueChangedCB, thisp);
}


void
SoDragger::childValueChangedCB(void *data, SoDragger *child)
{
  SoDragger *thisp = (SoDragger*)data;
  thisp->valueChanged();
}

void
SoDragger::childStartCB(void *data, SoDragger *child)
{
  SoDragger *thisp = (SoDragger*)data;
  thisp->saveStartParameters();
  thisp->setActiveChildDragger(child);
  thisp->startCB.invokeCallbacks(thisp);
}

void
SoDragger::childMotionCB(void *data, SoDragger *child)
{
  SoDragger *thisp = (SoDragger*)data;
  thisp->motionCB.invokeCallbacks(thisp);
}

void
SoDragger::childFinishCB(void *data, SoDragger *child)
{
  SoDragger *thisp = (SoDragger*)data;
  thisp->finishCB.invokeCallbacks(thisp);
  thisp->setActiveChildDragger(NULL);
}

void
SoDragger::childOtherEventCB(void *data, SoDragger *child)
{
  SoDragger *thisp = (SoDragger*)data;
  thisp->currentEvent = child->currentEvent;
  thisp->eventAction = child->eventAction;
  thisp->otherEventCB.invokeCallbacks(thisp);
}

//
// returns whether path goes through this node (dragger is picked)
//
SbBool
SoDragger::isPicked(SoPath *path)
{
  // last dragger in path must be this one
  SoFullPath *fullpath = (SoFullPath*)path;

  int i = fullpath->findNode(this);
  if (i < 0) return FALSE;

  int n = fullpath->getLength();
  for (++i; i < n; i++) {
    SoNode *node = fullpath->getNode(i);
    if (node->isOfType(SoDragger::getClassTypeId())) return FALSE;
  }
  return TRUE;
}

void
SoDragger::eventHandled(const SoEvent *event, SoHandleEventAction *action)
{
  action->setHandled();
  this->currentEvent = event;
  this->eventAction = action;
}

void
SoDragger::updateDraggerCache(const SoPath *path)
{
  if (this->draggerCache == NULL) this->draggerCache = new SoDraggerCache(this);
  if (path) this->draggerCache->checkUpdate((const SoFullPath*)path, path->findNode(this));
  else this->draggerCache->updateMatrix();
}
