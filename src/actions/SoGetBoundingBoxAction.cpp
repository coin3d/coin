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
  \class SoGetBoundingBoxAction SoGetBoundingBoxAction.h Inventor/actions/SoGetBoundingBoxAction.h
  \brief The SoGetBoundingBoxAction class calculates bounding boxes for nodes and subgraphs.
  \ingroup actions

  The use of bounding boxes is ubiquitous within the Coin library. It
  is needed for the correct execution of and for performance
  enhancements during rendering, picking, caching, culling, etc.

  Apply this action to a path or scene graph root to calculate the
  bounding box and the center point of the geometry contained within
  the scene.


  SoSeparator nodes are aggressively caching the results of bounding
  box calculations, so that they are really only re-calculated
  whenever the scenegraph rooted below any SoSeparator node has been
  modified. This means that applying this action to scenegraphs, or
  parts of scenegraphs, should be very quick on successive runs for
  "static" parts of the scene.

  Note: the algorithm used is not guaranteed to give an optimal bbox;
  it always considers two bounding boxes and extends one of them to
  contain the other. Since the boxes need not be parallel to the
  principal axes the new box might not be a perfect fit for the box
  not extended (it's coordinate system is changed, and to make a
  perfect fit all objects contained within it would have to be
  re-examined).

  \sa SoSeparator::boundingBoxCaching
*/

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoSubActionP.h>

#include <Inventor/elements/SoBBoxModelMatrixElement.h>
#include <Inventor/elements/SoLocalBBoxMatrixElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoNode.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

/*!
  \enum SoGetBoundingBoxAction::ResetType
  \COININTERNAL
*/


SO_ACTION_SOURCE(SoGetBoundingBoxAction);


// Overridden from parent class.
void
SoGetBoundingBoxAction::initClass(void)
{
  SO_ACTION_INIT_CLASS(SoGetBoundingBoxAction, SoAction);

  SO_ENABLE(SoGetBoundingBoxAction, SoViewportRegionElement);
}

/*!
  Constructor.

  It might seem unnecessary to have to pass in a viewport region
  argument to calculate bounding boxes, but there is a good reason for
  this: a few shape nodes needs to know the viewport region to
  calculate their bounding box -- these include SoText2 and SoImage,
  among others.

  What is particular about these shapes is that they are fundamentally
  2D shapes, but they are being rendered on the screen "surface" as if
  they were in a 3D scene. (This is possible because we can match each
  pixel's depth value against the 3D shapes in the scene.)

  To compute an accurate 3D bounding box of a shape rendered in 2D on
  the screen "surface", you need to "de-project" the screen-space area
  it occupies to a 2D rectangle placed at some depth in the
  scene. This "de-projecting" operation needs to know about the
  dimensions of the viewport.

  Also, some 3D shapes like for instance SoNurbsSurface, get slightly
  distorted if there's an SoComplexity node in the scenegraph with the
  SoComplexity::value field set to SCREEN_SPACE. Then it is also
  necessary to know the viewport region to find out how to accurately
  calculate the bounding box of those shapes.

  You would usually want to pass in a viewport region equal to the
  layout of the current renderarea canvas. If you have a viewer or
  So@Gui@RenderArea available, you can get hold of the viewport region
  data simply by doing

  \code
     const SbViewportRegion & vpreg = viewer->getViewportRegion();
  \endcode

  (If you don't have a viewer or renderarea available in your
  application at the point where you want to get the bounding box, it
  probably doesn't matter much what you set it to. The accuracy of the
  bounding box calculation might be slightly wrong versus the actual
  rendered appearance of the scene, but this is usually not
  noticable.)
*/
SoGetBoundingBoxAction::SoGetBoundingBoxAction(const SbViewportRegion & vp)
  : center(0, 0, 0),
    vpregion(vp),
    resetpath(NULL),
    flags(SoGetBoundingBoxAction::RESET_BEFORE)
{
  SO_ACTION_CONSTRUCTOR(SoGetBoundingBoxAction);
}

/*!
  Destructor.
*/
SoGetBoundingBoxAction::~SoGetBoundingBoxAction()
{
}

// *************************************************************************

/*!
  Set a new viewport region with this method, if it has changed from
  the oned passed in with the constructor.
*/
void
SoGetBoundingBoxAction::setViewportRegion(const SbViewportRegion & newregion)
{
  this->vpregion = newregion;
}

