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
  \class SbDPViewVolume SbLinear.h Inventor/SbLinear.h
  \brief The SbDPViewVolume class is a double precision viewing volume in 3D space.
  \ingroup base

  This class contains the necessary information for storing a view
  volume.  It has methods for projection of primitives from or into
  the 3D volume, doing camera transforms, view volume transforms etc.

  

  \sa SbViewportRegion.
*/

#include <Inventor/SbDPViewVolume.h>
#include <Inventor/SbViewVolume.h>
#include <Inventor/SbDPRotation.h>
#include <Inventor/SbDPLine.h>
#include <Inventor/SbLine.h>
#include <Inventor/SbDPMatrix.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbBox2f.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbVec2d.h>
#include <Inventor/SbVec3d.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <coindefs.h> // COIN_OBSOLETED()
#include <assert.h>

/*!
  \enum SbDPViewVolume::ProjectionType

  An SbDPViewVolume instance can represent either an orthogonal projection
  volume or a perspective projection volume.

  \sa ortho(), perspective(), getProjectionType().
*/

/*!
  \var SbDPViewVolume::ProjectionType SbDPViewVolume::ORTHOGRAPHIC
  Orthographic projection.
*/

/*!
  \var SbDPViewVolume::ProjectionType SbDPViewVolume::PERSPECTIVE
  Perspective projection.
*/

/*!
  \var SbDPViewVolume::ProjectionType SbDPViewVolume::type
  \internal
*/

/*!
  \var SbVec3d SbDPViewVolume::projPoint
  \internal
*/

/*!
  \var SbVec3d SbDPViewVolume::projDir
  \internal
*/

/*!  
  \var double SbDPViewVolume::nearDist
  \internal
*/

/*!
  \var double SbDPViewVolume::nearToFar
  \internal
*/

/*!
  \var SbVec3d SbDPViewVolume::llf
  \internal
*/

/*!
  \var SbVec3d SbDPViewVolume::lrf
  \internal
*/

/*! 
  \var SbVec3d SbDPViewVolume::ulf
  \internal
*/

//
// some convenience function for converting between single precision
// and double precision classes.
//
static SbVec3f 
to_sbvec3f(const SbVec3d & v)
{
  return SbVec3f((float)v[0], (float)v[1], (float)v[2]);
}

static SbVec3d 
to_sbvec3d(const SbVec3f & v)
{
  return SbVec3d((double)v[0], (double)v[1], (double)v[2]);
}

/*!
  Constructor. Note that the SbDPViewVolume instance will be uninitialized
  until you explicitly call \a ortho() or \a perspective().

  \sa ortho(), perspective().
 */
SbDPViewVolume::SbDPViewVolume(void)
{
}

/*!
  Destructor.
 */
SbDPViewVolume::~SbDPViewVolume(void)
{
}

// Orthographic projection matrix. From the "OpenGL Programming Guide,
// release 1", Appendix G (but with row-major mode).
static SbDPMatrix
get_perspective_projection(const double rightminusleft, const double rightplusleft,
                           const double topminusbottom, const double topplusbottom,
                           const double nearval, const double farval)
{
  SbDPMatrix proj;

  proj[0][0] = 2.0*nearval/rightminusleft;
  proj[0][1] = 0.0;
  proj[0][2] = 0.0;
  proj[0][3] = 0.0;
  proj[1][0] = 0.0;
  proj[1][1] = 2.0*nearval/topminusbottom;
  proj[1][2] = 0.0;
  proj[1][3] = 0.0;
  proj[2][0] = rightplusleft/rightminusleft;
  proj[2][1] = topplusbottom/topminusbottom;
  proj[2][2] = -(farval+nearval)/(farval-nearval);
  proj[2][3] = -1.0;
  proj[3][0] = 0.0;
  proj[3][1] = 0.0;
  proj[3][2] = -2.0*farval*nearval/(farval-nearval);
  proj[3][3] = 0.0;

  return proj;
}


