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
  \class SoTabPlaneDragger SoTabPlaneDragger.h Inventor/draggers/SoTabPlaneDragger.h
  \brief The SoTabPlaneDragger class is a dragger you can translate and scale within a plane.
  \ingroup draggers

  \DRAGGER_DEFAULT_SCREENSHOT

  <center>
  <img src="http://doc.coin3d.org/images/Coin/draggers/tabplane.png">
  </center>

  For translation, click anywhere inside the dragger's plane and move
  it about in 2D.

  For non-uniform scaling operations, click and drag any of the 4 side
  tabs. For uniform scaling, click and drag any of the 4 corner tabs.

  \sa SoTabBoxDragger
*/


#include <Inventor/draggers/SoTabPlaneDragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/projectors/SbPlaneProjector.h>
#include <Inventor/projectors/SbLineProjector.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/SbRotation.h>
#include <coindefs.h> // COIN_STUB()
#include <assert.h>

#include <data/draggerDefaults/tabPlaneDragger.h>


/*!
  \var SoSFVec3f SoTabPlaneDragger::translation

  Continuously updated to contain the current translation from the
  dragger's local origo position.
*/

/*!
  \var SoSFVec3f SoTabPlaneDragger::scaleFactor

  Continuously updated to contain the current vector of scaling along
  the X, Y and Z axes. The Z component will always be 1.0.
*/

#define WHATKIND_NONE      0
#define WHATKIND_SCALE     1
#define WHATKIND_TRANSLATE 2

#define CONSTRAINT_OFF  0
#define CONSTRAINT_WAIT 1
#define CONSTRAINT_X    2
#define CONSTRAINT_Y    3
#define CONSTRAINT_Z    4


// used to quickly find correct position of tabs

static float edgetab_lookup[] = {
  0.0f, 1.0f,
  1.0f, 0.0f,
  0.0f, -1.0f,
  -1.0f, 0.0f
};

static float cornertab_lookup[] = {
  1.0f, 1.0f,
  1.0f, -1.0f,
  -1.0f, -1.0f,
  -1.0f, 1.0f
};

// FIXME: find a better solution than this lame Z_OFFSET hack, pederb 20000301
#define Z_OFFSET 0.01        // dummy offset for tabs to get "correct" picking
#define TABSIZE 10.0f        // size (in pixels when projected to screen) of tabs

SO_KIT_SOURCE(SoTabPlaneDragger);

// doc in superclass
void
SoTabPlaneDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoTabPlaneDragger, SO_FROM_INVENTOR_1);
}