/*!
  Returns the viewport region used by the action instance.
*/
const SbViewportRegion &
SoGetBoundingBoxAction::getViewportRegion(void) const
{
  return this->vpregion;
}

/*!
  Returns the projected bounding box after (or during) traversal.
*/
SbBox3f
SoGetBoundingBoxAction::getBoundingBox(void) const
{
  return this->bbox.project();
}

/*!
  Returns the bounding box and transformation matrix to global
  coordinates. Use after (or during) traversal.
*/
SbXfBox3f &
SoGetBoundingBoxAction::getXfBoundingBox(void)
{
  return this->bbox;
}

/*!
  Returns center point of scene after the action has been applied.

  This might differ from the geometric center of the bounding box, as
  shape nodes may "weight" the center point according to various
  criteria (i.e. a faceset could for instance weight the center point
  according to the area within its bounding box where there are more
  polygons).
*/
const SbVec3f &
SoGetBoundingBoxAction::getCenter(void) const
{
  if (!this->isCenterSet()) {
    // Cast away constness and set.
    SoGetBoundingBoxAction * action = (SoGetBoundingBoxAction *)this;
    action->center.setValue(0.0f, 0.0f, 0.0f);
  }
  // Center point should not be affected by the current
  // transformation.
  return this->center;
}

/*!
  Sets whether the returned bounding box should be calculated in the
  coordinate system of the camera space or not.
*/
void
SoGetBoundingBoxAction::setInCameraSpace(const SbBool on)
{
  if (on) this->flags |= SoGetBoundingBoxAction::CAMERA_SPACE;
  else this->flags &= ~SoGetBoundingBoxAction::CAMERA_SPACE;
}

/*!
  Returns whether the bounding box returned is to be in camera space.
*/
SbBool
SoGetBoundingBoxAction::isInCameraSpace(void) const
{
  return (this->flags & SoGetBoundingBoxAction::CAMERA_SPACE) != 0;
}

/*!
  Forces the computed bounding box to be reset and the transformation
  to be identity before or after the tail node of \a path, depending
  on the \a resetbefore argument.  \c NULL can be specified for the \a
  path argument to disable this behavior.

  \sa getResetPath(), isResetPath(), isResetBefore(), getWhatReset()
*/

void
SoGetBoundingBoxAction::setResetPath(const SoPath * path,
                                     const SbBool resetbefore,
                                     const ResetType what)
{
  this->resetpath = path;
  this->resettype = what;
  if (resetbefore) this->flags |= SoGetBoundingBoxAction::RESET_BEFORE;
  else this->flags &= ~SoGetBoundingBoxAction::RESET_BEFORE;
}

/*!
  Returns the reset path (or \c NULL).

  \sa setResetPath(), isResetPath(), isResetBefore(), getWhatReset()
*/
const SoPath *
SoGetBoundingBoxAction::getResetPath(void) const
{
  return this->resetpath;
}

/*!
  Returns whether a reset path is set or not.

  \sa setResetPath(), getResetPath(), isResetBefore(), getWhatReset()
*/
SbBool
SoGetBoundingBoxAction::isResetPath(void) const
{
  return this->resetpath != NULL;
}

/*!
  Returns whether the bounding box and transformation is reset before
  or after the tail node of the reset path.

  \sa setResetPath(), getResetPath(), isResetPath(), getWhatReset()
*/
SbBool
SoGetBoundingBoxAction::isResetBefore(void) const
{
  return (this->flags & SoGetBoundingBoxAction::RESET_BEFORE) != 0;
}

/*!
  Returns what type of reset has been specified for the reset path.

  \sa setResetPath(), getResetPath(), isResetPath(), isResetBefore()
*/
SoGetBoundingBoxAction::ResetType
SoGetBoundingBoxAction::getWhatReset(void) const
{
  return this->resettype;
}

/*!
  \COININTERNAL
  Called before node traversal of each node (from SoNode action method).
*/
void
SoGetBoundingBoxAction::checkResetBefore(void)
{
  if (this->resetpath && this->isResetBefore()) {
    const SoFullPath * curpath = (const SoFullPath *) this->getCurPath();
    const SoFullPath * resetpath = (const SoFullPath *) this->resetpath;
    if ((curpath->getTail() == resetpath->getTail()) &&
        curpath->containsPath(resetpath)) {
      if (this->resettype & SoGetBoundingBoxAction::TRANSFORM) {
        SoBBoxModelMatrixElement::reset(this->getState(), curpath->getTail());
      }
      if (this->resettype & SoGetBoundingBoxAction::BBOX) {
        this->bbox.makeEmpty();
        this->bbox.setTransform(SbMatrix::identity());
        this->resetCenter();
      }
    }
  }
}

