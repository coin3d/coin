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

#include <Inventor/draggers/SoDragger.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbViewVolume.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/nodes/SoMatrixTransform.h>


SO_KIT_SOURCE(SoDragger);


SoDragger::SoDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoDragger);

  SO_KIT_ADD_CATALOG_ENTRY(motionMatrix, SoMatrixTransform, TRUE, topSeparator, geomSeparator, FALSE);

  SO_KIT_INIT_INSTANCE();
}

SoDragger::~SoDragger()
{
  COIN_STUB();
}

void
SoDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoDragger);
}

void
SoDragger::initClasses(void)
{
  COIN_STUB();
}

void
SoDragger::addStartCallback(SoDraggerCB * func, void * data)
{
  COIN_STUB();
}

void
SoDragger::removeStartCallback(SoDraggerCB * func, void * data)
{
  COIN_STUB();
}

void
SoDragger::addMotionCallback(SoDraggerCB * func, void * data)
{
  COIN_STUB();
}

void
SoDragger::removeMotionCallback(SoDraggerCB * func, void * data)
{
  COIN_STUB();
}

void
SoDragger::addFinishCallback(SoDraggerCB * func, void * data)
{
  COIN_STUB();
}

void
SoDragger::removeFinishCallback(SoDraggerCB * func, void * data)
{
  COIN_STUB();
}

void
SoDragger::addValueChangedCallback(SoDraggerCB * func, void * data)
{
  COIN_STUB();
}

void
SoDragger::removeValueChangedCallback(SoDraggerCB * func, void * data)
{
  COIN_STUB();
}

void
SoDragger::setMinGesture(int pixels)
{
  COIN_STUB();
}

int
SoDragger::getMinGesture(void) const
{
  COIN_STUB();
  return 0;
}

SbBool
SoDragger::enableValueChangedCallbacks(SbBool newval)
{
  COIN_STUB();
  return FALSE;
}

const SbMatrix &
SoDragger::getMotionMatrix(void)
{
  COIN_STUB();
  static SbMatrix m;
  return m;
}

void
SoDragger::addOtherEventCallback(SoDraggerCB * func, void * data)
{
  COIN_STUB();
}

void
SoDragger::removeOtherEventCallback(SoDraggerCB * func, void * data)
{
  COIN_STUB();
}

void
SoDragger::registerChildDragger(SoDragger * child)
{
  COIN_STUB();
}

void
SoDragger::unregisterChildDragger(SoDragger * child)
{
  COIN_STUB();
}

void
SoDragger::registerChildDraggerMovingIndependently(SoDragger * child)
{
  COIN_STUB();
}

void
SoDragger::unregisterChildDraggerMovingIndependently(SoDragger * child)
{
  COIN_STUB();
}

SbMatrix
SoDragger::getLocalToWorldMatrix(void)
{
  COIN_STUB();
  return SbMatrix::identity();
}

SbMatrix
SoDragger::getWorldToLocalMatrix(void)
{
  COIN_STUB();
  return SbMatrix::identity();
}

SbVec3f
SoDragger::getLocalStartingPoint(void)
{
  COIN_STUB();
  return SbVec3f();
}

SbVec3f
SoDragger::getWorldStartingPoint(void)
{
  COIN_STUB();
  return SbVec3f();
}

void
SoDragger::getPartToLocalMatrix(const SbName & partname, SbMatrix & parttolocalmatrix, SbMatrix & localtopartmatrix)
{
  COIN_STUB();
}

void
SoDragger::transformMatrixLocalToWorld(const SbMatrix & frommatrix, SbMatrix & tomatrix)
{
  COIN_STUB();
}

void
SoDragger::transformMatrixWorldToLocal(const SbMatrix & frommatrix, SbMatrix & tomatrix)
{
  COIN_STUB();
}

void
SoDragger::transformMatrixToLocalSpace(const SbMatrix & frommatrix, SbMatrix & tomatrix, const SbName & fromspacepartname)
{
  COIN_STUB();
}

void
SoDragger::setMotionMatrix(const SbMatrix & newmatrix)
{
  COIN_STUB();
}

void
SoDragger::valueChanged(void)
{
  COIN_STUB();
}

const SbMatrix &
SoDragger::getStartMotionMatrix(void)
{
  COIN_STUB();
  static SbMatrix m;
  return m;
}

void
SoDragger::saveStartParameters(void)
{
  COIN_STUB();
}

const SoPath *
SoDragger::getPickPath(void) const
{
  COIN_STUB();
  return NULL;
}

const SoEvent *
SoDragger::getEvent(void) const
{
  COIN_STUB();
  return NULL;
}

SoPath *
SoDragger::createPathToThis(void)
{
  COIN_STUB();
  return NULL;
}

const SoPath *
SoDragger::getSurrogatePartPickedOwner(void) const
{
  COIN_STUB();
  return NULL;
}

const SbName &
SoDragger::getSurrogatePartPickedName(void) const
{
  COIN_STUB();
  static SbName n;
  return n;
}

const SoPath *
SoDragger::getSurrogatePartPickedPath(void) const
{
  COIN_STUB();
  return NULL;
}

void
SoDragger::setStartingPoint(const SoPickedPoint * newpoint)
{
  COIN_STUB();
}

void
SoDragger::setStartingPoint(const SbVec3f & newpoint)
{
  COIN_STUB();
}

