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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SbViewVolume SbLinear.h Inventor/SbLinear.h
  \brief The SbViewVolume class is a viewing volume in 3D space.
  \ingroup base

  This class contains the necessary information for storing a view
  volume.  It has methods for projection of primitives from or into
  the 3D volume, doing camera transforms, view volume transforms etc.

  \sa SbViewportRegion.
*/

/*¡
These methods are still not implemented:
<UL>
<LI>SbViewVolume::getAlignRotation()</LI>
<LI>SbViewVolume::narrow()</LI>
<LI>SbViewVolume::zNarrow()</LI>
</UL>
*/

/*¡
These methods need better documentation:
<UL>
<LI>SbViewVolume::ortho()</LI>
<LI>SbViewVolume::perspective()</LI>
</UL>
*/

#include <assert.h>
#include <Inventor/SbViewVolume.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbLine.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbBox2f.h>
#include <Inventor/SbBox3f.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \enum SbViewVolume::ProjectionType

  An SbViewVolume instance can represent either an orthogonal projection
  volume or a perspective projection volume.

  \sa ortho(), perspective(), getProjectionType().
 */


/*!
  Constructor. Note that the SbViewVolume instance will be uninitialized
  until you explicitly call \a ortho() or \a perspective().

  \sa ortho(), perspective().
 */
SbViewVolume::SbViewVolume(void)
{
}

/*!
  Destructor.
 */
SbViewVolume::~SbViewVolume(void)
{
}

/*!
  Returns the view volume's affine matrix and projection matrix.

  \sa getMatrix(), getCameraSpaceMatrix()
 */
void
SbViewVolume::getMatrices(SbMatrix& affine, SbMatrix& proj) const
{
  affine = this->getCameraSpaceMatrix();

  // Find rotation of camera "up vector".
  SbVec3f upvec = this->upperleftfrust - this->lowerleftfrust;
#if COIN_DEBUG
  if (upvec == SbVec3f(0.0f, 0.0f, 0.0f)) {
    SoDebugError::postWarning("SbViewVolume::getMatrices",
                              "empty frustum!");
    proj = SbMatrix::identity();
    return;
  }
#endif // COIN_DEBUG
  upvec.normalize();
  affine.multDirMatrix(upvec, upvec);
  SbRotation zrot(upvec, SbVec3f(0, 1, 0));

  // Combine with affine matrix.
  SbMatrix tmp;
  tmp.setRotate(zrot);
  affine.multRight(tmp);


  // Transform frustum corner vectors back to original position.
  SbVec3f ll, lr, ul;
  affine.multVecMatrix(this->lowerleftfrust, ll);
  affine.multVecMatrix(this->upperleftfrust, ul);
  affine.multVecMatrix(this->lowerrightfrust, lr);

  float l = ll[0];
  float r = lr[0];
  float t = ul[1];
  float b = ll[1];
  float n = this->getNearDist();
  float f = n + this->getDepth();

  if(this->type == SbViewVolume::ORTHOGRAPHIC)
    proj = SbViewVolume::getOrthoProjection(l, r, b, t, n, f);
  else
    proj = SbViewVolume::getPerspectiveProjection(l, r, b, t, n, f);
}

/*!
  Returns the combined affine and projection matrix.

  \sa getMatrices(), getCameraSpaceMatrix()
 */
SbMatrix
SbViewVolume::getMatrix(void) const
{
  SbMatrix affine, proj;
  this->getMatrices(affine, proj);
  return affine.multRight(proj);
}

/*!
  Returns a matrix which will translate the view volume camera back to
  origo, and rotate the camera so it'll point along the negative z axis.

  Note that the matrix will \a not include the rotation necessary to
  make the camera up vector point along the positive y axis (i.e.
  camera roll is not accounted for).

  \sa getMatrices(), getMatrix()
 */
SbMatrix
SbViewVolume::getCameraSpaceMatrix(void) const
{
  // Find rotation of projection direction.
  SbRotation pdrot =
    SbRotation(this->projectiondir, SbVec3f(0.0f, 0.0f, -1.0f));

  // Combine transforms.
  SbMatrix mat, tmp;
  mat.setTranslate(-this->projectionpt);
  tmp.setRotate(pdrot);
  mat.multRight(tmp);

  return mat;
}

/*!
  Project the given 2D point from the projection plane into a 3D line.

  \a pt coordinates should be normalized to be within [0, 1].
 */
void
SbViewVolume::projectPointToLine(const SbVec2f& pt, SbLine& line) const
{
  SbVec3f pt0, pt1;
  this->projectPointToLine(pt, pt0, pt1);
  line.setValue(pt0, pt1);
}

