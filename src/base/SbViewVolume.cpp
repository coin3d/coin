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
  \class SbViewVolume SbLinear.h Inventor/SbLinear.h
  \brief The SbViewVolume class is a viewing volume in 3D space.
  \ingroup base

  This class contains the necessary information for storing a view
  volume.  It has methods for projection of primitives from or into
  the 3D volume, doing camera transforms, view volume transforms etc.

  \sa SbViewportRegion.
*/

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

#include <coindefs.h> // COIN_OBSOLETED()
#include <assert.h>

/*!
  \enum SbViewVolume::ProjectionType

  An SbViewVolume instance can represent either an orthogonal projection
  volume or a perspective projection volume.

  \sa ortho(), perspective(), getProjectionType().
*/

/*!
  \var SbViewVolume::ProjectionType SbViewVolume::ORTHOGRAPHIC
  Orthographic projection.
*/

/*!
  \var SbViewVolume::ProjectionType SbViewVolume::PERSPECTIVE
  Perspective projection.
*/

/*!
  \var SbViewVolume::ProjectionType SbViewVolume::type
  \internal
*/

/*!
  \var SbVec3f SbViewVolume::projPoint
  \internal
*/

/*!
  \var SbVec3f SbViewVolume::projDir
  \internal
*/

/*!  
  \var float SbViewVolume::nearDist
  \internal
*/

/*!
  \var float SbViewVolume::nearToFar
  \internal
*/

/*!
  \var SbVec3f SbViewVolume::llf
  \internal
*/

/*!
  \var SbVec3f SbViewVolume::lrf
  \internal
*/

/*! 
  \var SbVec3f SbViewVolume::ulf
  \internal
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
  SbVec3f upvec = this->ulf - this->llf;
#if COIN_DEBUG
  if (upvec == SbVec3f(0.0f, 0.0f, 0.0f)) {
    SoDebugError::postWarning("SbViewVolume::getMatrices",
                              "empty frustum!");
    affine = SbMatrix::identity();
    proj = SbMatrix::identity();
    return;
  }
#endif // COIN_DEBUG
  SbVec3f rightvec = this->lrf - this->llf;

  // store width and height (needed to generate projection matrix)
  float height = upvec.normalize();
  float width = rightvec.normalize();

  // build matrix that will transform into camera coordinate system
  SbMatrix mat;
  mat[0][0] = rightvec[0];
  mat[0][1] = rightvec[1];
  mat[0][2] = rightvec[2];
  mat[0][3] = 0.0f;

  mat[1][0] = upvec[0];
  mat[1][1] = upvec[1];
  mat[1][2] = upvec[2];
  mat[1][3] = 0.0f;

  mat[2][0] = -this->projDir[0];
  mat[2][1] = -this->projDir[1];
  mat[2][2] = -this->projDir[2];
  mat[2][3] = 0.0f;

  mat[3][0] = this->projPoint[0];
  mat[3][1] = this->projPoint[1];
  mat[3][2] = this->projPoint[2];
  mat[3][3] = 1.0f;

  // the affine matrix is the inverse of the camera coordinate system
  affine = mat.inverse();

  float l = -width * 0.5f;
  float r = width * 0.5f;
  float t = height * 0.5f;
  float b = - height * 0.5f;
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
    SbRotation(this->projDir, SbVec3f(0.0f, 0.0f, -1.0f));

  // Combine transforms.
  SbMatrix mat, tmp;
  mat.setTranslate(-this->projPoint);
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

  SbVec3f dx = this->lrf - this->llf;
  SbVec3f dy = this->ulf - this->llf;

#if COIN_DEBUG
  if (dx.sqrLength() == 0.0f || dy.sqrLength() == 0.0f) {
    SoDebugError::postWarning("SbViewVolume::projectPointToLine",
                              "invalid frustum");
    return;
  }
#endif // COIN_DEBUG

  line0 = this->llf + dx*pt[0] + dy*pt[1];
  SbVec3f dir;
  if (this->type == PERSPECTIVE) {
    dir = line0 - this->projPoint;
    dir.normalize();
    line1 = line0 + dir * this->getDepth() / dir.dot(this->projDir);
  }
  else {
    dir = this->projDir;
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

  The returned coordinates (\a dst) are normalized to be in range [0, 1].
*/
void
SbViewVolume::projectToScreen(const SbVec3f& src, SbVec3f& dst) const
{
  this->getMatrix().multVecMatrix(src, dst);
  
  // coordinates are in range [-1, 1], normalize to [0,1]
  dst *= 0.5f;
  dst += SbVec3f(0.5f, 0.5f, 0.5f);
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
  return SbPlane(-this->projDir,
                 this->projPoint + distFromEye * this->projDir);
}

