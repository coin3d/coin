/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SbRotation SbLinear.h Inventor/SbLinear.h
  \brief The SbRotation class represents a rotation in 3D space.
  \ingroup base

  SbRotation is used extensively throughout the Coin library.

  An SbRotation is stored internally as a quaternion for speed and
  storage reasons, but inquiries can be done to get and set axis and
  angle values for convenience.

  \sa SbMatrix
*/


#include <Inventor/SbRotation.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbMatrix.h>
#include <assert.h>
#include <float.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  The default constructor just initializes a valid rotation. The
  actual value is unspecified, and you should not depend on it.
*/
SbRotation::SbRotation(void)
  // This translates to zero rotation around the positive Z axis.
  : quat(0.0f, 0.0f, 0.0f, 1.0f)
{
}

/*!
  Construct a new SbRotation object initialized with the given
  axis-of-rotation and rotation angle.
 */
SbRotation::SbRotation(const SbVec3f & axis, const float radians)
{
#if COIN_DEBUG
  if (axis.length()==0.0f)
    SoDebugError::postWarning("SbRotation::SbRotation",
                              "axis parameter has zero length => "
                              "undefined axis.");
#endif // COIN_DEBUG
  this->setValue(axis, radians);
}

/*!
  Construct a new SbRotation object initialized with the given quaternion
  components.

  The array must be ordered as follows:

  q[0] = x, q[1] = y, q[2] = z and q[3] = w, where the quaternion is
  specified by q=w+xi+yj+zk.
 */
SbRotation::SbRotation(const float q[4])
{
  this->setValue(q);
}

/*!
  Construct a new SbRotation object initialized with the given quaternion
  components.
 */
SbRotation::SbRotation(const float q0, const float q1,
                       const float q2, const float q3)
{
  this->setValue(q0, q1, q2, q3);
}

/*!
  Construct a new SbRotation object initialized with the given rotation
  matrix.
 */
SbRotation::SbRotation(const SbMatrix & m)
{
  this->setValue(m);
}

/*!
  Construct a rotation which is the minimum rotation necessary to make vector
  \a rotateFrom point in the direction of vector \a rotateTo.
 */
SbRotation::SbRotation(const SbVec3f & rotateFrom, const SbVec3f & rotateTo)
{
  // Parameters are checked in setValue().

  this->setValue(rotateFrom, rotateTo);
}

/*!
  Return pointer to an array with the rotation expressed as four
  quaternion values.

  \sa setValue().
*/
const float *
SbRotation::getValue(void) const
{
  return &this->quat[0];
}

/*!
  Return the four quaternion components representing the rotation.

  \sa setValue().
 */
void
SbRotation::getValue(float & q0, float & q1, float & q2, float & q3) const
{
  q0 = this->quat[0];
  q1 = this->quat[1];
  q2 = this->quat[2];
  q3 = this->quat[3];
}

/*!
  Set the rotation.

  \sa getValue().
*/
SbRotation &
SbRotation::setValue(const float q0, const float q1,
                     const float q2, const float q3)
{
  this->quat[0] = q0;
  this->quat[1] = q1;
  this->quat[2] = q2;
  this->quat[3] = q3;
  this->quat.normalize();
  return *this;
}

/*!
  Return the rotation in the form of an axis-of-rotation and a rotation
  angle.

  \sa setValue().
 */
void
SbRotation::getValue(SbVec3f & axis, float & radians) const
{
  if((this->quat[3] >= -1.0f) && (this->quat[3] <= 1.0f)) {
    radians = float(acos(this->quat[3])) * 2.0f;
    float scale = (float)sin(radians / 2.0f);

    if(scale != 0.0f) {
      axis[0] = this->quat[0] / scale;
      axis[1] = this->quat[1] / scale;
      axis[2] = this->quat[2] / scale;
      // FIXME: why not just flip the sign on each component according
      // to "scale" and normalize the axis instead? 20010111 mortene.
      return;
    }
  }

  // Quaternion can't be converted to axis and rotation angle, so we just
  // set up values to indicate this.
  axis.setValue(0.0f, 0.0f, 1.0f);
  radians = 0.0f;
}