/*!
  Project the given 2D point from the projection plane into two points
  defining a 3D line. The first point, \a line0, will be the
  corresponding point for the projection on the near plane, while \a line1
  will be the line endpoint, lying in the far plane.
 */
void
SbViewVolume::projectPointToLine(const SbVec2f& pt,
                                 SbVec3f& line0, SbVec3f& line1) const
{
#if 0 // OBSOLETED, pederb 19991215. More efficient version below
  SbVec3f scr_n(pt[0], pt[1], -1.0f);
  SbVec3f scr_f(pt[0], pt[1], 1.0f);

  scr_n[0] -= 0.5f;
  scr_n[1] -= 0.5f;
  scr_f[0] -= 0.5f;
  scr_f[1] -= 0.5f;

  scr_n[0] *= 2.0f;
  scr_n[1] *= 2.0f;
  scr_f[0] *= 2.0f;
  scr_f[1] *= 2.0f;

  SbMatrix m = this->getMatrix().inverse();

  m.multVecMatrix(scr_n, line0);
  m.multVecMatrix(scr_f, line1);

#else // new, faster version

  SbVec3f dx = this->lowerrightfrust - this->lowerleftfrust;
  SbVec3f dy = this->upperleftfrust - this->lowerleftfrust;

#if COIN_DEBUG
  if (dx.sqrLength() == 0.0f || dy.sqrLength() == 0.0f) {
    SoDebugError::postWarning("SbViewVolume::projectPointToLine",
                              "invalid frustum: <%f, %f, %f>",
                              this->lowerrightfrust,
                              this->lowerleftfrust,
                              this->upperleftfrust);
    return;
  }
#endif // COIN_DEBUG

  line0 = this->lowerleftfrust + dx*pt[0] + dy*pt[1];
  SbVec3f dir;
  if (this->type == PERSPECTIVE) {
    dir = line0 - projectionpt;
    dir.normalize();
    line1 = line0 + dir * this->getDepth() / dir.dot(this->projectiondir);
  }
  else {
    dir = this->projectiondir;
    line1 = line0 + dir*this->getDepth();
  }
#endif // faster version
}

/*!
  Project the \a src point to a normalized set of screen coordinates in
  the projection plane and place the result in \a dst.

  It is safe to let \a src and \dst be the same SbVec3f instance.

  The z-coordinate of \a dst is monotonically increasing for points
  closer to the far plane. Note however that this is not a linear
  relationship, the \a dst z-coordinate is calculated as follows:

   Orthogonal view:  DSTz = (-2 * SRCz - far - near) / (far - near),
   Perspective view:  DSTz = (-SRCz * (far - near) - 2*far*near) / (far - near)
 */
void
SbViewVolume::projectToScreen(const SbVec3f& src, SbVec3f& dst) const
{
  this->getMatrix().multVecMatrix(src, dst);

  dst[0] /= 2.0f;
  dst[1] /= 2.0f;

  dst[0] += 0.5f;
  dst[1] += 0.5f;
}

/*!
  Returns an SbPlane instance which has a normal vector in the opposite
  direction of which the camera is pointing. This means the
  plane will be parallel to the near and far clipping planes.

  \sa getSightPoint()
 */
SbPlane
SbViewVolume::getPlane(const float distFromEye) const
{
  return SbPlane(-this->projectiondir,
                 this->projectionpt + distFromEye * this->projectiondir);
}

/*!
  Returns the point on the center line-of-sight from the camera position
  with the given distance.

  \sa getPlane()
 */
SbVec3f
SbViewVolume::getSightPoint(const float distFromEye) const
{
  return this->projectionpt + this->projectiondir * distFromEye;
}

/*!
  Return the 3D point which projects to \a normPoint (in normalized
  coordinates) and lies on the plane perpendicular to the camera
  direction and \a distFromEye distance away from the camera position.
 */