/*!
  Returns the point on the center line-of-sight from the camera position
  with the given distance.

  \sa getPlane()
 */
SbVec3f
SbViewVolume::getSightPoint(const float distFromEye) const
{
  return this->projPoint + this->projDir * distFromEye;
}

/*!
  Return the 3D point which projects to \a normPoint and lies on the
  plane perpendicular to the camera direction and \a distFromEye
  distance away from the camera position.

  \a normPoint should be given in normalized coordinates, where the
  visible render canvas is covered by the range [0.0, 1.0].
 */
SbVec3f
SbViewVolume::getPlanePoint(const float distFromEye,
                            const SbVec2f & normPoint) const
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
      this->llf +
      (this->lrf - this->llf) * normPoint[0] +
      (this->ulf - this->llf) * normPoint[1];
    dvec -= this->getProjectionPoint();
    dvec.normalize();

    // Distance to point.
    float d = distFromEye/dvec.dot(this->getProjectionDirection());

    volpt = d * dvec + this->getProjectionPoint();
  }

  return volpt;
}

/*!
  This method is obsoleted in Coin. Let us know if you need it, and
  we'll consider implementing it.
 */
SbRotation
SbViewVolume::getAlignRotation(SbBool rightangleonly) const
{
  COIN_OBSOLETED();
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
    SbVec3f rightvec = this->lrf - this->llf;
    return (normRadius * rightvec).length();
  }
  else {
    // Find screen space coordinates of sphere center point and tangent
    // point.
    SbVec3f center_scr;
    this->projectToScreen(worldCenter, center_scr);
    center_scr[0] += normRadius;

    // Vectors spanning the projection plane.
    SbVec3f upvec = this->ulf - this->llf;
    SbVec3f rightvec = this->lrf - this->llf;

    // Find projection plane point for the sphere tangent touch point,
    // which is then used to define the sphere tangent line.
    SbVec3f ppp =
      this->llf + center_scr[0] * rightvec + center_scr[1] * upvec;
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
#if COIN_DEBUG && 0 // debug test disabled, 2001-02-16, pederb
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

  SbVec3f xvec = this->lrf - this->llf;
  xvec.normalize();
  SbVec3f yvec = this->ulf - this->llf;
  yvec.normalize();

  nvw.ulf = nvw.llf + (xvec * left * w + yvec * top * h);
  nvw.lrf =
    nvw.llf + (xvec * right * w + yvec * bottom * h);
  nvw.llf += xvec * left * w + yvec * bottom * h;

  return nvw;
}

/*!

  Returns a narrowed version of the view volume which is within the
  given [0, 1] normalized coordinates. The box x and y coordinates are
  taken to be corner points of a normalized "view window" on the near
  clipping plane. The box z coordinates are used to adjust the near
  and far clipping planes, and should be relative to the current
  clipping planes. A value of 1.0 is at the current near plane. A
  value of 0.0 is at the current far plane.
*/
SbViewVolume
SbViewVolume::narrow(const SbBox3f & box) const
{
  const SbVec3f & bmin = box.getMin();
  const SbVec3f & bmax = box.getMax();
  return this->narrow(bmin[0], bmin[1], bmax[0], bmax[1]).zNarrow(bmax[2], bmin[2]);
}