/*!
  Return this rotation in the form of a matrix.

  \sa setValue().
 */
void
SbRotation::getValue(SbMatrix & matrix) const
{
  const float x = this->quat[0];
  const float y = this->quat[1];
  const float z = this->quat[2];
  const float w = this->quat[3];

  matrix[0][0] = w*w + x*x - y*y - z*z;
  matrix[0][1] = 2*x*y + 2*w*z;
  matrix[0][2] = 2*x*z - 2*w*y;
  matrix[0][3] = 0.0f;

  matrix[1][0] = 2*x*y-2*w*z;
  matrix[1][1] = w*w - x*x + y*y - z*z;
  matrix[1][2] = 2*y*z + 2*w*x;
  matrix[1][3] = 0.0f;

  matrix[2][0] = 2*x*z + 2*w*y;
  matrix[2][1] = 2*y*z - 2*w*x;
  matrix[2][2] = w*w - x*x - y*y + z*z;
  matrix[2][3] = 0.0f;

  matrix[3][0] = 0.0f;
  matrix[3][1] = 0.0f;
  matrix[3][2] = 0.0f;
  matrix[3][3] = w*w + x*x + y*y + z*z;
}

/*!
  Invert the rotation. Returns reference to self.

  \sa inverse()
 */
SbRotation &
SbRotation::invert(void)
{
  float length = this->quat.length();
#if COIN_DEBUG
  if (length==0.0f)
    SoDebugError::postWarning("SbRotation::invert",
                              "Quarternion has zero length => "
                              "undefined rotation.");
#endif // COIN_DEBUG

  // Optimize by doing 1 div and 4 muls instead of 4 divs.
  float inv = 1.0f / length;

  this->quat[0] = -this->quat[0] * inv;
  this->quat[1] = -this->quat[1] * inv;
  this->quat[2] = -this->quat[2] * inv;
  this->quat[3] = this->quat[3] * inv;
  return *this;
}

/*!
  Non-destructively inverses the rotation and returns the result.

  \sa invert()
 */
SbRotation
SbRotation::inverse(void) const
{
  float length = this->quat.length();
#if COIN_DEBUG
  if (length==0.0f)
    SoDebugError::postWarning("SbRotation::inverse",
                              "Quaternion has zero length => "
                              "undefined rotation.");
#endif // COIN_DEBUG

  // Optimize by doing 1 div and 4 muls instead of 4 divs.
  float inv = 1.0f / length;

  SbRotation rot;
  rot.quat[0] = -this->quat[0] * inv;
  rot.quat[1] = -this->quat[1] * inv;
  rot.quat[2] = -this->quat[2] * inv;
  rot.quat[3] = this->quat[3] * inv;

  return rot;
}

/*!
  Reset the rotation by the four quaternions in the array.

  \sa getValue().
 */
SbRotation&
SbRotation::setValue(const float q[4])
{
  this->quat[0] = q[0];
  this->quat[1] = q[1];
  this->quat[2] = q[2];
  this->quat[3] = q[3];
  this->quat.normalize();
  return *this;
}

/*!
  Set the rotation from the components of the given matrix. Returns
  reference to self.

  \sa getValue().
 */
SbRotation &
SbRotation::setValue(const SbMatrix & m)
{
  float scalerow = m[0][0] + m[1][1] + m[2][2];

  if (scalerow > 0.0f) {
    float s = (float)sqrt(scalerow + m[3][3]);
    this->quat[3] = s * 0.5f;
    s = 0.5f / s;

    this->quat[0] = (m[1][2] - m[2][1]) * s;
    this->quat[1] = (m[2][0] - m[0][2]) * s;
    this->quat[2] = (m[0][1] - m[1][0]) * s;
  }
  else {
    int i = 0;
    if (m[1][1] > m[0][0]) i = 1;
    if (m[2][2] > m[i][i]) i = 2;

    int j = (i+1)%3;
    int k = (j+1)%3;

    float s = (float)sqrt((m[i][i] - (m[j][j] + m[k][k])) + m[3][3]);

    this->quat[i] = s * 0.5f;
    s = 0.5f / s;

    this->quat[3] = (m[j][k] - m[k][j]) * s;
    this->quat[j] = (m[i][j] + m[j][i]) * s;
    this->quat[k] = (m[i][k] + m[k][i]) * s;
  }

  if (m[3][3] != 1.0f) this->operator*=(1.0f/(float)(sqrt(m[3][3])));
  return *this;
}