SbVec3f
SbViewVolume::getPlanePoint(const float distFromEye,
                            const SbVec2f& normPoint) const
{
  SbVec3f volpt;

  if(this->getProjectionType() == SbViewVolume::ORTHOGRAPHIC) {
    SbVec3f scr(normPoint[0], normPoint[1], -1.0f);

    scr[0] -= 0.5f;
    scr[1] -= 0.5f;
    scr[0] *= 2.0f;
    scr[1] *= 2.0f;

    SbMatrix m = this->getMatrix().inverse();
    m.multVecMatrix(scr, volpt);
    volpt += (distFromEye - this->getNearDist()) *
      this->getProjectionDirection();
  }
  else {
    // Find vector pointing in the direction of the normalized 2D
    // point.
    SbVec3f dvec =
      this->lowerleftfrust +
      (this->lowerrightfrust - this->lowerleftfrust) * normPoint[0] +
      (this->upperleftfrust - this->lowerleftfrust) * normPoint[1];
    dvec -= this->getProjectionPoint();
    dvec.normalize();

    // Distance to point.
    float d = distFromEye/dvec.dot(this->getProjectionDirection());

    volpt = d * dvec + this->getProjectionPoint();
  }

  return volpt;
}

/*!
  TODO: not implemented
 */
SbRotation
SbViewVolume::getAlignRotation(SbBool /* rightAngleOnly */) const
{
  // TODO: not implemented.
  assert(0);
  return SbRotation::identity();
}

/*!
  Given a sphere with center in \a worldCenter and an initial radius of \a 1.0,
  return the scale factor needed to make this sphere have a \a normRadius
  radius when projected onto the near clipping plane.
 */
float
SbViewVolume::getWorldToScreenScale(const SbVec3f& worldCenter,
                                    float normRadius) const
{
#if COIN_DEBUG
  if (normRadius < 0.0f) {
    SoDebugError::postWarning("SbViewVolume::getWorldToScreenScale",
                              "normRadius (%f) should be >=0.0f.", normRadius);
    return 1.0f;
  }
  if (this->getWidth() == 0.0f || this->getHeight() == 0.0f) {
    SoDebugError::postWarning("SbViewVolume::getWorldToScreenScale",
                              "invalid frustum <%f, %f>",
                              this->getWidth(), this->getHeight());
    return 1.0f;
  }
#endif // COIN_DEBUG

  if(this->getProjectionType() == SbViewVolume::ORTHOGRAPHIC) {
    SbVec3f rightvec = this->lowerrightfrust - this->lowerleftfrust;
    return (normRadius * rightvec).length();
  }
  else {
    // Find screen space coordinates of sphere center point and tangent
    // point.
    SbVec3f center_scr;
    this->projectToScreen(worldCenter, center_scr);
    center_scr[0] += normRadius;

    // Vectors spanning the projection plane.
    SbVec3f upvec = this->upperleftfrust - this->lowerleftfrust;
    SbVec3f rightvec = this->lowerrightfrust - this->lowerleftfrust;

    // Find projection plane point for the sphere tangent touch point,
    // which is then used to define the sphere tangent line.
    SbVec3f ppp =
      this->lowerleftfrust + center_scr[0] * rightvec + center_scr[1] * upvec;
    SbLine tl(this->getProjectionPoint(), ppp);

    // Define the plane which is cutting the sphere in half and is normal
    // to the camera direction.
    SbVec3f sphere_camera_vec = worldCenter - this->getProjectionPoint();
    SbPlane p = SbPlane(sphere_camera_vec, worldCenter);

    // Find tangent point of sphere.
    SbVec3f tangentpt;
    SbBool result = p.intersect(tl, tangentpt);
    assert(result != FALSE);

    // Return radius (which is equal to the scale factor, since we're
    // dealing with a unit sphere).
    return (tangentpt - worldCenter).length();
  }
}

/*!
  Projects the given box onto the projection plane and returns the
  normalized screen space it occupies.
 */
SbVec2f
SbViewVolume::projectBox(const SbBox3f& box) const
{
#if COIN_DEBUG
  if (box.isEmpty()) {
    SoDebugError::postWarning("SbViewVolume::projectBox",
                              "Box is empty.");
  }
#endif // COIN_DEBUG

  SbVec3f mincorner = box.getMin();
  SbVec3f maxcorner = box.getMax();
  SbBox2f span;

  for(int i=0; i < 2; i++) {
    for(int j=0; j < 2; j++) {
      for(int k=0; k < 2; k++) {
        SbVec3f corner(i ? mincorner[0] : maxcorner[0],
                       j ? mincorner[1] : maxcorner[1],
                       k ? mincorner[2] : maxcorner[2]);
        this->projectToScreen(corner, corner);
        span.extendBy(SbVec2f(corner[0], corner[1]));
      }
    }
  }

  return SbVec2f(span.getMax()[0] - span.getMin()[0],
                 span.getMax()[1] - span.getMin()[1]);
}