// Perspective projection matrix. From the "OpenGL Programming Guide,
// release 1", Appendix G (but with row-major mode).
static SbDPMatrix
get_ortho_projection(const double rightminusleft, const double rightplusleft,
                     const double topminusbottom, const double topplusbottom,
                     const double nearval, const double farval)
{
  SbDPMatrix proj;
  proj[0][0] = 2.0/rightminusleft;
  proj[0][1] = 0.0;
  proj[0][2] = 0.0;
  proj[0][3] = 0.0;
  proj[1][0] = 0.0;
  proj[1][1] = 2.0/topminusbottom;
  proj[1][2] = 0.0;
  proj[1][3] = 0.0;
  proj[2][0] = 0.0;
  proj[2][1] = 0.0;
  proj[2][2] = -2.0/(farval-nearval);
  proj[2][3] = 0.0;
  proj[3][0] = -rightplusleft/rightminusleft;
  proj[3][1] = -topplusbottom/topminusbottom;
  proj[3][2] = -(farval+nearval)/(farval-nearval);
  proj[3][3] = 1.0;

  return proj;

}


/*!
  Returns the view volume's affine matrix and projection matrix.

  \sa getMatrix(), getCameraSpaceMatrix()
 */
void
SbDPViewVolume::getMatrices(SbDPMatrix& affine, SbDPMatrix& proj) const
{
  SbVec3d upvec = this->ulf - this->llf;
#if COIN_DEBUG
  if (upvec == SbVec3d(0.0, 0.0, 0.0)) {
    SoDebugError::postWarning("SbDPViewVolume::getMatrices",
                              "empty frustum!");
    affine = SbDPMatrix::identity();
    proj = SbDPMatrix::identity();
    return;
  }
#endif // COIN_DEBUG
  SbVec3d rightvec = this->lrf - this->llf;

  (void) upvec.normalize();
  (void) rightvec.normalize();

  // build matrix that will transform into camera coordinate system
  SbDPMatrix mat;
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

  // rotate frustum points back to an axis-aligned view volume to
  // calculate parameters for the projection matrix
  SbVec3d nlrf, nllf, nulf;

  affine.multDirMatrix(this->lrf, nlrf);
  affine.multDirMatrix(this->llf, nllf);
  affine.multDirMatrix(this->ulf, nulf);

  double rml = nlrf[0] - nllf[0];
  double rpl = nlrf[0] + nllf[0];
  double tmb = nulf[1] - nllf[1];
  double tpb = nulf[1] + nllf[1];
  double n = this->getNearDist();
  double f = n + this->getDepth();

#if COIN_DEBUG
  if (rml <= 0.0f || tmb <= 0.0f || n >= f) {
    SoDebugError::postWarning("SbDPViewVolume::getMatrices",
                              "invalid frustum");
    proj = SbDPMatrix::identity();
    return;
  }
#endif // COIN_DEBUG


  if(this->type == SbDPViewVolume::ORTHOGRAPHIC)
    proj = get_ortho_projection(rml, rpl, tmb, tpb, n, f);
  else
    proj = get_perspective_projection(rml, rpl, tmb, tpb, n, f);
}

/*!
  Returns the combined affine and projection matrix.

  \sa getMatrices(), getCameraSpaceMatrix()
 */