/*!
  Reset rotation with the given axis-of-rotation and rotation angle.
  Returns reference to self.

  Make sure \a axis is not the null vector when calling this method.

  \sa getValue().
 */
SbRotation &
SbRotation::setValue(const SbVec3f & axis, const float radians)
{
#if COIN_DEBUG
  if (axis.length()==0.0f)
    SoDebugError::postWarning("SbRotation::setValue",
                              "axis parameter has zero length.");
#endif // COIN_DEBUG

  // From <http://www.automation.hut.fi/~jaro/thesis/hyper/node9.html>.

  this->quat[3] = (float)cos(radians/2);

  const float sineval = (float)sin(radians/2);
  SbVec3f a = axis;
  a.normalize();
  this->quat[0] = a[0] * sineval;
  this->quat[1] = a[1] * sineval;
  this->quat[2] = a[2] * sineval;
  return *this;
}

/*!
  Construct a rotation which is the minimum rotation necessary to make vector
  \a rotateFrom point in the direction of vector \a rotateTo.

  Returns reference to self.

  \sa getValue().
 */
SbRotation &
SbRotation::setValue(const SbVec3f & rotateFrom, const SbVec3f & rotateTo)
{
#if COIN_DEBUG
  // Check if the vectors are valid.
  if (rotateFrom.length()==0.0f) {
    SoDebugError::postWarning("SbRotation::setValue",
                              "rotateFrom argument has zero length.");
  }
  if (rotateTo.length()==0.0f) {
    SoDebugError::postWarning("SbRotation::setValue",
                              "rotateTo argument has zero length.");
  }
#endif // COIN_DEBUG

  SbVec3f from(rotateFrom);
  from.normalize();
  SbVec3f to(rotateTo);
  to.normalize();

  const float dot = from.dot(to);
  SbVec3f crossvec = from.cross(to);
  const float crosslen = crossvec.length();

  if (crosslen == 0.0f) { // Parallel vectors
    // Check if they are pointing in the same direction.
    if (dot > 0.0f) {
      this->setValue(0.0f, 0.0f, 0.0f, 1.0f);
    }
    // Ok, so they are parallel and pointing in the opposite direction
    // of each other.
    else {
      // Try crossing with x axis.
      SbVec3f t = from.cross(SbVec3f(1.0f, 0.0f, 0.0f));
      // If not ok, cross with y axis.
      if(t.length() == 0.0f) t = from.cross(SbVec3f(0.0f, 1.0f, 0.0f));

      t.normalize();
      this->setValue(t[0], t[1], t[2], 0.0f);
    }
  }
  else { // Vectors are not parallel
    crossvec.normalize();
    // The fabs() wrapping is to avoid problems when `dot' "overflows"
    // a tiny wee bit, which can lead to sqrt() returning NaN.
    crossvec *= (float)sqrt(0.5f * fabs(1.0f - dot));
    // The fabs() wrapping is to avoid problems when `dot' "underflows"
    // a tiny wee bit, which can lead to sqrt() returning NaN.
    this->setValue(crossvec[0], crossvec[1], crossvec[2],
                   (float)sqrt(0.5 * fabs(1.0 + dot)));
  }

  return *this;
}

/*!
  Multiplies the quaternions.

  Note that order is important when combining quaternions with the
  multiplication operator.
 */
SbRotation &
SbRotation::operator*=(const SbRotation & q)
{
  // Formula from <http://www.lboro.ac.uk/departments/ma/gallery/quat/>

  float tx, ty, tz, tw;
  this->getValue(tx, ty, tz, tw);
  float qx, qy, qz, qw;
  q.getValue(qx, qy, qz, qw);

  this->setValue(qw*tx + qx*tw + qy*tz - qz*ty,
                 qw*ty - qx*tz + qy*tw + qz*tx,
                 qw*tz + qx*ty - qy*tx + qz*tw,
                 qw*tw - qx*tx - qy*ty - qz*tz);
  return *this;
}