/*!
  Returns a narrowed version of the view volume which is within the
  given [0, 1] normalized coordinates. The coordinates are taken to be
  corner points of a normalized "view window" on the near clipping
  plane.  I.e.:

  \code
  SbViewVolume view;
  view.ortho(0, 100, 0, 100, 0.1, 1000);
  view = view.narrow(0.25, 0.5, 0.75, 1.0);
  \endcode

  ..will give a view volume with corner points <25, 75> and <50, 100>.

  \sa scale(), scaleWidth(), scaleHeight()
 */
SbViewVolume
SbViewVolume::narrow(float left, float bottom,
                     float right, float top) const
{
#if COIN_DEBUG
  if (left<0.0f) {
    SoDebugError::postWarning("SbViewVolume::narrow",
                              "left coordinate (%f) should be >=0.0f. "
                              "Clamping to 0.0f.",left);
    left=0.0f;
  }
  if (right>1.0f) {
    SoDebugError::postWarning("SbViewVolume::narrow",
                              "right coordinate (%f) should be <=1.0f. "
                              "Clamping to 1.0f.",right);
    right=1.0f;
  }
  if (bottom<0.0f) {
    SoDebugError::postWarning("SbViewVolume::narrow",
                              "bottom coordinate (%f) should be >=0.0f. "
                              "Clamping to 0.0f.",bottom);
    bottom=0.0f;
  }
  if (top>1.0f) {
    SoDebugError::postWarning("SbViewVolume::narrow",
                              "top coordinate (%f) should be <=1.0f. "
                              "Clamping to 1.0f.",top);
    top=1.0f;
  }
  if (left>right) {
    SoDebugError::postWarning("SbViewVolume::narrow",
                              "right coordinate (%f) should be larger than "
                              "left coordinate (%f). Swapping left/right.",
                              right,left);
    float tmp=right;
    right=left;
    left=tmp;
  }
  if (bottom>top) {
    SoDebugError::postWarning("SbViewVolume::narrow",
                              "top coordinate (%f) should be larger than "
                              "bottom coordinate (%f). Swapping top/bottom.",
                              top,bottom);
    float tmp=top;
    top=bottom;
    bottom=tmp;
  }
#endif // COIN_DEBUG

  SbViewVolume nvw = *this;

  float w = nvw.getWidth();
  float h = nvw.getHeight();

  SbVec3f xvec = this->lowerrightfrust - this->lowerleftfrust;
  xvec.normalize();
  SbVec3f yvec = this->upperleftfrust - this->lowerleftfrust;
  yvec.normalize();

  nvw.upperleftfrust = nvw.lowerleftfrust + (xvec * left * w + yvec * top * h);
  nvw.lowerrightfrust =
    nvw.lowerleftfrust + (xvec * right * w + yvec * bottom * h);
  nvw.lowerleftfrust += xvec * left * w + yvec * bottom * h;

  return nvw;
}

/*!
  TODO: not implemented
 */
SbViewVolume
SbViewVolume::narrow(const SbBox3f& /* box */) const
{
  //TODO: Check if legal box in debug-mode.
  // TODO: not implemented.
  assert(0);
  return *this;
}

/*!
  Set up the view volume as a rectangular box for orthographic
  parallel projections. The line of sight will be along the negative
  z axis, through the center of the plane defined by the point
  <(right+left)/2, (top+bottom)/2, 0>.

  TODO: illustrate?

  \sa perspective().
*/
void
SbViewVolume::ortho(float left, float right,
                    float bottom, float top,
                    float nearval, float farval)
{
#if COIN_DEBUG
  if (left>right) {
    SoDebugError::postWarning("SbViewVolume::ortho",
                              "right coordinate (%f) should be larger than "
                              "left coordinate (%f). Swapping left/right.",
                              right,left);
    float tmp=right;
    right=left;
    left=tmp;
  }
  if (bottom>top) {
    SoDebugError::postWarning("SbViewVolume::ortho",
                              "top coordinate (%f) should be larger than "
                              "bottom coordinate (%f). Swapping bottom/top.",
                              top,bottom);
    float tmp=top;
    top=bottom;
    bottom=tmp;
  }
  if (nearval>farval) {
    SoDebugError::postWarning("SbViewVolume::ortho",
                              "far coordinate (%f) should be larger than near "
                              "coordinate (%f). Swapping near/far.",farval,nearval);
    float tmp=farval;
    farval=nearval;
    nearval=tmp;
  }
#endif // COIN_DEBUG

  this->type = SbViewVolume::ORTHOGRAPHIC;
  this->projectionpt.setValue(0.0f, 0.0f, 0.0f);
  this->projectiondir.setValue(0.0f, 0.0f, -1.0f);
  this->nearplanedistance = nearval;
  this->nearfardistance = farval - nearval;
  this->lowerleftfrust.setValue(left, bottom, -nearval);
  this->lowerrightfrust.setValue(right, bottom, -nearval);
  this->upperleftfrust.setValue(left, top, -nearval);
}