// FIXME: document which parts need to be present in the geometry
// scenegraph, and what role they play in the dragger. 20010913 mortene.
/*!
  \DRAGGER_CONSTRUCTOR

  \NODEKIT_PRE_DIAGRAM

  \verbatim
  CLASS SoTabPlaneDragger
  -->"this"
        "callbackList"
        "topSeparator"
           "motionMatrix"
           "geomSeparator"
  -->         "planeSwitch"
  -->            "translator"
  -->            "scaleTabs"
  -->               "scaleTabMaterial"
  -->               "scaleTabHints"
  -->               "scaleTabMaterialBinding"
  -->               "scaleTabNormalBinding"
  -->               "scaleTabNormal"
  -->               "edgeScaleCoords"
  -->               "edgeScaleTab0"
  -->               "edgeScaleTab1"
  -->               "edgeScaleTab2"
  -->               "edgeScaleTab3"
  -->               "cornerScaleCoords"
  -->               "cornerScaleTab0"
  -->               "cornerScaleTab1"
  -->               "cornerScaleTab2"
  -->               "cornerScaleTab3"
  \endverbatim

  \NODEKIT_POST_DIAGRAM


  \NODEKIT_PRE_TABLE

  \verbatim
  CLASS SoTabPlaneDragger
  PVT   "this",  SoTabPlaneDragger  --- 
        "callbackList",  SoNodeKitListPart [ SoCallback, SoEventCallback ] 
  PVT   "topSeparator",  SoSeparator  --- 
  PVT   "motionMatrix",  SoMatrixTransform  --- 
  PVT   "geomSeparator",  SoSeparator  --- 
  PVT   "planeSwitch",  SoSwitch  --- 
        "translator",  SoSeparator  --- 
  PVT   "scaleTabs",  SoSeparator  --- 
        "scaleTabMaterial",  SoMaterial  --- 
        "scaleTabHints",  SoShapeHints  --- 
  PVT   "scaleTabMaterialBinding",  SoMaterialBinding  --- 
  PVT   "scaleTabNormalBinding",  SoNormalBinding  --- 
  PVT   "scaleTabNormal",  SoNormal  --- 
  PVT   "edgeScaleCoords",  SoCoordinate3  --- 
  PVT   "edgeScaleTab0",  SoIndexedFaceSet  --- 
  PVT   "edgeScaleTab1",  SoIndexedFaceSet  --- 
  PVT   "edgeScaleTab2",  SoIndexedFaceSet  --- 
  PVT   "edgeScaleTab3",  SoIndexedFaceSet  --- 
  PVT   "cornerScaleCoords",  SoCoordinate3  --- 
  PVT   "cornerScaleTab0",  SoIndexedFaceSet  --- 
  PVT   "cornerScaleTab1",  SoIndexedFaceSet  --- 
  PVT   "cornerScaleTab2",  SoIndexedFaceSet  --- 
  PVT   "cornerScaleTab3",  SoIndexedFaceSet  --- 
  \endverbatim

  \NODEKIT_POST_TABLE
*/
SoTabPlaneDragger::SoTabPlaneDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoTabPlaneDragger);

  SO_KIT_ADD_CATALOG_ENTRY(planeSwitch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator, SoSeparator, TRUE, planeSwitch, scaleTabs, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scaleTabs, SoSeparator, TRUE, planeSwitch, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scaleTabMaterial, SoMaterial, TRUE, scaleTabs, scaleTabHints, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scaleTabHints, SoShapeHints, TRUE, scaleTabs, scaleTabMaterialBinding, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scaleTabMaterialBinding, SoMaterialBinding, TRUE, scaleTabs, scaleTabNormalBinding, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scaleTabNormalBinding, SoNormalBinding, TRUE, scaleTabs, scaleTabNormal, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scaleTabNormal, SoNormal, TRUE, scaleTabs, edgeScaleCoords, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(edgeScaleCoords, SoCoordinate3, TRUE, scaleTabs, edgeScaleTab0, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(edgeScaleTab0, SoIndexedFaceSet, TRUE, scaleTabs, edgeScaleTab1, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(edgeScaleTab1, SoIndexedFaceSet, TRUE, scaleTabs, edgeScaleTab2, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(edgeScaleTab2, SoIndexedFaceSet, TRUE, scaleTabs, edgeScaleTab3, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(edgeScaleTab3, SoIndexedFaceSet, TRUE, scaleTabs, cornerScaleCoords, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(cornerScaleCoords, SoCoordinate3, TRUE, scaleTabs, cornerScaleTab0, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(cornerScaleTab0, SoIndexedFaceSet, TRUE, scaleTabs, cornerScaleTab1, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(cornerScaleTab1, SoIndexedFaceSet, TRUE, scaleTabs, cornerScaleTab2, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(cornerScaleTab2, SoIndexedFaceSet, TRUE, scaleTabs, cornerScaleTab3, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(cornerScaleTab3, SoIndexedFaceSet, TRUE, scaleTabs, "", FALSE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("tabPlaneDragger.iv",
                                       TABPLANEDRAGGER_draggergeometry,
                                       sizeof(TABPLANEDRAGGER_draggergeometry));
  }

  SO_KIT_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_KIT_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));

  SO_KIT_INIT_INSTANCE();

  this->setPartAsDefault("translator", "tabPlaneTranslator");
  this->setPartAsDefault("scaleTabMaterial", "tabPlaneScaleTabMaterial");
  this->setPartAsDefault("scaleTabHints", "tabPlaneScaleTabHints");

  SoSwitch *sw = SO_GET_ANY_PART(this, "planeSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, SO_SWITCH_ALL);

  this->createPrivateParts();
  this->prevsizex = this->prevsizey = 0.0f;
  this->reallyAdjustScaleTabSize(NULL);
  this->constraintState = CONSTRAINT_OFF;
  this->whatkind = WHATKIND_NONE;
  this->adjustTabs = TRUE;

  this->addStartCallback(SoTabPlaneDragger::startCB);
  this->addMotionCallback(SoTabPlaneDragger::motionCB);
  this->addFinishCallback(SoTabPlaneDragger::finishCB);
  this->addValueChangedCallback(SoTabPlaneDragger::valueChangedCB);
  this->addOtherEventCallback(SoTabPlaneDragger::metaKeyChangeCB);

  this->planeProj = new SbPlaneProjector;
  this->lineProj = new SbLineProjector;

  this->translFieldSensor = new SoFieldSensor(SoTabPlaneDragger::fieldSensorCB, this);
  this->translFieldSensor->setPriority(0);
  this->scaleFieldSensor = new SoFieldSensor(SoTabPlaneDragger::fieldSensorCB, this);
  this->scaleFieldSensor->setPriority(0);

  this->setUpConnections(TRUE, TRUE);
}

/*!
  Protected destructor.

  (Dragger classes are derived from SoBase, so they are reference
  counted and automatically destroyed when their reference count goes
  to 0.)
 */
SoTabPlaneDragger::~SoTabPlaneDragger()
{
  delete this->translFieldSensor;
  delete this->scaleFieldSensor;
  delete this->planeProj;
  delete this->lineProj;
}

// Doc in superclass.
SbBool
SoTabPlaneDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);

    SoTabPlaneDragger::fieldSensorCB(this, NULL);

    if (this->translFieldSensor->getAttachedField() != &this->translation) {
      this->translFieldSensor->attach(&this->translation);
    }
    if (this->scaleFieldSensor->getAttachedField() != &this->scaleFactor) {
      this->scaleFieldSensor->attach(&this->scaleFactor);
    }
  }
  else {
    if (this->translFieldSensor->getAttachedField() != NULL) {
      this->translFieldSensor->detach();
    }
    if (this->scaleFieldSensor->getAttachedField() != NULL) {
      this->scaleFieldSensor->detach();
    }
    inherited::setUpConnections(onoff, doitalways);
  }
  return !(this->connectionsSetUp = onoff);
}

// Doc in superclass.
void
SoTabPlaneDragger::setDefaultOnNonWritingFields(void)
{
  this->edgeScaleCoords.setDefault(TRUE);
  this->cornerScaleCoords.setDefault(TRUE);

  inherited::setDefaultOnNonWritingFields();
}

/*! \COININTERNAL */
void
SoTabPlaneDragger::fieldSensorCB(void * d, SoSensor *)
{
  SoTabPlaneDragger *thisp = (SoTabPlaneDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  thisp->workFieldsIntoTransform(matrix);
  thisp->setMotionMatrix(matrix);
}

/*! \COININTERNAL */
void
SoTabPlaneDragger::valueChangedCB(void *, SoDragger * d)
{
  SoTabPlaneDragger *thisp = (SoTabPlaneDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  SbVec3f trans, scale;
  SbRotation rot, scaleOrient;
  matrix.getTransform(trans, rot, scale, scaleOrient);

  thisp->translFieldSensor->detach();
  if (thisp->translation.getValue() != trans)
    thisp->translation = trans;
  thisp->translFieldSensor->attach(&thisp->translation);

  thisp->scaleFieldSensor->detach();
  if (thisp->scaleFactor.getValue() != scale)
    thisp->scaleFactor = scale;
  thisp->scaleFieldSensor->attach(&thisp->scaleFactor);
}

// Doc in superclass.
void
SoTabPlaneDragger::GLRender(SoGLRenderAction * action)
{
  //
  // I think it is best to always recalculate tabs. In OIV, you'll need to
  // click into the dragger sometimes to make the dragger recalculate tabs
  // (after zooming, for instance).
  //
  if (1 || this->adjustTabs) {
    // disable notification do avoid multiple redraws, but
    // remember to invalidate open caches.
    SbBool oldnotify = this->enableNotify(FALSE);
    SoCacheElement::invalidate(action->getState());
    
    this->reallyAdjustScaleTabSize(action);
    this->adjustTabs = FALSE;
    
    (void) this->enableNotify(oldnotify);
  }
  inherited::GLRender(action);
}

/*!
  Signals the dragger to recalculate the size of its tabs. This method
  is not doing anything useful in Coin, since the tab sizes are recalculated
  every time the dragger is rendered, even though this method has not
  been called.
*/
void
SoTabPlaneDragger::adjustScaleTabSize(void)
{
  this->adjustTabs = TRUE;
}

/*!
  Recalculates the size of the tabs, based on the current view volume,
  the current viewport, the current model matrix and the current scale
  factor. If \a action == \e NULL, a default size will be used.
*/
void
SoTabPlaneDragger::reallyAdjustScaleTabSize(SoGLRenderAction *action)
{
  SoCoordinate3 *coordnode;
  SbVec3f *coords;

  float sizex = 0.08f;
  float sizey = 0.08f;
  if (action != NULL) {
    SoState *state = action->getState();
    SbMatrix toworld = SoModelMatrixElement::get(state);
    toworld.multLeft(this->getMotionMatrix());
    const SbViewVolume &vv = SoViewVolumeElement::get(state);
    const SbViewportRegion &vp = SoViewportRegionElement::get(state);
    SbVec3f center(0.0f, 0.0f, 0.0f);
    toworld.multVecMatrix(center, center);
    sizex = sizey =
      vv.getWorldToScreenScale(center, TABSIZE/float(vp.getViewportSizePixels()[0]));

    SbVec3f scale;
    {
      SbRotation r, so;
      SbVec3f t;
      toworld.getTransform(t, r, scale, so);
    }
    sizex /= scale[0];
    sizey /= scale[1];
  }

  if (sizex == this->prevsizex && this->prevsizey == sizey) return;
 
  this->prevsizex = sizex;
  this->prevsizey = sizey;

  float halfx = sizex * 0.5f;
  float halfy = sizey * 0.5f;

  coordnode = SO_GET_ANY_PART(this, "edgeScaleCoords", SoCoordinate3);
  coordnode->point.setNum(16);
  coords = coordnode->point.startEditing();
  {
    coords[0].setValue(halfx, 1.0f, Z_OFFSET);
    coords[1].setValue(-halfx, 1.0f, Z_OFFSET);
    coords[2].setValue(-halfx, 1.0f-sizey, Z_OFFSET);
    coords[3].setValue(halfx, 1.0f-sizey, Z_OFFSET);

    coords[4].setValue(1.0f, -halfy, Z_OFFSET);
    coords[5].setValue(1.0f, halfy, Z_OFFSET);
    coords[6].setValue(1.0f-sizex, halfy, Z_OFFSET);
    coords[7].setValue(1.0f-sizex, -halfy, Z_OFFSET);

    coords[8].setValue(-halfx, -1.0f, Z_OFFSET);
    coords[9].setValue(halfx, -1.0f, Z_OFFSET);
    coords[10].setValue(halfx, -1.0f+sizey, Z_OFFSET);
    coords[11].setValue(-halfx, -1.0f+sizey, Z_OFFSET);

    coords[12].setValue(-1.0f, halfy, Z_OFFSET);
    coords[13].setValue(-1.0f, -halfy, Z_OFFSET);
    coords[14].setValue(-1.0f+sizex, -halfy, Z_OFFSET);
    coords[15].setValue(-1.0f+sizex, halfy, Z_OFFSET);
  }
  coordnode->point.finishEditing();

  coordnode = SO_GET_ANY_PART(this, "cornerScaleCoords", SoCoordinate3);
  coordnode->point.setNum(16);
  coords = coordnode->point.startEditing();
  {
    coords[0].setValue(1.0f, 1.0f, Z_OFFSET);
    coords[1].setValue(1.0f-sizex, 1.0f, Z_OFFSET);
    coords[2].setValue(1.0f-sizex, 1.0f-sizey, Z_OFFSET);
    coords[3].setValue(1.0f, 1.0f-sizey, Z_OFFSET);

    coords[4].setValue(1.0f, -1.0f, Z_OFFSET);
    coords[5].setValue(1.0f, -1.0f+sizey, Z_OFFSET);
    coords[6].setValue(1.0f-sizex, -1.0f+sizey, Z_OFFSET);
    coords[7].setValue(1.0f-sizex, -1.0f, Z_OFFSET);

    coords[8].setValue(-1.0f, -1.0f, Z_OFFSET);
    coords[9].setValue(-1.0f+sizex, -1.0f, Z_OFFSET);
    coords[10].setValue(-1.0f+sizex, -1.0f+sizey, Z_OFFSET);
    coords[11].setValue(-1.0f, -1.0f+sizey, Z_OFFSET);

    coords[12].setValue(-1.0f, 1.0f, Z_OFFSET);
    coords[13].setValue(-1.0f, 1.0f-sizey, Z_OFFSET);
    coords[14].setValue(-1.0f+sizex, 1.0f-sizey, Z_OFFSET);
    coords[15].setValue(-1.0f+sizex, 1.0f, Z_OFFSET);
  }
  coordnode->point.finishEditing();
}

/*!
  Not implemented.
*/
void
SoTabPlaneDragger::getXYScreenLengths(SbVec2f & lengths,
                                      const SbMatrix & localtoscreen,
                                      const SbVec2s & winsize)
{
  // FIXME: I found this method just defined in the header file, but
  // not implemented (!). We should obviously implement it if it's
  // useful. 20011127 mortene.
  COIN_STUB();
}

/*! \COININTERNAL
  Called when dragger is selected (picked) by the user.
*/
void
SoTabPlaneDragger::dragStart(void)
{
  int i;
  const SoPath *pickpath = this->getPickPath();
  const SoEvent *event = this->getEvent();

  SbBool found = FALSE;
  SbVec3f startpt = this->getLocalStartingPoint();

  this->constraintState = CONSTRAINT_OFF;

  SbString str;
  if (!found) {
    for (i = 0; i < 4; i++) {
      str.sprintf("edgeScaleTab%d", i);
      if (pickpath->findNode(this->getNodeFieldNode(str.getString())) >= 0) break;
    }
    if (i < 4) {
      found = TRUE;
      this->constraintState = i & 1 ? CONSTRAINT_X : CONSTRAINT_Y;
      this->whatkind = WHATKIND_SCALE;
      this->scaleCenter.setValue(-edgetab_lookup[i*2], -edgetab_lookup[i*2+1], 0.0f);
    }
  }
  if (!found) {
    for (i = 0; i < 4; i++) {
      str.sprintf("cornerScaleTab%d", i);
      if (pickpath->findNode(this->getNodeFieldNode(str.getString())) >= 0) break;
    }
    if (i < 4) {
      found = TRUE;
      this->whatkind = WHATKIND_SCALE;
      this->scaleCenter.setValue(-cornertab_lookup[i*2], -cornertab_lookup[i*2+1], 0.0f);
    }
  }
  if (!found) {
    assert(pickpath->findNode(this->getNodeFieldNode("translator")) >= 0);
    found = TRUE;
    this->whatkind = WHATKIND_TRANSLATE;
  }

  if (this->whatkind == WHATKIND_SCALE) {
    this->lineProj->setLine(SbLine(this->scaleCenter, startpt));
  }
  else { // translate
    this->planeProj->setPlane(SbPlane(SbVec3f(0.0f, 0.0f, 1.0f), startpt));
    this->constraintState = CONSTRAINT_OFF;
    if (event->wasShiftDown()) {
      this->getLocalToWorldMatrix().multVecMatrix(startpt, this->worldRestartPt);
      this->constraintState = CONSTRAINT_WAIT;
    }
  }
}

/*! \COININTERNAL
  Called when user drags the mouse after picking the dragger.
*/
void
SoTabPlaneDragger::drag(void)
{
  if (this->whatkind == WHATKIND_SCALE) {
    SbVec3f startpt = this->getLocalStartingPoint();
    this->lineProj->setViewVolume(this->getViewVolume());
    this->lineProj->setWorkingSpace(this->getLocalToWorldMatrix());
    SbVec3f projpt = this->lineProj->project(this->getNormalizedLocaterPosition());

    SbVec3f center = this->scaleCenter;

    float orglen = (startpt-center).length();
    float currlen = (projpt-center).length();
    float scale = 0.0f;

    if (orglen > 0.0f) scale = currlen / orglen;
    if (scale > 0.0f && (startpt-center).dot(projpt-center) <= 0.0f) scale = 0.0f;

    SbVec3f scalevec(scale, scale, 1.0f);
    if (this->constraintState == CONSTRAINT_X) {
      scalevec[1] = 1.0f;
    }
    else if (this->constraintState == CONSTRAINT_Y) {
      scalevec[0] = 1.0f;
    }
    this->setMotionMatrix(this->appendScale(this->getStartMotionMatrix(),
                                            scalevec,
                                            center));

  }
  else { // translate
    SbVec3f startpt = this->getLocalStartingPoint();
    this->planeProj->setViewVolume(this->getViewVolume());
    this->planeProj->setWorkingSpace(this->getLocalToWorldMatrix());
    SbVec3f projpt = this->planeProj->project(this->getNormalizedLocaterPosition());

    const SoEvent *event = this->getEvent();
    SbBool reset = FALSE;
    if (event->wasShiftDown() && this->constraintState == CONSTRAINT_OFF) {
      this->constraintState = CONSTRAINT_WAIT;
      this->setStartLocaterPosition(event->getPosition());
      this->getLocalToWorldMatrix().multVecMatrix(projpt, this->worldRestartPt);
      reset = TRUE;
    }
    else if (!event->wasShiftDown() && this->constraintState != CONSTRAINT_OFF) {
      this->constraintState = CONSTRAINT_OFF;
      reset = TRUE;
    }
    if (reset) {
      this->saveStartParameters();
      SbVec3f worldpt;
      this->getLocalToWorldMatrix().multVecMatrix(projpt, worldpt);
      this->setStartingPoint(worldpt);
      startpt = projpt;
    }
    SbVec3f motion;
    SbVec3f localrestartpt;
    if (this->constraintState != CONSTRAINT_OFF) {
      this->getWorldToLocalMatrix().multVecMatrix(this->worldRestartPt,
                                                  localrestartpt);
      motion = localrestartpt - startpt;
    }
    else motion = projpt - startpt;
    switch(this->constraintState) {
    case CONSTRAINT_OFF:
      break;
    case CONSTRAINT_WAIT:
      if (this->isAdequateConstraintMotion()) {
        SbVec3f newmotion = projpt - localrestartpt;
        int biggest = 0;
        double bigval = fabs(newmotion[0]);
        if (fabs(newmotion[1]) > bigval) {
          biggest = 1;
        }
        motion[biggest] += newmotion[biggest];
        this->constraintState = CONSTRAINT_X + biggest;
      }
      else {
        return;
      }
      break;
    case CONSTRAINT_X:
      motion[0] += projpt[0] - localrestartpt[0];
      break;
    case CONSTRAINT_Y:
      motion[1] += projpt[1] - localrestartpt[1];
      break;
    case CONSTRAINT_Z:
      motion[2] += projpt[2] - localrestartpt[2];
    }
    this->setMotionMatrix(this->appendTranslation(this->getStartMotionMatrix(), motion));
  }
}

/*! \COININTERNAL
  Called when mouse button is released after picking and interacting
  with the dragger.
*/
void
SoTabPlaneDragger::dragFinish(void)
{
  this->whatkind = WHATKIND_NONE;
}

/*! \COININTERNAL */
void
SoTabPlaneDragger::startCB(void *, SoDragger * d)
{
  SoTabPlaneDragger *thisp = (SoTabPlaneDragger*)d;
  thisp->dragStart();
}

/*! \COININTERNAL */
void
SoTabPlaneDragger::motionCB(void *, SoDragger * d)
{
  SoTabPlaneDragger *thisp = (SoTabPlaneDragger*)d;
  thisp->drag();
}

/*! \COININTERNAL */
void
SoTabPlaneDragger::finishCB(void *, SoDragger * d)
{
  SoTabPlaneDragger *thisp = (SoTabPlaneDragger*)d;
  thisp->dragFinish();
}

/*! \COININTERNAL */
void
SoTabPlaneDragger::metaKeyChangeCB(void *, SoDragger * d)
{
  SoTabPlaneDragger *thisp = (SoTabPlaneDragger*)d;
  if (!thisp->isActive.getValue()) return;
  if (!(thisp->whatkind == WHATKIND_TRANSLATE)) return;

  const SoEvent *event = thisp->getEvent();
  if (event->wasShiftDown() && thisp->constraintState == CONSTRAINT_OFF) {
    thisp->drag();
  }
  else if (!event->wasShiftDown() && thisp->constraintState != CONSTRAINT_OFF) {
    thisp->drag();
  }
}


//
// this method is not as naughty as it sounds :-) It simply creates the parts
// it is not possible to configure through the dragger defaults file.
//
void
SoTabPlaneDragger::createPrivateParts(void)
{
  SoMaterialBinding *mb = SO_GET_ANY_PART(this, "scaleTabMaterialBinding", SoMaterialBinding);
  mb->value = SoMaterialBinding::OVERALL;

  SoNormalBinding *nb = SO_GET_ANY_PART(this, "scaleTabNormalBinding", SoNormalBinding);
  nb->value = SoNormalBinding::OVERALL;

  SoNormal *normal = SO_GET_ANY_PART(this, "scaleTabNormal", SoNormal);
  normal->vector.setValue(SbVec3f(0.0f, 0.0f, 1.0f));

  SoIndexedFaceSet *fs;
  SbString str;
  int idx = 0;
  int i, j;
  int32_t *ptr;

  for (i = 0; i < 8; i++) {
    if (i == 0 || i == 4) idx = 0;
    if (i < 4)
      str.sprintf("edgeScaleTab%d", i);
    else
      str.sprintf("cornerScaleTab%d", i-4);
    fs = (SoIndexedFaceSet*) this->getAnyPart(SbName(str.getString()), TRUE);
    fs->coordIndex.setNum(5);
    ptr = fs->coordIndex.startEditing();
    {
      for (j = 0; j < 4; j++) ptr[j] = idx++;
      ptr[4] = -1;
    }
    fs->coordIndex.finishEditing();
    fs->normalIndex.setValue(0);
    fs->materialIndex.setValue(0);
  }

  // turn off render caching since the geometry below this node might
  // change very often.
  SoSeparator *sep = SO_GET_ANY_PART(this, "scaleTabs", SoSeparator);
  sep->renderCaching = SoSeparator::OFF;
}

//
// returns the node in the SoSFNode field fieldname
//
SoNode *
SoTabPlaneDragger::getNodeFieldNode(const char *fieldname)
{
  SoField *field = this->getField(fieldname);
  assert(field != NULL);
  assert(field->isOfType(SoSFNode::getClassTypeId()));
  assert(((SoSFNode*)field)->getValue() != NULL);
  return ((SoSFNode*)field)->getValue();
}

// Undefine these again, as some of them are also used in other
// dragger sourcecode files (which causes trouble when using the
// compact build hack where all .cpp files are included into all.cpp).

#undef WHATKIND_NONE
#undef WHATKIND_SCALE
#undef WHATKIND_TRANSLATE

#undef CONSTRAINT_OFF
#undef CONSTRAINT_WAIT
#undef CONSTRAINT_X
#undef CONSTRAINT_Y
#undef CONSTRAINT_Z
