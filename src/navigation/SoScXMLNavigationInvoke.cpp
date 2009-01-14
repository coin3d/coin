/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include "navigation/SoScXMLNavigationInvoke.h"

#include <assert.h>
#include <math.h>
#include <float.h>

#include <Inventor/SbViewVolume.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbLine.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/fields/SoSFVec3d.h>
#include <Inventor/scxml/SoScXMLStateMachine.h>
#include <Inventor/scxml/SoScXMLEvent.h>

SCXML_OBJECT_ABSTRACT_SOURCE(SoScXMLNavigationInvoke);

void
SoScXMLNavigationInvoke::initClass(void)
{
  SCXML_OBJECT_INIT_ABSTRACT_CLASS(SoScXMLNavigationInvoke, ScXMLInvoke);
}

// Rotate camera around its focal point.

void
SoScXMLNavigationInvoke::reorientCamera(SoCamera * camera, const SbRotation & rot)
{
  if (camera == NULL) return;

  // Find global coordinates of focal point.
  SbVec3f direction;
  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
  SbVec3f focalpoint = camera->position.getValue() +
    camera->focalDistance.getValue() * direction;

  // Set new orientation value by accumulating the new rotation.
  camera->orientation = rot * camera->orientation.getValue();

  // Reposition camera so we are still pointing at the same old focal point.
  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
  camera->position = focalpoint - camera->focalDistance.getValue() * direction;

  // some custom code to support UTMCamera cameras
  static const SoType utmcamtype(SoType::fromName("UTMCamera"));
  if (utmcamtype != SoType::badType() && camera->isOfType(utmcamtype)) {
    SbVec3d offset;
    offset.setValue(camera->position.getValue());
    SoSFVec3d * utmpositionfield =
      (SoSFVec3d *) camera->getField("utmposition");
    assert(utmpositionfield);
    utmpositionfield->setValue(utmpositionfield->getValue()+offset);
    camera->position.setValue(0.0f, 0.0f, 0.0f);
  }
}

// Move camera parallel with the plane orthogonal to the camera
// direction vector.

void
SoScXMLNavigationInvoke::panCamera(SoCamera * camera,
                                   float vpaspect,
                                   const SbPlane & panplane,
                                   const SbVec2f & previous,
                                   const SbVec2f & current)
{
  if (camera == NULL) return; // can happen for empty scenegraph
  if (current == previous) return; // useless invocation

  // Find projection points for the last and current mouse coordinates.
  SbViewVolume vv = camera->getViewVolume(vpaspect);
  SbLine line;
  vv.projectPointToLine(current, line);
  SbVec3f current_planept;
  panplane.intersect(line, current_planept);
  vv.projectPointToLine(previous, line);
  SbVec3f old_planept;
  panplane.intersect(line, old_planept);

  // Reposition camera according to the vector difference between the
  // projected points.
  camera->position =
    camera->position.getValue() - (current_planept - old_planept);
}

// *************************************************************************
// Dependent on the camera type this will either shrink or expand the
// height of the viewport (orthogonal camera) or move the camera
// closer or further away from the focal point in the scene.