/*!
  Set up the view volume for perspective projections. The line of
  sight will be through origo along the negative z axis.

  TODO: document relationship between field-of-view, aspect ratio and
  span of near clipping plane. Should be illustrated.

  \sa ortho().
*/
void
SbViewVolume::perspective(float fovy, float aspect,
                          float nearval, float farval)
{
#if COIN_DEBUG
  if (fovy<0.0f || fovy > M_PI) {
    SoDebugError::postWarning("SbViewVolume::perspective",
                              "Field of View 'fovy' (%f) is out of bounds "
                              "[0,PI]. Clamping to be within bounds.",fovy);
    if (fovy<0.0f) fovy=0.0f;
    else if (fovy>M_PI) fovy=M_PI;
  }

  if (aspect<0.0f) {
    SoDebugError::postWarning("SbViewVolume::perspective",
                              "Aspect ratio 'aspect' (%d) should be >=0.0f. "
                              "Clamping to 0.0f.",aspect);
    aspect=0.0f;
  }

  if (nearval>farval) {
    SoDebugError::postWarning("SbViewVolume::perspective",
                              "far coordinate (%f) should be larger than "
                              "near coordinate (%f). Swapping near/far.",
                              farval,nearval);
    float tmp=farval;
    farval=nearval;
    nearval=tmp;
  }
#endif // COIN_DEBUG

  this->type = SbViewVolume::PERSPECTIVE;
  this->projectionpt.setValue(0.0f, 0.0f, 0.0f);
  this->projectiondir.setValue(0.0f, 0.0f, -1.0f);
  this->nearplanedistance = nearval;
  this->nearfardistance = farval - nearval;

  float top = nearval * float(tan(fovy/2.0f));
  float bottom = -top;
  float left = bottom * aspect;
  float right = -left;

  this->lowerleftfrust.setValue(left, bottom, -nearval);
  this->lowerrightfrust.setValue(right, bottom, -nearval);
  this->upperleftfrust.setValue(left, top, -nearval);
}

/*!
  Rotate the direction which the camera is pointing in.

  \sa translateCamera().
 */
void
SbViewVolume::rotateCamera(const SbRotation& q)
{
  SbMatrix mat;
  mat.setRotate(q);

  mat.multDirMatrix(this->projectiondir, this->projectiondir);

  if(this->type == SbViewVolume::ORTHOGRAPHIC) {
    mat.multVecMatrix(this->lowerleftfrust, this->lowerleftfrust);
    mat.multVecMatrix(this->lowerrightfrust, this->lowerrightfrust);
    mat.multVecMatrix(this->upperleftfrust, this->upperleftfrust);
  }
  // SbViewVolume::PERSPECTIVE
  else {
    mat.multVecMatrix(this->lowerleftfrust - this->projectionpt,
                      this->lowerleftfrust);
    this->lowerleftfrust += this->projectionpt;
    mat.multVecMatrix(this->lowerrightfrust - this->projectionpt,
                      this->lowerrightfrust);
    this->lowerrightfrust += this->projectionpt;
    mat.multVecMatrix(this->upperleftfrust - this->projectionpt,
                      this->upperleftfrust);
    this->upperleftfrust += this->projectionpt;
  }
}

/*!
  Translate the camera position of the view volume.

  \sa rotateCamera().
 */
void
SbViewVolume::translateCamera(const SbVec3f& v)
{
  this->projectionpt += v;
  this->lowerleftfrust += v;
  this->lowerrightfrust += v;
  this->upperleftfrust += v;
}

/*!
  Return the vector pointing from the center of the view volume towards
  the camera. This is just the vector pointing in the opposite direction
  of \a getProjectionDirection().

  \sa getProjectionDirection().
 */
SbVec3f
SbViewVolume::zVector(void) const
{
  return -this->projectiondir;
}

/*!
  Return a copy SbViewVolume with narrowed depth by supplying parameters
  for new near and far clipping planes. \a near and \a far should be
  given in terms of \a zVector(), i.e. \a near must be a larger value
  than \a far.

  \sa zVector().
 */
