/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
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
  \class SoVRMLBillboard SoVRMLBillboard.h Inventor/VRMLnodes/SoVRMLBillboard.h
  \brief The SoVRMLBillboard class is used for rotating geometry towards the viewpoint.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim

  Billboard {
    eventIn      MFNode   addChildren
    eventIn      MFNode   removeChildren
    exposedField SFVec3f  axisOfRotation 0 1 0     # (-inf, inf)
    exposedField MFNode   children       []
    field        SFVec3f  bboxCenter     0 0 0     # (-inf, inf)
    field        SFVec3f  bboxSize       -1 -1 -1  # (0, inf) or -1,-1,-1
  }

  The Billboard node is a grouping node which modifies its coordinate
  system so that the Billboard node's local Z-axis turns to point at
  the viewer.  The Billboard node has children which may be other
  children nodes.  The axisOfRotation field specifies which axis to
  use to perform the rotation. This axis is defined in the local
  coordinate system.  When the axisOfRotation field is not (0, 0, 0),
  the following steps describe how to rotate the billboard to face the
  viewer:

  \li a. Compute the vector from the Billboard node's origin to the
  viewer's position. This vector is called the billboard-to-viewer
  vector.

  \li b. Compute the plane defined by the axisOfRotation and the
  billboard-to-viewer vector.

  \li c. Rotate the local Z-axis of the billboard into the plane from b., pivoting
  around the axisOfRotation.

  When the axisOfRotation field is set to (0, 0, 0), the special case
  of viewer-alignment is indicated. In this case, the object rotates
  to keep the billboard's local Y-axis parallel with the Y-axis of the
  viewer.  This special case is distinguished by setting the
  axisOfRotation to (0, 0, 0). The following steps describe how to
  align the billboard's Y-axis to the Y-axis of the viewer:

  \li d. Compute the billboard-to-viewer vector.

  \li e. Rotate the Z-axis of the billboard to be collinear with the
  billboard-to-viewer vector and pointing towards the viewer's
  position.

  \li f. Rotate the Y-axis of the billboard to be parallel and
  oriented in the same direction as the Y-axis of the viewer.

  If the axisOfRotation and the billboard-to-viewer line are
  coincident, the plane cannot be established and the resulting
  rotation of the billboard is undefined. For example, if the
  axisOfRotation is set to (0,1,0) (Y-axis) and the viewer flies over
  the billboard and peers directly down the Y-axis, the results are
  undefined.  Multiple instances of Billboard nodes (DEF/USE) operate
  as expected: each instance rotates in its unique coordinate system
  to face the viewer.  Subclause 4.6.5, Grouping and children nodes
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.5),
  provides a description of the children, addChildren, and
  removeChildren fields and eventIns.  The bboxCenter and bboxSize
  fields specify a bounding box that encloses the Billboard node's
  children. This is a hint that may be used for optimization
  purposes. The results are undefined if the specified bounding box is
  smaller than the actual bounding box of the children at any time. A
  default bboxSize value, (-1, -1, -1), implies that the bounding box
  is not specified and if needed shall be calculated by the browser. A
  description of the bboxCenter and bboxSize fields is contained in
  4.6.4, Bounding boxes
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.4),
*/

/*!
  \var SoSFVec3f SoVRMLBillboard::axisOfRotation

  The axis of rotation for the geometry.
*/

/*!
  \var SoSFVec3f SoVRMLBillboard::bboxCenter
  The bounding box center hint. Default value is (0, 0, 0).
*/

/*!
  \var SoSFVec3f SoVRMLBillboard::bboxSize
  The bounding box size hint. Default value is (-1, -1, -1).
*/

#include <Inventor/VRMLnodes/SoVRMLBillboard.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/SbPlane.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/SbRotation.h>
#include <Inventor/misc/SoGL.h>
#include <math.h>
#include <Inventor/errors/SoDebugError.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

SO_NODE_SOURCE(SoVRMLBillboard);