// FIXME: bitmap-illustration for function doc which shows how the
// frustum is set up wrt the input arguments. 20010919 mortene.
/*!
  Set up the view volume as a rectangular box for orthographic
  parallel projections. The line of sight will be along the negative
  z axis, through the center of the plane defined by the point
  <(right+left)/2, (top+bottom)/2, 0>.

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
  this->projPoint.setValue(0.0f, 0.0f, 0.0f);
  this->projDir.setValue(0.0f, 0.0f, -1.0f);
  this->nearDist = nearval;
  this->nearToFar = farval - nearval;
  this->llf.setValue(left, bottom, -nearval);
  this->lrf.setValue(right, bottom, -nearval);
  this->ulf.setValue(left, top, -nearval);
}

// FIXME: bitmap-illustration for function doc which shows how the
// frustum is set up wrt the input arguments. 20010919 mortene.
/*!
  Set up the view volume for perspective projections. The line of
  sight will be through origo along the negative z axis.

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
  this->projPoint.setValue(0.0f, 0.0f, 0.0f);
  this->projDir.setValue(0.0f, 0.0f, -1.0f);
  this->nearDist = nearval;
  this->nearToFar = farval - nearval;

  float top = nearval * float(tan(fovy/2.0f));
  float bottom = -top;
  float left = bottom * aspect;
  float right = -left;

  this->llf.setValue(left, bottom, -nearval);
  this->lrf.setValue(right, bottom, -nearval);
  this->ulf.setValue(left, top, -nearval);
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

  mat.multDirMatrix(this->projDir, this->projDir);

  if(this->type == SbViewVolume::ORTHOGRAPHIC) {
    mat.multVecMatrix(this->llf, this->llf);
    mat.multVecMatrix(this->lrf, this->lrf);
    mat.multVecMatrix(this->ulf, this->ulf);
  }
  // SbViewVolume::PERSPECTIVE
  else {
    mat.multVecMatrix(this->llf - this->projPoint,
                      this->llf);
    this->llf += this->projPoint;
    mat.multVecMatrix(this->lrf - this->projPoint,
                      this->lrf);
    this->lrf += this->projPoint;
    mat.multVecMatrix(this->ulf - this->projPoint,
                      this->ulf);
    this->ulf += this->projPoint;
  }
}

/*!
  Translate the camera position of the view volume.

  \sa rotateCamera().
 */
void
SbViewVolume::translateCamera(const SbVec3f& v)
{
  this->projPoint += v;
  this->llf += v;
  this->lrf += v;
  this->ulf += v;
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
  return -this->projDir;
}

/*!
  Return a copy SbViewVolume with narrowed depth by supplying parameters
  for new near and far clipping planes.

  \a nearval and \farval should be relative to the current clipping
  planes. A value of 1.0 is at the current near plane. A value of
  0.0 is at the current far plane.

  \sa zVector().
*/
SbViewVolume
SbViewVolume::zNarrow(float nearval, float farval) const
{
  SbViewVolume narrowed = *this;

  narrowed.nearDist = this->nearDist + (1.0f - nearval) * this->nearToFar;
  narrowed.nearToFar = this->nearDist + this->nearToFar * (1.0f - farval);

  SbVec3f dummy;
  this->getPlaneRectangle(narrowed.nearDist-this->nearDist,
                          narrowed.llf,
                          narrowed.lrf,
                          narrowed.ulf,
                          dummy);
  return narrowed;
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

  SbVec3f xvec = this->lrf - this->llf;
  xvec.normalize();
  SbVec3f diffvec = xvec * wdiff;

  this->llf -= diffvec;
  this->ulf -= diffvec;
  this->lrf += diffvec;
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

  SbVec3f upvec = this->ulf - this->llf;
  upvec.normalize();
  SbVec3f diffvec = upvec * hdiff;

  this->llf -= diffvec;
  this->ulf += diffvec;
  this->lrf -= diffvec;
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
  return this->projPoint;
}

/*!
  Returns the direction of projection, i.e. the direction the camera is
  pointing.

  \sa getNearDist().
 */
const SbVec3f&
SbViewVolume::getProjectionDirection(void) const
{
  return this->projDir;
}