SbViewVolume
SbViewVolume::zNarrow(float nearval, float farval) const
{
#if COIN_DEBUG
  if (nearval>farval) {
    SoDebugError::postWarning("SbViewVolume::zNarrow",
                              "far coordinate (%f) should be larger than "
                              "near coordinate (%f). Swapping near/far.",
                              farval,nearval);
    float tmp=farval;
    farval=nearval;
    nearval=tmp;
  }
#endif // COIN_DEBUG

  // TODO
  assert(0);
  return *this;

#if 0 // not working yet
  SbViewVolume nvw = *this;

  float n = SbMin(1.0f, nearval);
  float f = SbMax(0.0f, farval);

  nvw.nearplanedistance = n;
  nvw.nearfardistance = -(f-n) * this->nearfardistance;

  float oldvecz = this->getDepth() + this->getNearDist();

  nvw.lowerleftfrust +=
    (-nvw.nearplanedistance +
     ((nvw.nearplanedistance + nvw.nearfardistance) - oldvecz)) *
     nvw.projectiondir;

  nvw.lowerrightfrust +=
    (-nvw.nearplanedistance +
     ((nvw.nearplanedistance + nvw.nearfardistance) - oldvecz)) *
     nvw.projectiondir;

  nvw.upperleftfrust +=
    (-nvw.nearplanedistance +
     ((nvw.nearplanedistance + nvw.nearfardistance) - oldvecz)) *
     nvw.projectiondir;

  return nvw;

#endif // 0
}

/*!
  Scale width and height of viewing frustum by the given ratio around the
  projection plane center axis.

  \sa scaleWidth(), scaleHeight().
 */
void
SbViewVolume::scale(float factor)
{
#if COIN_DEBUG
  if (factor<0.0f) {
    SoDebugError::postWarning("SbViewVolume::scale",
                              "Scale factor (%f) should be >=0.0f. Clamping "
                              "to 0.0f.",factor);
    factor=0.0f;
  }
#endif // COIN_DEBUG

  this->scaleWidth(factor);
  this->scaleHeight(factor);
}

/*!
  Scale width of viewing frustum by the given ratio around the vertical
  center axis in the projection plane.

  \sa scale(), scaleHeight().
 */
void
SbViewVolume::scaleWidth(float ratio)
{
#if COIN_DEBUG
  if (ratio<0.0f) {
    SoDebugError::postWarning("SbViewVolume::scaleWidth",
                              "Scale factor (%f) should be >=0.0f. "
                              "Clamping to 0.0f.",ratio);
    ratio=0.0f;
  }
#endif // COIN_DEBUG

  float w = this->getWidth();
  float neww = w * ratio;
  float wdiff = (neww - w)/2.0f;

  SbVec3f xvec = this->lowerrightfrust - this->lowerleftfrust;
  xvec.normalize();
  SbVec3f diffvec = xvec * wdiff;

  this->lowerleftfrust -= diffvec;
  this->upperleftfrust -= diffvec;
  this->lowerrightfrust += diffvec;
}

/*!
  Scale height of viewing frustum by the given ratio around the horizontal
  center axis in the projection plane.

  \sa scale(), scaleWidth().
 */
void
SbViewVolume::scaleHeight(float ratio)
{
#if COIN_DEBUG
  if (ratio<0.0f) {
    SoDebugError::postWarning("SbViewVolume::scaleHeight",
                              "Scale factor (%f) should be >=0.0f. "
                              "Clamping to 0.0f.",ratio);
    ratio=0.0f;
  }
#endif // COIN_DEBUG

  float h = this->getHeight();
  float newh = h * ratio;
  float hdiff = (newh - h)/2.0f;

  SbVec3f upvec = this->upperleftfrust - this->lowerleftfrust;
  upvec.normalize();
  SbVec3f diffvec = upvec * hdiff;

  this->lowerleftfrust -= diffvec;
  this->upperleftfrust += diffvec;
  this->lowerrightfrust -= diffvec;
}

/*!
  Return current view volume projection type, which can be
  either \a ORTHOGRAPHIC or \a PERSPECTIVE.

  \sa SbViewVolume::ProjectionType
 */
SbViewVolume::ProjectionType
SbViewVolume::getProjectionType(void) const
{
  return this->type;
}

/*!
  Returns coordinates of center point in the projection plane.
 */
const SbVec3f&
SbViewVolume::getProjectionPoint(void) const
{
  return this->projectionpt;
}

/*!
  Returns the direction of projection, i.e. the direction the camera is
  pointing.

  \sa getNearDist().
 */
const SbVec3f&
SbViewVolume::getProjectionDirection(void) const
{
  return this->projectiondir;
}