// Doc in parent
void
SoVRMLBillboard::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLBillboard, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLBillboard::SoVRMLBillboard(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLBillboard);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(axisOfRotation, (0.0f, 0.0f, 0.0f));
  SO_VRMLNODE_ADD_FIELD(bboxCenter, (0.0f, 0.0f, 0.0f));
  SO_VRMLNODE_ADD_FIELD(bboxSize, (-1.0f, -1.0f, -1.0f));
}

/*!
  Constructor. \a numchildren is the expected number of children.
*/
SoVRMLBillboard::SoVRMLBillboard(int numchildren)
  : inherited(numchildren)
{
}

/*!
  Destructor.
*/
SoVRMLBillboard::~SoVRMLBillboard()
{
}

// Doc in parent
void
SoVRMLBillboard::doAction(SoAction * action)
{
  SoState * state = action->getState();
  state->push();
  this->performRotation(state);
  SoGroup::doAction(action);
  state->pop();
}

// Doc in parent
void
SoVRMLBillboard::callback(SoCallbackAction * action)
{
  SoVRMLBillboard::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLBillboard::GLRender(SoGLRenderAction * action)
{
  switch (action->getCurPathCode()) {
  case SoAction::NO_PATH:
  case SoAction::BELOW_PATH:
    this->GLRenderBelowPath(action);
    break;
  case SoAction::OFF_PATH:
    // do nothing. Separator will reset state.
    break;
  case SoAction::IN_PATH:
    this->GLRenderInPath(action);
    break;
  }
}

// Doc in parent
void
SoVRMLBillboard::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoVRMLBillboard::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLBillboard::getMatrix(SoGetMatrixAction * action)
{
  // FIXME: hmmm, what should I do here. pederb, 2001-10-30
}

// Doc in parent
void
SoVRMLBillboard::pick(SoPickAction * action)
{
  SoVRMLBillboard::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLBillboard::search(SoSearchAction * action)
{
  SoNode::search(action);
  if (action->isFound()) return;
  SoGroup::doAction(action);
}

// Doc in parent
void
SoVRMLBillboard::GLRenderBelowPath(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  state->push();
  this->performRotation(state);

  int n = this->getChildren()->getLength();
  SoNode ** childarray = (SoNode**) this->getChildren()->getArrayPtr();

  action->pushCurPath();
  for (int i = 0; i < n && !action->hasTerminated(); i++) {
    if (action->abortNow()) {
      // only cache if we do a full traversal
      SoCacheElement::invalidate(state);
      break;
    }
    action->popPushCurPath(i, childarray[i]);
    childarray[i]->GLRenderBelowPath(action);

#if COIN_DEBUG
    // The GL error test is default disabled for this optimized
    // path.  If you get a GL error reporting an error in the
    // Separator node, enable this code by setting the environment
    // variable COIN_GLERROR_DEBUGGING to "1" to see exactly which
    // node caused the error.
    static SbBool chkglerr = sogl_glerror_debugging();
    if (chkglerr) {
      int err = glGetError();
      if (err != GL_NO_ERROR) {
        SoDebugError::postInfo("SoVRMLBillboard::GLRenderBelowPath",
                               "GL error: %s, nodetype: %s",
                               sogl_glerror_string(err).getString(),
                               (*this->getChildren())[i]->getTypeId().getName().getString());
      }
    }
#endif // COIN_DEBUG
  }
  action->popCurPath();
  state->pop();
}

// Doc in parent
void
SoVRMLBillboard::GLRenderInPath(SoGLRenderAction * action )
{
  int numindices;
  const int * indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    SoState * state = action->getState();
    SoNode ** childarray = (SoNode**) this->getChildren()->getArrayPtr();
    state->push();
    this->performRotation(state);

    int childidx = 0;
    for (int i = 0; i < numindices; i++) {
      SoAction::PathCode pathcode = action->getCurPathCode();
      for (; childidx < indices[i] && !action->hasTerminated(); childidx++) {
        SoNode * offpath = childarray[childidx];
        if (offpath->affectsState()) {
          action->pushCurPath(childidx, offpath);
          if (!action->abortNow()) {
            offpath->GLRenderOffPath(action);
          }
          else {
            SoCacheElement::invalidate(state);
          }
          action->popCurPath(pathcode);
        }
      }
      SoNode * inpath = childarray[childidx];
      action->pushCurPath(childidx, inpath);
      if (!action->abortNow()) {
        inpath->GLRenderInPath(action);
      }
      else {
        SoCacheElement::invalidate(state);
      }
      action->popCurPath(pathcode);
      childidx++;
    }
    state->pop();
  }
  else {
    // we got to the end of the path
    assert(action->getCurPathCode() == SoAction::BELOW_PATH);
    this->GLRenderBelowPath(action);
  }
}

// Doc in parent
void
SoVRMLBillboard::GLRenderOffPath(SoGLRenderAction * action)
{
  // do nothing
}

// Doc in parent
void
SoVRMLBillboard::notify(SoNotList * list)
{
  inherited::notify(list);
}

//
// private method that appends the needed rotation to the state
//
void
SoVRMLBillboard::performRotation(SoState * state) const
{
  SbRotation rot;
  const SbMatrix & mm = SoModelMatrixElement::get(state);
  SbMatrix imm = mm.inverse();

  SbVec3f toviewer;
  SbVec3f cameray(0.0f, 1.0f, 0.0f);
  const SbViewVolume & vv = SoViewVolumeElement::get(state);

  if (0 && vv.getProjectionType() == SbViewVolume::PERSPECTIVE) {
    SbVec3f worldpos(0.0f, 0.0f, 0.0f);
    mm.multVecMatrix(worldpos, worldpos);
    toviewer = vv.getProjectionPoint() - worldpos;
    imm.multDirMatrix(toviewer, toviewer);
  }
  else { // ORTHOGRAPHIC
    toviewer = - vv.getProjectionDirection();
    imm.multDirMatrix(toviewer, toviewer);
  }

  (void) toviewer.normalize();

  SbVec3f rotaxis = this->axisOfRotation.getValue();

  if (rotaxis == SbVec3f(0.0f, 0.0f, 0.0f)) {
    // 1. Compute the billboard-to-viewer vector.
    // 2. Rotate the Z-axis of the billboard to be collinear with the
    //    billboard-to-viewer vector and pointing towards the viewer's position.
    // 3. Rotate the Y-axis of the billboard to be parallel and oriented in the
    //    same direction as the Y-axis of the viewer.
    rot.setValue(SbVec3f(0.f, 0.0f, 1.0f), toviewer);
    SbVec3f viewup = vv.getViewUp();
    imm.multDirMatrix(viewup, viewup);

    SbVec3f yaxis(0.0f, 1.0f, 0.0f);
    rot.multVec(yaxis, yaxis);
    SbRotation rot2(yaxis, viewup);

    SbVec3f axis;
    float angle;
    rot.getValue(axis, angle);
    rot2.getValue(axis, angle);
    rot = rot * rot2;
    SoModelMatrixElement::rotateBy(state, (SoNode*) this, rot);
  }
  else {
    // 1. Compute the vector from the Billboard node's origin to the viewer's
    //    position. This vector is called the billboard-to-viewer vector.
    // 2. Compute the plane defined by the axisOfRotation and the
    //    billboard-to-viewer vector.
    // 3. Rotate the local Z-axis of the billboard into the plane from 2.,
    //    pivoting around the axisOfRotation.

    SbVec3f zaxis(0.0f, 0.0f, 1.0f);
    SbPlane plane(rotaxis.cross(toviewer), 0.0f);
    const SbVec3f n = plane.getNormal();
    SbVec3f vecinplane = zaxis - n * n[2];
    (void) vecinplane.normalize();
    if (vecinplane.dot(toviewer) < 0.0f) vecinplane = - vecinplane;
    float angle = acos(SbClamp(vecinplane.dot(zaxis), -1.0f, 1.0f));
    rot.setValue(rotaxis, n[2] < 0.0f ? angle : - angle);

    SoModelMatrixElement::rotateBy(state, (SoNode*) this, rot);
  }
}