SbDPMatrix
SbDPViewVolume::getMatrix(void) const
{
  SbDPMatrix affine, proj;
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
SbDPMatrix
SbDPViewVolume::getCameraSpaceMatrix(void) const
{
  // Find rotation of projection direction.
  SbDPRotation pdrot =
    SbDPRotation(this->projDir, SbVec3d(0.0f, 0.0f, -1.0f));

  // Combine transforms.
  SbDPMatrix mat, tmp;
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
SbDPViewVolume::projectPointToLine(const SbVec2d& pt, SbDPLine& line) const
{
  SbVec3d pt0, pt1;
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
SbDPViewVolume::projectPointToLine(const SbVec2d& pt,
                                   SbVec3d & line0, SbVec3d & line1) const
{
#if 0 // OBSOLETED, pederb 19991215. More efficient version below
  SbVec3d scr_n(pt[0], pt[1], -1.0f);
  SbVec3d scr_f(pt[0], pt[1], 1.0f);

  scr_n[0] -= 0.5f;
  scr_n[1] -= 0.5f;
  scr_f[0] -= 0.5f;
  scr_f[1] -= 0.5f;

  scr_n[0] *= 2.0f;
  scr_n[1] *= 2.0f;
  scr_f[0] *= 2.0f;
  scr_f[1] *= 2.0f;

  SbDPMatrix m = this->getMatrix().inverse();

  m.multVecMatrix(scr_n, line0);
  m.multVecMatrix(scr_f, line1);

#else // new, faster version

  SbVec3d dx = this->lrf - this->llf;
  SbVec3d dy = this->ulf - this->llf;

#if COIN_DEBUG
  if (dx.sqrLength() == 0.0f || dy.sqrLength() == 0.0f) {
    SoDebugError::postWarning("SbDPViewVolume::projectPointToLine",
                              "invalid frustum");
    return;
  }
#endif // COIN_DEBUG

  line0 = this->projPoint + this->llf + dx*pt[0] + dy*pt[1];
  SbVec3d dir;
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

  It is safe to let \a src and \dst be the same SbVec3d instance.
  
  The z-coordinate of \a dst is monotonically increasing for points
  closer to the far plane. Note however that this is not a linear
  relationship, the \a dst z-coordinate is calculated as follows:
  
  Orthogonal view:  DSTz = (-2 * SRCz - far - near) / (far - near),
  Perspective view:  DSTz = (-SRCz * (far - near) - 2*far*near) / (far - near)

  The returned coordinates (\a dst) are normalized to be in range [0, 1].
*/
void
SbDPViewVolume::projectToScreen(const SbVec3d& src, SbVec3d& dst) const
{
  this->getMatrix().multVecMatrix(src, dst);
  
  // coordinates are in range [-1, 1], normalize to [0,1]
  dst *= 0.5f;
  dst += SbVec3d(0.5f, 0.5f, 0.5f);
}

/*!
  Returns an SbPlane instance which has a normal vector in the opposite
  direction of which the camera is pointing. This means the
  plane will be parallel to the near and far clipping planes.

  \sa getSightPoint()
 */
SbPlane
SbDPViewVolume::getPlane(const double distFromEye) const
{
  return SbPlane(to_sbvec3f(-this->projDir),
                 to_sbvec3f(this->projPoint + distFromEye * this->projDir));
}

/*!
  Returns the point on the center line-of-sight from the camera position
  with the given distance.

  \sa getPlane()
 */
SbVec3d
SbDPViewVolume::getSightPoint(const double distFromEye) const
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
SbVec3d
SbDPViewVolume::getPlanePoint(const double distFromEye,
                            const SbVec2d & normPoint) const
{
  SbVec3d volpt;

  if(this->getProjectionType() == SbDPViewVolume::ORTHOGRAPHIC) {
    SbVec3d scr(normPoint[0], normPoint[1], -1.0f);

    scr[0] -= 0.5f;
    scr[1] -= 0.5f;
    scr[0] *= 2.0f;
    scr[1] *= 2.0f;

    SbDPMatrix m = this->getMatrix().inverse();
    m.multVecMatrix(scr, volpt);
    volpt += (distFromEye - this->getNearDist()) *
      this->getProjectionDirection();
  }
  else {
    // Find vector pointing in the direction of the normalized 2D
    // point.
    SbVec3d dvec =
      this->llf +
      (this->lrf - this->llf) * normPoint[0] +
      (this->ulf - this->llf) * normPoint[1];
    dvec.normalize();

    // Distance to point.
    double d = distFromEye/dvec.dot(this->getProjectionDirection());

    volpt = d * dvec + this->getProjectionPoint();
  }

  return volpt;
}

/*!
  This method is obsoleted in Coin. Let us know if you need it, and
  we'll consider implementing it.
 */
SbDPRotation
SbDPViewVolume::getAlignRotation(SbBool rightangleonly) const
{
  COIN_OBSOLETED();
  return SbDPRotation::identity();
}

/*!
  Given a sphere with center in \a worldCenter and an initial radius of \a 1.0,
  return the scale factor needed to make this sphere have a \a normRadius
  radius when projected onto the near clipping plane.
 */
double
SbDPViewVolume::getWorldToScreenScale(const SbVec3d& worldCenter,
                                    double normRadius) const
{
#if COIN_DEBUG
  if (normRadius < 0.0f) {
    SoDebugError::postWarning("SbDPViewVolume::getWorldToScreenScale",
                              "normRadius (%f) should be >=0.0f.", normRadius);
    return 1.0f;
  }
  if (this->getWidth() == 0.0f || this->getHeight() == 0.0f) {
    SoDebugError::postWarning("SbDPViewVolume::getWorldToScreenScale",
                              "invalid frustum <%f, %f>",
                              this->getWidth(), this->getHeight());
    return 1.0f;
  }
#endif // COIN_DEBUG

  if(this->getProjectionType() == SbDPViewVolume::ORTHOGRAPHIC) {
    SbVec3d rightvec = this->lrf - this->llf;
    return (normRadius * rightvec).length();
  }
  else {
    // Find screen space coordinates of sphere center point and tangent
    // point.
    SbVec3d center_scr;
    this->projectToScreen(worldCenter, center_scr);
    center_scr[0] += normRadius;

    // Vectors spanning the projection plane.
    SbVec3d upvec = this->ulf - this->llf;
    SbVec3d rightvec = this->lrf - this->llf;

    // Find projection plane point for the sphere tangent touch point,
    // which is then used to define the sphere tangent line.
    SbVec3d ppp = this->projPoint + 
      this->llf + center_scr[0] * rightvec + center_scr[1] * upvec;
    SbLine tl(to_sbvec3f(this->getProjectionPoint()), to_sbvec3f(ppp));

    // Define the plane which is cutting the sphere in half and is normal
    // to the camera direction.
    SbVec3d sphere_camera_vec = worldCenter - this->getProjectionPoint();
    SbPlane p = SbPlane(to_sbvec3f(sphere_camera_vec), to_sbvec3f(worldCenter));

    // Find tangent point of sphere.
    SbVec3f tangentpt;
    SbBool result = p.intersect(tl, tangentpt);
    assert(result != FALSE);

    // Return radius (which is equal to the scale factor, since we're
    // dealing with a unit sphere).
    return (to_sbvec3d(tangentpt) - worldCenter).length();
  }
}

/*!
  Projects the given box onto the projection plane and returns the
  normalized screen space it occupies.
 */
SbVec2d
SbDPViewVolume::projectBox(const SbBox3f& box) const
{
#if COIN_DEBUG
  if (box.isEmpty()) {
    SoDebugError::postWarning("SbDPViewVolume::projectBox",
                              "Box is empty.");
  }
#endif // COIN_DEBUG

  SbVec3d mincorner = to_sbvec3d(box.getMin());
  SbVec3d maxcorner = to_sbvec3d(box.getMax());
  SbBox2f span;

  for(int i=0; i < 2; i++) {
    for(int j=0; j < 2; j++) {
      for(int k=0; k < 2; k++) {
        SbVec3d corner(i ? mincorner[0] : maxcorner[0],
                       j ? mincorner[1] : maxcorner[1],
                       k ? mincorner[2] : maxcorner[2]);
        this->projectToScreen(corner, corner);
        span.extendBy(SbVec2f((float)corner[0], (float)corner[1]));
      }
    }
  }

  return SbVec2d(span.getMax()[0] - span.getMin()[0],
                 span.getMax()[1] - span.getMin()[1]);
}

/*!
  Returns a narrowed version of the view volume which is within the
  given [0, 1] normalized coordinates. The coordinates are taken to be
  corner points of a normalized "view window" on the near clipping
  plane.  I.e.:

  \code
  SbDPViewVolume view;
  view.ortho(0, 100, 0, 100, 0.1, 1000);
  view = view.narrow(0.25, 0.5, 0.75, 1.0);
  \endcode

  ..will give a view volume with corner points <25, 75> and <50, 100>.

  \sa scale(), scaleWidth(), scaleHeight()
 */
SbDPViewVolume
SbDPViewVolume::narrow(double left, double bottom,
                     double right, double top) const
{
#if COIN_DEBUG && 0 // debug test disabled, 2001-02-16, pederb
  if (left<0.0f) {
    SoDebugError::postWarning("SbDPViewVolume::narrow",
                              "left coordinate (%f) should be >=0.0f. "
                              "Clamping to 0.0f.",left);
    left=0.0f;
  }
  if (right>1.0f) {
    SoDebugError::postWarning("SbDPViewVolume::narrow",
                              "right coordinate (%f) should be <=1.0f. "
                              "Clamping to 1.0f.",right);
    right=1.0f;
  }
  if (bottom<0.0f) {
    SoDebugError::postWarning("SbDPViewVolume::narrow",
                              "bottom coordinate (%f) should be >=0.0f. "
                              "Clamping to 0.0f.",bottom);
    bottom=0.0f;
  }
  if (top>1.0f) {
    SoDebugError::postWarning("SbDPViewVolume::narrow",
                              "top coordinate (%f) should be <=1.0f. "
                              "Clamping to 1.0f.",top);
    top=1.0f;
  }
  if (left>right) {
    SoDebugError::postWarning("SbDPViewVolume::narrow",
                              "right coordinate (%f) should be larger than "
                              "left coordinate (%f). Swapping left/right.",
                              right,left);
    double tmp=right;
    right=left;
    left=tmp;
  }
  if (bottom>top) {
    SoDebugError::postWarning("SbDPViewVolume::narrow",
                              "top coordinate (%f) should be larger than "
                              "bottom coordinate (%f). Swapping top/bottom.",
                              top,bottom);
    double tmp=top;
    top=bottom;
    bottom=tmp;
  }
#endif // COIN_DEBUG

  SbDPViewVolume nvw = *this;

  double w = nvw.getWidth();
  double h = nvw.getHeight();

  SbVec3d xvec = this->lrf - this->llf;
  xvec.normalize();
  SbVec3d yvec = this->ulf - this->llf;
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
SbDPViewVolume
SbDPViewVolume::narrow(const SbBox3f & box) const
{
  SbVec3d bmin = to_sbvec3d(box.getMin());
  SbVec3d bmax = to_sbvec3d(box.getMax());
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
SbDPViewVolume::ortho(double left, double right,
                    double bottom, double top,
                    double nearval, double farval)
{
#if COIN_DEBUG
  if (left>right) {
    SoDebugError::postWarning("SbDPViewVolume::ortho",
                              "right coordinate (%f) should be larger than "
                              "left coordinate (%f). Swapping left/right.",
                              right,left);
    double tmp=right;
    right=left;
    left=tmp;
  }
  if (bottom>top) {
    SoDebugError::postWarning("SbDPViewVolume::ortho",
                              "top coordinate (%f) should be larger than "
                              "bottom coordinate (%f). Swapping bottom/top.",
                              top,bottom);
    double tmp=top;
    top=bottom;
    bottom=tmp;
  }
  if (nearval>farval) {
    SoDebugError::postWarning("SbDPViewVolume::ortho",
                              "far coordinate (%f) should be larger than near "
                              "coordinate (%f). Swapping near/far.",farval,nearval);
    double tmp=farval;
    farval=nearval;
    nearval=tmp;
  }
#endif // COIN_DEBUG

  this->type = SbDPViewVolume::ORTHOGRAPHIC;
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
SbDPViewVolume::perspective(double fovy, double aspect,
                          double nearval, double farval)
{
#if COIN_DEBUG
  if (fovy<0.0f || fovy > M_PI) {
    SoDebugError::postWarning("SbDPViewVolume::perspective",
                              "Field of View 'fovy' (%f) is out of bounds "
                              "[0,PI]. Clamping to be within bounds.",fovy);
    if (fovy<0.0f) fovy=0.0f;
    else if (fovy>M_PI) fovy=M_PI;
  }

  if (aspect<0.0f) {
    SoDebugError::postWarning("SbDPViewVolume::perspective",
                              "Aspect ratio 'aspect' (%d) should be >=0.0f. "
                              "Clamping to 0.0f.",aspect);
    aspect=0.0f;
  }

  if (nearval>farval) {
    SoDebugError::postWarning("SbDPViewVolume::perspective",
                              "far coordinate (%f) should be larger than "
                              "near coordinate (%f). Swapping near/far.",
                              farval,nearval);
    double tmp=farval;
    farval=nearval;
    nearval=tmp;
  }
#endif // COIN_DEBUG

  this->type = SbDPViewVolume::PERSPECTIVE;
  this->projPoint.setValue(0.0f, 0.0f, 0.0f);
  this->projDir.setValue(0.0f, 0.0f, -1.0f);
  this->nearDist = nearval;
  this->nearToFar = farval - nearval;

  double top = nearval * double(tan(fovy/2.0f));
  double bottom = -top;
  double left = bottom * aspect;
  double right = -left;

  this->llf.setValue(left, bottom, -nearval);
  this->lrf.setValue(right, bottom, -nearval);
  this->ulf.setValue(left, top, -nearval);
}

/*!
  Rotate the direction which the camera is pointing in.

  \sa translateCamera().
 */
void
SbDPViewVolume::rotateCamera(const SbDPRotation& q)
{
  SbDPMatrix mat;
  mat.setRotate(q);

  mat.multDirMatrix(this->projDir, this->projDir);
  mat.multDirMatrix(this->llf, this->llf);
  mat.multDirMatrix(this->lrf, this->lrf);
  mat.multDirMatrix(this->ulf, this->ulf);
}

/*!
  Translate the camera position of the view volume.

  \sa rotateCamera().
 */
void
SbDPViewVolume::translateCamera(const SbVec3d & v)
{
  this->projPoint += v;
}

/*!
  Return the vector pointing from the center of the view volume towards
  the camera. This is just the vector pointing in the opposite direction
  of \a getProjectionDirection().

  \sa getProjectionDirection().
 */
SbVec3d
SbDPViewVolume::zVector(void) const
{
  return -this->projDir;
}

/*!
  Return a copy SbDPViewVolume with narrowed depth by supplying parameters
  for new near and far clipping planes.

  \a nearval and \farval should be relative to the current clipping
  planes. A value of 1.0 is at the current near plane. A value of
  0.0 is at the current far plane.

  \sa zVector().
*/
SbDPViewVolume
SbDPViewVolume::zNarrow(double nearval, double farval) const
{
  SbDPViewVolume narrowed = *this;

  narrowed.nearDist = this->nearDist + (1.0f - nearval) * this->nearToFar;
  narrowed.nearToFar = this->nearDist + this->nearToFar * (1.0f - farval);

  SbVec3d dummy;
  this->getPlaneRectangle(narrowed.nearDist - this->nearDist,
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
SbDPViewVolume::scale(double factor)
{
#if COIN_DEBUG
  if (factor<0.0f) {
    SoDebugError::postWarning("SbDPViewVolume::scale",
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
SbDPViewVolume::scaleWidth(double ratio)
{
#if COIN_DEBUG
  if (ratio<0.0f) {
    SoDebugError::postWarning("SbDPViewVolume::scaleWidth",
                              "Scale factor (%f) should be >=0.0f. "
                              "Clamping to 0.0f.",ratio);
    ratio=0.0f;
  }
#endif // COIN_DEBUG

  double w = this->getWidth();
  double neww = w * ratio;
  double wdiff = (neww - w)/2.0f;

  SbVec3d xvec = this->lrf - this->llf;
  xvec.normalize();
  SbVec3d diffvec = xvec * wdiff;

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
SbDPViewVolume::scaleHeight(double ratio)
{
#if COIN_DEBUG
  if (ratio<0.0f) {
    SoDebugError::postWarning("SbDPViewVolume::scaleHeight",
                              "Scale factor (%f) should be >=0.0f. "
                              "Clamping to 0.0f.",ratio);
    ratio=0.0f;
  }
#endif // COIN_DEBUG

  double h = this->getHeight();
  double newh = h * ratio;
  double hdiff = (newh - h)/2.0f;

  SbVec3d upvec = this->ulf - this->llf;
  upvec.normalize();
  SbVec3d diffvec = upvec * hdiff;

  this->llf -= diffvec;
  this->ulf += diffvec;
  this->lrf -= diffvec;
}

/*!
  Return current view volume projection type, which can be
  either \a ORTHOGRAPHIC or \a PERSPECTIVE.

  \sa SbDPViewVolume::ProjectionType
 */
SbDPViewVolume::ProjectionType
SbDPViewVolume::getProjectionType(void) const
{
  return this->type;
}

/*!
  Returns coordinates of center point in the projection plane.
 */
const SbVec3d&
SbDPViewVolume::getProjectionPoint(void) const
{
  return this->projPoint;
}

/*!
  Returns the direction of projection, i.e. the direction the camera is
  pointing.

  \sa getNearDist().
 */
const SbVec3d&
SbDPViewVolume::getProjectionDirection(void) const
{
  return this->projDir;
}

/*!
  Returns distance from projection plane to near clipping plane.

  \sa getProjectionDirection().
 */
double
SbDPViewVolume::getNearDist(void) const
{
  return this->nearDist;
}

/*!
  Returns width of viewing frustum in the projection plane.

  \sa getHeight(), getDepth().
*/
double
SbDPViewVolume::getWidth(void) const
{
  return (this->lrf - this->llf).length();
}

/*!
  Returns height of viewing frustum in the projection plane.

  \sa getWidth(), getDepth().
*/
double
SbDPViewVolume::getHeight(void) const
{
  return (this->ulf - this->llf).length();
}

/*!
  Returns depth of viewing frustum, i.e. the distance from the near clipping
  plane to the far clipping plane.

  \sa getWidth(), getHeight().
 */
double
SbDPViewVolume::getDepth(void) const
{
  return this->nearToFar;
}

/*!
  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized compile.
 */
void
SbDPViewVolume::print(FILE * fp) const
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
SbDPViewVolume::getViewVolumePlanes(SbPlane planes[6]) const
{
  SbVec3d far_ll;
  SbVec3d far_lr;
  SbVec3d far_ul;
  SbVec3d far_ur;

  this->getPlaneRectangle(this->nearToFar, far_ll, far_lr, far_ul, far_ur);
  SbVec3d near_ur = this->ulf + (this->lrf-this->llf);

  SbVec3f f_ulf = to_sbvec3f(this->ulf + this->projPoint);
  SbVec3f f_llf = to_sbvec3f(this->llf + this->projPoint);
  SbVec3f f_lrf = to_sbvec3f(this->lrf + this->projPoint);
  SbVec3f f_near_ur = to_sbvec3f(near_ur + this->projPoint);
  SbVec3f f_far_ll = to_sbvec3f(far_ll + this->projPoint);
  SbVec3f f_far_lr = to_sbvec3f(far_lr + this->projPoint);
  SbVec3f f_far_ul = to_sbvec3f(far_ul + this->projPoint);
  SbVec3f f_far_ur = to_sbvec3f(far_ur + this->projPoint);
  
  planes[0] = SbPlane(f_ulf, f_llf, f_far_ll);  // left
  planes[1] = SbPlane(f_llf, f_lrf, f_far_lr); // bottom
  planes[2] = SbPlane(f_lrf, f_near_ur, f_far_ur); // right
  planes[3] = SbPlane(f_near_ur, f_ulf, f_far_ul); // top
  planes[4] = SbPlane(f_ulf, f_near_ur, f_lrf); // near
  planes[5] = SbPlane(f_far_ll, f_far_lr, f_far_ur); // far
}

/*!
  Transform the viewing volume by \a matrix.
 */
void
SbDPViewVolume::transform(const SbDPMatrix & matrix)
{
  SbVec3d oldprojpt = this->projPoint;
  SbVec3d newprojpt;
  SbVec3d newllf;
  SbVec3d newlrf;
  SbVec3d newulf;
  matrix.multVecMatrix(oldprojpt, newprojpt);

  // need to translate frustum point with the projection point before
  // transforming, then translate back afterwards.
  matrix.multVecMatrix(this->llf+oldprojpt, newllf);
  newllf -= newprojpt;

  matrix.multVecMatrix(this->lrf+oldprojpt, newlrf);
  newlrf -= newprojpt;

  matrix.multVecMatrix(this->ulf+oldprojpt, newulf);
  newulf -= newprojpt;

  // Construct and tranform nearpt and farpt to find the new near and
  // far values.
  SbVec3d nearpt;
  SbVec3d farpt;
  matrix.multVecMatrix(oldprojpt + this->nearDist * this->projDir, 
                       nearpt);

  double fardist = this->nearDist + this->nearToFar;
  matrix.multVecMatrix(oldprojpt + fardist * this->projDir, farpt);

  matrix.multDirMatrix(this->projDir, this->projDir);
  this->projPoint = newprojpt;
  this->llf = newllf;
  this->ulf = newulf;
  this->lrf = newlrf;
  this->nearDist = (nearpt-newprojpt).length();
  this->nearToFar = (farpt-newprojpt).length() - this->nearDist; 
}

/*!
  Returns the view up vector for this view volume. It's a vector
  which is perpendicular to the projection direction, and parallel and
  oriented in the same direction as the vector from the lower left
  corner to the upper left corner of the near plane.
*/
SbVec3d
SbDPViewVolume::getViewUp(void) const
{
  SbVec3d v = this->ulf - this->llf;
  (void) v.normalize();
  return v;
}

//
// Returns the four points defining the view volume rectangle at the
// specified distance from the near plane, towards the far plane. The
// points are returned in normalized view volume coordinates
// (projPoint is not added).
void
SbDPViewVolume::getPlaneRectangle(const double distance, SbVec3d & lowerleft,
                                  SbVec3d & lowerright,
                                  SbVec3d & upperleft,
                                  SbVec3d & upperright) const
{
  SbVec3d near_ur = this->ulf + (this->lrf-this->llf);

  if (this->type == PERSPECTIVE) {
    double depth = this->nearDist + distance;
    SbVec3d dir;
    dir = this->llf;
    dir.normalize();
    lowerleft = dir * depth / dir.dot(this->projDir);

    dir = this->lrf;
    dir.normalize();
    lowerright = dir * depth / dir.dot(this->projDir);

    dir = this->ulf;
    dir.normalize();
    upperleft = dir * depth / dir.dot(this->projDir);

    dir = near_ur;
    dir.normalize();
    upperright = dir * depth / dir.dot(this->projDir);
  }
  else {
    lowerleft = this->llf + this->projDir * distance;
    lowerright = this->lrf + this->projDir * distance;
    upperleft = this->ulf + this->projDir * distance;
    upperright = near_ur + this->projDir * distance;
  }
}

void 
SbDPViewVolume::copyValues(SbViewVolume & vv)
{
  vv.type = (SbViewVolume::ProjectionType) this->type;
  vv.projPoint = to_sbvec3f(this->projPoint);
  vv.projDir = to_sbvec3f(this->projDir);
  vv.nearDist = (float) this->nearDist;
  vv.nearToFar = (float) this->nearToFar;
  vv.llf = to_sbvec3f(this->llf + this->projPoint);
  vv.lrf = to_sbvec3f(this->llf + this->projPoint);
  vv.ulf = to_sbvec3f(this->llf + this->projPoint);
}