/*!
  Returns distance from projection plane to near clipping plane.

  \sa getProjectionDirection().
 */
float
SbViewVolume::getNearDist(void) const
{
  return this->nearplanedistance;
}

/*!
  Returns width of viewing frustum in the projection plane.

  \sa getHeight(), getDepth().
*/
float
SbViewVolume::getWidth(void) const
{
  return (this->lowerrightfrust - this->lowerleftfrust).length();
}

/*!
  Returns height of viewing frustum in the projection plane.

  \sa getWidth(), getDepth().
*/
float
SbViewVolume::getHeight(void) const
{
  return (this->upperleftfrust - this->lowerleftfrust).length();
}

/*!
  Returns depth of viewing frustum, i.e. the distance from the near clipping
  plane to the far clipping plane.

  \sa getWidth(), getHeight().
 */
float
SbViewVolume::getDepth(void) const
{
  return this->nearfardistance;
}

/*!
  Private method to make a matrix for orthogonal parallel projection.
 */
SbMatrix
SbViewVolume::getOrthoProjection(const float left, const float right,
                                 const float bottom, const float top,
                                 const float nearval, const float farval)
{
#if COIN_DEBUG
  if (left == right || bottom == top || nearval == farval) {
    SoDebugError::postWarning("SbViewVolume::getOrthoProjection",
                              "invalid frustum: <%f, %f> <%f, %f> <%f, %f>",
                              left, right, bottom, top, nearval, farval);
    return SbMatrix::identity();
  }
#endif // COIN_DEBUG

  SbMatrix proj;

  // Projection matrix. From the "OpenGL Programming Guide, release 1",
  // Appendix G (but with row-major mode).

  proj[0][0] = 2.0f/(right-left);
  proj[0][1] = 0.0f;
  proj[0][2] = 0.0f;
  proj[0][3] = 0.0f;
  proj[1][0] = 0.0f;
  proj[1][1] = 2.0f/(top-bottom);
  proj[1][2] = 0.0f;
  proj[1][3] = 0.0f;
  proj[2][0] = 0.0f;
  proj[2][1] = 0.0f;
  proj[2][2] = -2.0f/(farval-nearval);
  proj[2][3] = 0.0f;
  proj[3][0] = -(right+left)/(right-left);
  proj[3][1] = -(top+bottom)/(top-bottom);
  proj[3][2] = -(farval+nearval)/(farval-nearval);
  proj[3][3] = 1.0f;

  return proj;
}


/*!
  Private method to make a matrix for perspective projection.
 */
SbMatrix
SbViewVolume::getPerspectiveProjection(const float left, const float right,
                                       const float bottom, const float top,
                                       const float nearval, const float farval)
{
#if COIN_DEBUG
  if (left == right || bottom == top || nearval == farval) {
    SoDebugError::postWarning("SbViewVolume::getPerspectiveProjection",
                              "invalid frustum: <%f, %f> <%f, %f> <%f, %f>",
                              left, right, bottom, top, nearval, farval);
    return SbMatrix::identity();
  }
#endif // COIN_DEBUG

  SbMatrix proj;

  // Projection matrix. From the "OpenGL Programming Guide, release 1",
  // Appendix G (but with row-major mode).

  proj[0][0] = 2.0f*nearval/(right-left);
  proj[0][1] = 0.0f;
  proj[0][2] = 0.0f;
  proj[0][3] = 0.0f;
  proj[1][0] = 0.0f;
  proj[1][1] = 2.0f*nearval/(top-bottom);
  proj[1][2] = 0.0f;
  proj[1][3] = 0.0f;
  proj[2][0] = (right+left)/(right-left);
  proj[2][1] = (top+bottom)/(top-bottom);
  proj[2][2] = -(farval+nearval)/(farval-nearval);
  proj[2][3] = -1.0f;
  proj[3][0] = 0.0f;
  proj[3][1] = 0.0f;
  proj[3][2] = -2.0f*farval*nearval/(farval-nearval);
  proj[3][3] = 0.0f;

  return proj;
}

/*!
  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized compile.
 */