/*!
  Multiplies components of quaternion with scalar value \a s.
  Returns reference to self.
 */
SbRotation &
SbRotation::operator*=(const float s)
{
  this->quat *= s;
  return *this;
}

/*!
  \relates SbRotation

  Check if the two rotations are equal.

  \sa equals().
 */
int
operator==(const SbRotation & q1, const SbRotation & q2)
{
  return (q1.quat == q2.quat);
}

/*!
  \relates SbRotation

  Check if the two rotations are unequal.

  \sa equals().
 */
int
operator!=(const SbRotation & q1, const SbRotation & q2)
{
  return !(q1 == q2);
}

/*!
  Check the internal quaternion representation vectors for equality
  within the given tolerance.
 */
SbBool
SbRotation::equals(const SbRotation & r, const float tolerance) const
{
  return this->quat.equals(r.quat, tolerance);
}

/*!
  \relates SbRotation

  Multiplies the two rotations and returns the result.

  Note that order is important when combining quaternions with the
  multiplication operator.
*/
SbRotation
operator*(const SbRotation & q1, const SbRotation & q2)
{
  SbRotation q(q1);
  q *= q2;
  return q;
}

/*!
  Rotate the \a src vector and put the result in \a dst.
 */
void
SbRotation::multVec(const SbVec3f & src, SbVec3f & dst) const
{
  // FIXME: this looks amazingly ineffective. Should
  // optimize. 20010907 mortene.

  SbMatrix mat;
  mat.setRotate(*this);
  mat.multVecMatrix(src, dst);
}

/*!
  Scale the angle of rotation by \a scaleFactor.
 */
void
SbRotation::scaleAngle(const float scaleFactor)
{
  SbVec3f axis;
  float rad;

  this->getValue(axis, rad);
  this->setValue(axis, rad * scaleFactor);
}

/*!
  \relates SbRotation

  Interpolates along the shortest path between the two rotation
  positions (from \a rot0 to \a rot1).

  Returns the SbRotation which will rotate \a rot0 the given part \a t
  of the spherical distance towards \a rot1, where \a t=0 will yield \a rot0
  and \a t=1 will yield \a rot1.

  \a t should be in the interval [0, 1].
 */
SbRotation
SbRotation::slerp(const SbRotation & rot0, const SbRotation & rot1, float t)
{
#if COIN_DEBUG
  if (t<0.0f || t>1.0f) {
    SoDebugError::postWarning("SbRotation::slerp",
                              "The t parameter (%f) is out of bounds [0,1]. "
                              "Clamping to bounds.", t);
    if (t<0.0f) t=0.0f;
    else if (t>1.0f) t=1.0f;
  }
#endif // COIN_DEBUG

  SbRotation from = rot0;
  SbRotation to = rot1;

  float dot = from.quat.dot(to.quat);

  // Find the correct direction of the interpolation.
  if(dot < 0.0f) {
    dot = -dot;
    to.quat.negate();
  }

  // fallback to linear interpolation, in case we run out of floating
  // point precision
  float scale0 = 1.0 - t;
  float scale1 = t;

  if ((1.0f - dot) > FLT_EPSILON) {
    float angle = (float)acos(dot);
    float sinangle = (float)sin(angle);
    if (sinangle > FLT_EPSILON) {
      // calculate spherical interpolation
      scale0 = float(sin((1.0 - t) * angle)) / sinangle;
      scale1 = float(sin(t * angle)) / sinangle;
    }
  }
  SbVec4f vec = (scale0 * from.quat) + (scale1 * to.quat);
  return SbRotation(vec[0], vec[1], vec[2], vec[3]);
}

/*!
  Returns an identity rotation.
 */
SbRotation
SbRotation::identity(void)
{
  return SbRotation(0.0f, 0.0f, 0.0f, 1.0f);
}

/*!
  Dump the state of this object to the \a fp file stream. Only works
  in debug version of library, method does nothing in an optimized
  compile.
 */
void
SbRotation::print(FILE * fp) const
{
#if COIN_DEBUG
  this->quat.print(fp);
#endif // COIN_DEBUG
}