/*!
  Returns distance from projection plane to near clipping plane.

  \sa getProjectionDirection().
 */
float
SbViewVolume::getNearDist(void) const
{
  return this->nearDist;
}

/*!
  Returns width of viewing frustum in the projection plane.

  \sa getHeight(), getDepth().
*/
float
SbViewVolume::getWidth(void) const
{
  return (this->lrf - this->llf).length();
}

/*!
  Returns height of viewing frustum in the projection plane.

  \sa getWidth(), getDepth().
*/
float
SbViewVolume::getHeight(void) const
{
  return (this->ulf - this->llf).length();
}

/*!
  Returns depth of viewing frustum, i.e. the distance from the near clipping
  plane to the far clipping plane.

  \sa getWidth(), getHeight().
 */
float
SbViewVolume::getDepth(void) const
{
  return this->nearToFar;
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
  this->llf.print(fp);
  fprintf( fp, "\n" );
  fprintf( fp, "    lrf:    " );
  this->lrf.print(fp);
  fprintf( fp, "\n" );
  fprintf( fp, "    ulf:    " );
  this->ulf.print(fp);
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

  this->getPlaneRectangle(this->nearToFar, far_ll, far_lr, far_ul, far_ur);
  SbVec3f near_ur = this->ulf + (this->lrf-this->llf);

  planes[0] = SbPlane(this->ulf, this->llf, far_ll);  // left
  planes[1] = SbPlane(this->llf, this->lrf, far_lr); // bottom
  planes[2] = SbPlane(this->lrf, near_ur, far_ur); // right
  planes[3] = SbPlane(near_ur, this->ulf, far_ul); // top
  planes[4] = SbPlane(this->ulf, near_ur, this->lrf); // near
  planes[5] = SbPlane(far_ll, far_lr, far_ur); // far
}

/*!
  Transform the viewing volume by \a matrix.
 */
void
SbViewVolume::transform(const SbMatrix & matrix)
{
  matrix.multDirMatrix(this->projDir, this->projDir);

  if(this->type == SbViewVolume::ORTHOGRAPHIC) {
    matrix.multVecMatrix(this->llf, this->llf);
    matrix.multVecMatrix(this->lrf, this->lrf);
    matrix.multVecMatrix(this->ulf, this->ulf);
  }
  // SbViewVolume::PERSPECTIVE
  else {
    matrix.multVecMatrix(this->llf, this->llf);
    matrix.multVecMatrix(this->lrf, this->lrf);
    matrix.multVecMatrix(this->ulf, this->ulf);
    matrix.multVecMatrix(this->projPoint, this->projPoint);
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
  SbVec3f v = this->ulf - this->llf;
  (void) v.normalize();
  return v;
}

//
// Returns the four points defining the view volume rectangle at the specified distance
// from the near plane, towards the far plane.
//
void
SbViewVolume::getPlaneRectangle(const float distance, SbVec3f & lowerleft,
                                SbVec3f & lowerright,
                                SbVec3f & upperleft,
                                SbVec3f & upperright) const
{
  SbVec3f near_ur = this->ulf + (this->lrf-this->llf);

  if (this->type == PERSPECTIVE) {
    float depth = this->nearDist + distance;
    SbVec3f dir;
    dir = this->llf - this->projPoint;
    dir.normalize();
    lowerleft = this->projPoint + dir * depth / dir.dot(this->projDir);

    dir = this->lrf - this->projPoint;
    dir.normalize();
    lowerright = this->projPoint + dir * depth / dir.dot(this->projDir);

    dir = this->ulf - this->projPoint;
    dir.normalize();
    upperleft = this->projPoint + dir * depth / dir.dot(this->projDir);

    dir = near_ur - this->projPoint;
    dir.normalize();
    upperright = this->projPoint + dir * depth / dir.dot(this->projDir);
  }
  else {
    lowerleft = this->llf + this->projDir * distance;
    lowerright = this->lrf + this->projDir * distance;
    upperleft = this->ulf + this->projDir * distance;
    upperright = near_ur + this->projDir * distance;
  }
}