const SbViewVolume &
SoDragger::getViewVolume(void)
{
  COIN_STUB();
  static SbViewVolume vv;
  return vv;
}

void
SoDragger::setViewVolume(const SbViewVolume & vol)
{
  COIN_STUB();
}

const SbViewportRegion &
SoDragger::getViewportRegion(void)
{
  COIN_STUB();
  static SbViewportRegion vpr;
  return vpr;
}

void
SoDragger::setViewportRegion(const SbViewportRegion & reg)
{
  COIN_STUB();
}

SoHandleEventAction *
SoDragger::getHandleEventAction(void) const
{
  COIN_STUB();
  return NULL;
}

void
SoDragger::setHandleEventAction(SoHandleEventAction * newAction)
{
  COIN_STUB();
}

void
SoDragger::setTempPathToThis(const SoPath * somethingclose)
{
  COIN_STUB();
}

void
SoDragger::grabEventsSetup(void)
{
  COIN_STUB();
}

void
SoDragger::grabEventsCleanup(void)
{
  COIN_STUB();
}

void
SoDragger::workFieldsIntoTransform(SbMatrix & mtx)
{
  COIN_STUB();
}

void
SoDragger::setFrontOnProjector(ProjectorFrontSetting newval)
{
  COIN_STUB();
}

SoDragger::ProjectorFrontSetting
SoDragger::getFrontOnProjector(void) const
{
  COIN_STUB();
  return FRONT;
}

void
SoDragger::setMinScale(float newminscale)
{
  COIN_STUB();
}

float
SoDragger::getMinScale(void)
{
  COIN_STUB();
  return -1.0f;
}

void
SoDragger::workValuesIntoTransform(SbMatrix & mtx, const SbVec3f * translationptr, const SbRotation * rotationptr, const SbVec3f * scalefactorptr, const SbRotation * scaleorientationptr, const SbVec3f * centerptr)
{
  COIN_STUB();
}

void
SoDragger::getTransformFast(SbMatrix & mtx, SbVec3f & translation, SbRotation & rotation, SbVec3f & scalefactor, SbRotation & scaleorientation, const SbVec3f & center)
{
  COIN_STUB();
}

void
SoDragger::getTransformFast(SbMatrix & mtx, SbVec3f & translation, SbRotation & rotation, SbVec3f & scalefactor, SbRotation & scaleorientation)
{
  COIN_STUB();
}

SbMatrix
SoDragger::appendTranslation(const SbMatrix & mtx, const SbVec3f & translation, const SbMatrix * conversion)
{
  COIN_STUB();
  return SbMatrix();
}

SbMatrix
SoDragger::appendScale(const SbMatrix & mtx, const SbVec3f & scale, const SbVec3f & scalecenter, const SbMatrix * conversion)
{
  COIN_STUB();
  return SbMatrix();
}

SbMatrix
SoDragger::appendRotation(const SbMatrix & mtx, const SbRotation & rot, const SbVec3f & rotcenter, const SbMatrix * conversion)
{
  COIN_STUB();
  return SbMatrix();
}

SbVec2f
SoDragger::getNormalizedLocaterPosition(void)
{
  COIN_STUB();
  return SbVec2f();
}

SbVec2s
SoDragger::getLocaterPosition(void)
{
  COIN_STUB();
  return SbVec2s();
}

SbVec2s
SoDragger::getStartLocaterPosition(void) const
{
  COIN_STUB();
  return SbVec2s();
}

void
SoDragger::setStartLocaterPosition(SbVec2s p)
{
  COIN_STUB();
}

SbBool
SoDragger::isAdequateConstraintMotion(void)
{
  COIN_STUB();
  return FALSE;
}

SbBool
SoDragger::shouldGrabBasedOnSurrogate(const SoPath * pickpath, const SoPath * surrogatepath)
{
  COIN_STUB();
  return FALSE;
}

void
SoDragger::setCameraInfo(SoAction * action)
{
  COIN_STUB();
}

void
SoDragger::handleEvent(SoHandleEventAction * ha)
{
  COIN_STUB();
}

void
SoDragger::transferMotion(SoDragger * child)
{
  COIN_STUB();
}

void
SoDragger::setIgnoreInBbox(SbBool newval)
{
  COIN_STUB();
}

SbBool
SoDragger::isIgnoreInBbox(void)
{
  COIN_STUB();
  return FALSE;
}

void
SoDragger::getBoundingBox(SoGetBoundingBoxAction * action)
{
  COIN_STUB();
}

void
SoDragger::setActiveChildDragger(SoDragger * newchilddragger)
{
  COIN_STUB();
}

SoDragger *
SoDragger::getActiveChildDragger(void) const
{
  COIN_STUB();
  return NULL;
}

void
SoDragger::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
}

void
SoDragger::childTransferMotionAndValueChangedCB(void *, SoDragger *)
{
  COIN_STUB();
}

void
SoDragger::childValueChangedCB(void *, SoDragger *)
{
  COIN_STUB();
}

void
SoDragger::childStartCB(void *, SoDragger *)
{
  COIN_STUB();
}

void
SoDragger::childMotionCB(void *, SoDragger *)
{
  COIN_STUB();
}

void
SoDragger::childFinishCB(void *, SoDragger *)
{
  COIN_STUB();
}

void
SoDragger::childOtherEventCB(void *, SoDragger *)
{
  COIN_STUB();
}