void
SoScXMLNavigationInvoke::zoom(SoCamera * camera, float diffvalue)
{
  if (camera == NULL) return; // can happen for empty scenegraph
  SoType cameratype = camera->getTypeId();

  // This will be in the range of <0, ->>.
  float multiplicator = float(exp(diffvalue));

  if (cameratype.isDerivedFrom(SoOrthographicCamera::getClassTypeId())) {

    // Since there's no perspective, "zooming" in the original sense
    // of the word won't have any visible effect. So we just increase
    // or decrease the field-of-view values of the camera instead, to
    // "shrink" the projection size of the model / scene.
    SoOrthographicCamera * oc = (SoOrthographicCamera *) camera;
    oc->height = oc->height.getValue() * multiplicator;

  }
  else {
    // FrustumCamera can be found in the SmallChange CVS module (it's
    // a camera that lets you specify (for instance) an off-center
    // frustum (similar to glFrustum())
    static const SbName frustumtypename("FrustumCamera");
    if (!cameratype.isDerivedFrom(SoPerspectiveCamera::getClassTypeId()) &&
        cameratype.getName() != frustumtypename) {
      static SbBool first = TRUE;
      if (first) {
        SoDebugError::postWarning("SoGuiFullViewerP::zoom",
                                  "Unknown camera type, "
                                  "will zoom by moving position, but this might not be correct.");
        first = FALSE;
      }
    }
    
    const float oldfocaldist = camera->focalDistance.getValue();
    const float newfocaldist = oldfocaldist * multiplicator;

    SbVec3f direction;
    camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);

    const SbVec3f oldpos = camera->position.getValue();
    const SbVec3f newpos = oldpos + (newfocaldist - oldfocaldist) * -direction;

    // This catches a rather common user interface "buglet": if the
    // user zooms the camera out to a distance from origo larger than
    // what we still can safely do floating point calculations on
    // (i.e. without getting NaN or Inf values), the faulty floating
    // point values will propagate until we start to get debug error
    // messages and eventually an assert failure from core Coin code.
    //
    // With the below bounds check, this problem is avoided.
    //
    // (But note that we depend on the input argument ''diffvalue'' to
    // be small enough that zooming happens gradually. Ideally, we
    // should also check distorigo with isinf() and isnan() (or
    // inversely; isinfite()), but those only became standardized with
    // C99.)
    const float distorigo = newpos.length();
    // sqrt(FLT_MAX) == ~ 1e+19, which should be both safe for further
    // calculations and ok for the end-user and app-programmer.
    if (distorigo > float(sqrt(FLT_MAX))) {

      //SoDebugError::postWarning("SoGuiFullViewerP::zoom",
      //                          "zoomed too far (distance to origo==%f (%e))",
      //                          distorigo, distorigo);
    }
    else {
      camera->position = newpos;
      camera->focalDistance = newfocaldist;
    }
  }
}


SoScXMLStateMachine *
SoScXMLNavigationInvoke::castToSo(ScXMLStateMachine * statemachine) const
{
  if (!statemachine) {
    SbString id;
    id.sprintf("%s::invoke", this->getTypeId().getName().getString());
    SoDebugError::post(id.getString(),
                       "Statemachine argument was NULL.");
    return NULL;
  }
  if (!statemachine->isOfType(SoScXMLStateMachine::getClassTypeId())) {
    SbString id;
    id.sprintf("%s::invoke", this->getTypeId().getName().getString());
    SoDebugError::post(id.getString(),
                       "SoScXMLNavigationInvoke-derived objects require "
                       "SoScXMLStateMachine-derived state machines.");
    return NULL;
  }
  return static_cast<SoScXMLStateMachine *>(statemachine);
}

const SoEvent *
SoScXMLNavigationInvoke::getSoEvent(SoScXMLStateMachine * statemachine) const
{
  const ScXMLEvent * ev = statemachine->getCurrentEvent();
  if (!ev) {
    SbString id;
    id.sprintf("%s::invoke", this->getTypeId().getName().getString());
    SoDebugError::post(id.getString(),
                       "SoScXMLStateMachine has no current event.");
    return NULL;
  }
  if (!ev->isOfType(SoScXMLEvent::getClassTypeId())) {
    SbString id;
    id.sprintf("%s::invoke", this->getTypeId().getName().getString());
    SoDebugError::post(id.getString(),
                       "SoScXMLStateMachine current event is not "
                       "of SoScXMLStateMachine type.");
    return NULL;
  }
  const SoEvent * soev = static_cast<const SoScXMLEvent *>(ev)->getSoEvent();
  if (!soev) {
    SbString id;
    id.sprintf("%s::invoke", this->getTypeId().getName().getString());
    SoDebugError::post(id.getString(),
                       "SoScXMLStateMachine current event does not carry "
                       "an SoEvent-derived event.");
    return NULL;
  }
  return soev;
}