/*!
  \COININTERNAL
  Called after node traversal of each node (from SoNode action method).
*/
void
SoGetBoundingBoxAction::checkResetAfter(void)
{
  if (this->resetpath && !this->isResetBefore()) {
    const SoFullPath * curpath = (const SoFullPath *) this->getCurPath();
    const SoFullPath * resetpath = (const SoFullPath *) this->resetpath;
    if ((curpath->getTail() == resetpath->getTail()) &&
        curpath->containsPath(resetpath)) {
      if (this->resettype & SoGetBoundingBoxAction::TRANSFORM) {
        SoBBoxModelMatrixElement::reset(this->getState(), curpath->getTail());
      }
      if (this->resettype & SoGetBoundingBoxAction::BBOX) {
        this->bbox.makeEmpty();
        this->bbox.setTransform(SbMatrix::identity());
        this->resetCenter();
      }
    }
  }
}

/*!
  Extend bounding box by the given \a box. Called from nodes during
  traversal.

  Should usually not be of interest to application programmers, unless
  you're extending Coin with your own shapenode extension classes.
*/
void
SoGetBoundingBoxAction::extendBy(const SbBox3f & box)
{
  if (box.isEmpty()) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoGetBoundingBoxAction::extendBy", "empty box");
#endif // COIN_DEBUG
    return;
  }

  SbXfBox3f xfbox = box;
  SbMatrix transform = SoLocalBBoxMatrixElement::get(this->getState());
  if (this->isInCameraSpace()) {
    transform.multRight(SoViewingMatrixElement::get(this->getState()));
  }

  xfbox.transform(transform);
  this->bbox.extendBy(xfbox);
}

/*! \overload */
void
SoGetBoundingBoxAction::extendBy(const SbXfBox3f & box)
{
  if (box.isEmpty()) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoGetBoundingBoxAction::extendBy", "empty box");
#endif // COIN_DEBUG
    return;
  }

  SbXfBox3f lbox = box;
  SbMatrix transform = SoLocalBBoxMatrixElement::get(this->state);
  if (this->isInCameraSpace()) {
    transform.multRight(SoViewingMatrixElement::get(this->state));
  }
  lbox.transform(transform);
  this->bbox.extendBy(lbox);
}

/*!
  \COININTERNAL
  Set a new center point during traversal.
*/
void
SoGetBoundingBoxAction::setCenter(const SbVec3f & center,
                                  const SbBool transformcenter)
{
  assert(!this->isCenterSet());
  this->flags |= SoGetBoundingBoxAction::CENTER_SET;

  if (transformcenter) {
    SbMatrix lmat = SoLocalBBoxMatrixElement::get(this->state);
    if (this->isInCameraSpace()) {
      lmat.multRight(SoViewingMatrixElement::get(this->state));
    }
    lmat.multVecMatrix(center, this->center);
  }
  else {
    this->center = center;
  }

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoGetBoundingBoxAction::setCenter",
                         "center: <%f, %f, %f>, transformcenter: %s, "
                         "this->center: <%f, %f, %f>",
                         center[0], center[1], center[2],
                         transformcenter ? "TRUE" : "FALSE",
                         this->center[0], this->center[1], this->center[2]);
#endif // debug
}

/*!
  \COININTERNAL
  Query about the center point during traversal.
*/
SbBool
SoGetBoundingBoxAction::isCenterSet(void) const
{
  return (this->flags & SoGetBoundingBoxAction::CENTER_SET) != 0;
}

/*!
  \COININTERNAL
  Reset the scene center point during traversal.
*/
void
SoGetBoundingBoxAction::resetCenter(void)
{
  this->flags &= ~SoGetBoundingBoxAction::CENTER_SET;
  this->center.setValue(0.0f, 0.0f, 0.0f);
}

// Documented in superclass. Overridden to reset center point and
// bounding box before traversal starts.
void
SoGetBoundingBoxAction::beginTraversal(SoNode * node)
{
  this->resetCenter();
  this->bbox.makeEmpty();

  SoViewportRegionElement::set(this->getState(), this->vpregion);
  inherited::beginTraversal(node);
}
