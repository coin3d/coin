//$ TEMPLATE cc_rotation_source(-type-, -elements-, -letter-)
//$ DEFINE -class-name- SbRotation

//$ INSERT TEMPLATE Copyright

#include <Inventor/C/base/rot4f.h>

/* ********************************************************************** */

/*!
  \class -class-name- SbLinear.h Inventor/SbLinear.h
  \brief The -class-name- class represents a rotation in 3D space.
  \ingroup base

  -class-name- is used extensively throughout the Coin library.

  An -class-name- is stored internally as a quaternion for speed and
  storage reasons, but inquiries can be done to get and set axis and
  angle values for convenience.

  \sa SbMatrix
*/

/*!
  \fn -class-name-::-class-name-(void)

  The default constructor just initializes a valid rotation.  The
  actual value is unspecified, and you should not depend on it.
*/

/*!
  \fn -class-name-::-class-name-(const SbVec3f & axis, const float radians)

  Construct a new -class-name- object initialized with the given
  axis-of-rotation and rotation angle.
*/

/*!
  \fn -class-name-::-class-name-(const float q[4])

  Construct a new -class-name- object initialized with the given quaternion
  components.

  The array must be ordered as follows:

  q[0] = x, q[1] = y, q[2] = z and q[3] = w, where the quaternion is
  specified by q=w+xi+yj+zk.
*/

/*!
  \fn -class-name-::-class-name-(const float q0, const float q1, const float q2, const float q3)

  Construct a new -class-name- object initialized with the given quaternion
  components.
*/

/*!
  \fn -class-name-::-class-name-(const SbMatrix & m)

  Construct a new -class-name- object initialized with the given rotation
  matrix.
*/

/*!
  \fn -class-name-::-class-name-(const SbVec3f & rotateFrom, const SbVec3f & rotateTo)

  Construct a rotation which is the minimum rotation necessary to make vector
  \a rotateFrom point in the direction of vector \a rotateTo.
*/

/*!
  \fn const float * -class-name-::getValue(void) const

  Return pointer to an array with the rotation expressed as four
  quaternion values.

  \sa setValue().
*/

/*!
  \fn void -class-name-::getValue(float & q0, float & q1, float & q2, float & q3) const

  Return the four quaternion components representing the rotation.

  \sa setValue().
*/

/*!
  \fn -class-name- & -class-name-::setValue(const float q0, const float q1, const float q2, const float q3)

  Set the rotation.

  \sa getValue().
*/

/*!
  \fn void -class-name-::getValue(SbVec3f & axis, float & radians) const

  Return the rotation in the form of an axis-of-rotation and a rotation
  angle.

  \sa setValue().
*/


/*!
  \fn void -class-name-::getValue(SbMatrix & matrix) const

  Return this rotation in the form of a matrix.

  \sa setValue().
*/

/*!
  \fn -class-name- & -class-name-::invert(void)

  Invert the rotation. Returns reference to self.

  \sa inverse()
*/


/*!
  \fn -class-name- -class-name-::inverse(void) const

  Non-destructively inverses the rotation and returns the result.

  \sa invert()
*/


/*!
  \fn -class-name- & -class-name-::setValue(const float q[4])

  Reset the rotation by the four quaternions in the array.

  \sa getValue().
*/

/*!
  \fn -class-name- & -class-name-::setValue(const SbMatrix & m)

  Set the rotation from the components of the given matrix. Returns
  reference to self.

  \sa getValue().
*/

/*!
  \fn -class-name- & -class-name-::setValue(const SbVec3f & axis, const float radians)

  Reset rotation with the given axis-of-rotation and rotation angle.
  Returns reference to self.

  Make sure \a axis is not the null vector when calling this method.

  \sa getValue().
*/

/*!
  \fn -class-name- & -class-name-::setValue(const SbVec3f & rotateFrom, const SbVec3f & rotateTo)

  Construct a rotation which is the minimum rotation necessary to make vector
  \a rotateFrom point in the direction of vector \a rotateTo.

  Returns reference to self.

  \sa getValue().
*/

/*!
  \fn -class-name- & -class-name-::operator *= (const -class-name- & q)

  Multiplies the quaternions.

  Note that order is important when combining quaternions with the
  multiplication operator.
*/

/*!
  \fn -class-name- & -class-name-::operator*=(const float s)

  Multiplies components of quaternion with scalar value \a s.
  Returns reference to self.
*/

/*!
  \fn int operator == (const -class-name- & q1, const -class-name- & q2)
  \relates -class-name-

  Check if the two rotations are equal.

  \sa equals().
*/

/*!
  \fn int operator != (const -class-name- & q1, const -class-name- & q2)

  \relates -class-name-

  Check if the two rotations are unequal.

  \sa equals().
*/

/*!
  \fn SbBool -class-name-::equals(const -class-name- & r, const float tolerance) const

  Check the internal quaternion representation vectors for equality
  within the given tolerance.
*/

/*!
  \fn -class-name- operator * (const -class-name- & q1, const -class-name- & q2)

  \relates -class-name-

  Multiplies the two rotations and returns the result.

  Note that order is important when combining quaternions with the
  multiplication operator.
*/

/*!
  \fn void -class-name-::multVec(const SbVec3f & src, SbVec3f & dst) const

  Rotate the \a src vector and put the result in \a dst.
*/

/*!
  \fn void -class-name-::scaleAngle(const float scaleFactor)

  Scale the angle of rotation by \a scaleFactor.
*/

/*!
  \fn -class-name- -class-name-::slerp(const -class-name- & rot0, const -class-name- & rot1, float t)

  \relates -class-name-

  Interpolates along the shortest path between the two rotation
  positions (from \a rot0 to \a rot1).

  Returns the -class-name- which will rotate \a rot0 the given part \a t
  of the spherical distance towards \a rot1, where \a t=0 will yield \a rot0
  and \a t=1 will yield \a rot1.

  \a t should be in the interval [0, 1].
*/

/*!
  \fn -class-name- -class-name-::identity(void)

  Returns an identity rotation.
*/

/*!
  \fn void -class-name-::print(FILE * fp) const

  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized compile.
*/