void
SbViewVolume::print(FILE * fp) const
{
#if COIN_DEBUG
  fprintf( fp, "  projtype: %d\n", (int) this->getProjectionType() );
  fprintf( fp, "  projpt:   " );
  this->getProjectionPoint().print(fp);
  fprintf( fp, "\n" );
  fprintf( fp, "  projdir:  " );
  this->getProjectionDirection().print(fp);
  fprintf( fp, "\n" );
  fprintf( fp, "  neardist: %f\n", this->getNearDist() );
  fprintf( fp, "  width:    %f\n", this->getWidth() );
  fprintf( fp, "  height:   %f\n", this->getHeight() );
  fprintf( fp, "  depth:    %f\n", this->getDepth() );
  fprintf( fp, "    llf:    " );
  this->lowerleftfrust.print(fp);
  fprintf( fp, "\n" );
  fprintf( fp, "    lrf:    " );
  this->lowerrightfrust.print(fp);
  fprintf( fp, "\n" );
  fprintf( fp, "    ulf:    " );
  this->upperleftfrust.print(fp);
  fprintf( fp, "\n" );
#endif // COIN_DEBUG
}

/*!
  Returns the six planes defining the view volume in the following
  order: left, bottom, right, top, near, far. Plane normals are
  directed into the view volume.

  This method is an extension for Coin, and is not available in the
  original Open Inventor.
*/
void
SbViewVolume::getViewVolumePlanes(SbPlane planes[6]) const
{
  SbVec3f far_ll;
  SbVec3f far_lr;
  SbVec3f far_ul;
  SbVec3f far_ur;

  float depth = this->nearplanedistance + this->nearfardistance;
  SbVec3f near_ur = this->upperleftfrust + (this->lowerrightfrust-this->lowerleftfrust);

  if (this->type == PERSPECTIVE) {
    SbVec3f dir;
    dir = this->lowerleftfrust - this->projectionpt;
    dir.normalize();
    far_ll = this->projectionpt + dir * depth / dir.dot(this->projectiondir);

    dir = this->lowerrightfrust - this->projectionpt;
    dir.normalize();
    far_lr = this->projectionpt + dir * depth / dir.dot(this->projectiondir);

    dir = this->upperleftfrust - this->projectionpt;
    dir.normalize();
    far_ul = this->projectionpt + dir * depth / dir.dot(this->projectiondir);

    dir = near_ur - this->projectionpt;
    dir.normalize();
    far_ur = this->projectionpt + dir * depth / dir.dot(this->projectiondir);
  }
  else {
    far_ll = this->lowerleftfrust + this->projectiondir * depth;
    far_lr = this->lowerrightfrust + this->projectiondir * depth;
    far_ul = this->upperleftfrust + this->projectiondir * depth;
    far_ur = near_ur + this->projectiondir * depth;
  }
  planes[0] = SbPlane(this->upperleftfrust, this->lowerleftfrust, far_ll);  // left
  planes[1] = SbPlane(this->lowerleftfrust, this->lowerrightfrust, far_lr); // bottom
  planes[2] = SbPlane(this->lowerrightfrust, near_ur, far_ur); // right
  planes[3] = SbPlane(near_ur, this->upperleftfrust, far_ul); // top
  planes[4] = SbPlane(this->upperleftfrust, near_ur, this->lowerrightfrust); // near
  planes[5] = SbPlane(far_ll, far_lr, far_ur); // far
}

/*!
  Transform the viewing volume by \a matrix.
 */
void
SbViewVolume::transform(const SbMatrix & matrix)
{
  matrix.multDirMatrix(this->projectiondir, this->projectiondir);

  if(this->type == SbViewVolume::ORTHOGRAPHIC) {
    matrix.multVecMatrix(this->lowerleftfrust, this->lowerleftfrust);
    matrix.multVecMatrix(this->lowerrightfrust, this->lowerrightfrust);
    matrix.multVecMatrix(this->upperleftfrust, this->upperleftfrust);
  }
  // SbViewVolume::PERSPECTIVE
  else {
    matrix.multDirMatrix(this->lowerleftfrust - this->projectionpt,
                         this->lowerleftfrust);
    matrix.multDirMatrix(this->lowerrightfrust - this->projectionpt,
                         this->lowerrightfrust);
    matrix.multDirMatrix(this->upperleftfrust - this->projectionpt,
                         this->upperleftfrust);
    matrix.multVecMatrix(this->projectionpt, this->projectionpt);

    this->lowerleftfrust += this->projectionpt;
    this->lowerrightfrust += this->projectionpt;
    this->upperleftfrust += this->projectionpt;
  }
}

/*!
  Returns the view up vector for this view volume. It's a vector
  which is perpendicular to the projection direction, and parallel and
  oriented in the same direction as the vector from the lower left
  corner to the upper left corner of the near plane.
*/
SbVec3f
SbViewVolume::getViewUp(void) const
{
  SbVec3f v = this->upperleftfrust - this->lowerleftfrust;
  v.normalize();
  return v;
}
