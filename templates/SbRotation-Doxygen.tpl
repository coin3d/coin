//$ TEMPLATE SbRotation-Doxygen(-type-, -elements-, -letter-)
//$ IF "-type-" =~ m/^(float|double|long double)$/
//$ DEFINE -integertype- 0
//$ ELSE
//$ DEFINE -integertype- 1
//$ ERROR "unimplemented data type"
//$ ENDIF
//$ IF -elements- < 4 || -elements- > 4
//$ ERROR "unimplemented dimension"
//$ ENDIF
//$ DEFINE -cxx_class- SbRotation
//$ DEFINE -cc_class- cc_rot-elements--letter-
//$ INSERT TEMPLATE Copyright

/*!
  \class -cxx_class- SbLinear.h Inventor/SbLinear.h
  \brief The -cxx_class- class represents a rotation in 3D space.
  \ingroup base

  -cxx_class- is used extensively throughout the Coin library.

  An -cxx_class- is stored internally as a quaternion for speed and
  storage reasons, but inquiries can be done to get and set axis and
  angle values for convenience.

  \sa SbMatrix
*/

/* ********************************************************************** */

/*!
  \fn -cxx_class-::-cxx_class-(void)

  The default constructor just initializes a valid rotation.  The
  actual value is unspecified, and you should not depend on it.
*/

/*!
  \fn -cxx_class-::-cxx_class-(const SbVec3f & axis, const -type- radians)

  Construct a new -cxx_class- object initialized with the given
  axis-of-rotation and rotation angle.
*/

/*!
  \fn -cxx_class-::-cxx_class-(const -type- q[4])

  Construct a new -cxx_class- object initialized with the given quaternion
  components.

  The array must be ordered as follows:

  q[0] = x, q[1] = y, q[2] = z and q[3] = w, where the quaternion is
  specified by q=w+xi+yj+zk.
*/

/*!
  \fn -cxx_class-::-cxx_class-(const -type- q0, const -type- q1, const -type- q2, const -type- q3)

  Construct a new -cxx_class- object initialized with the given quaternion
  components.
*/

/*!
  \fn -cxx_class-::-cxx_class-(const SbMatrix & m)

  Construct a new -cxx_class- object initialized with the given rotation
  matrix.
*/

/*!
  \fn -cxx_class-::-cxx_class-(const SbVec3f & rotateFrom, const SbVec3f & rotateTo)

  Construct a rotation which is the minimum rotation necessary to make vector
  \a rotateFrom point in the direction of vector \a rotateTo.
*/

/*!
  \fn const -type- * -cxx_class-::getValue(void) const

  Return pointer to an array with the rotation expressed as four
  quaternion values.

  \sa setValue().
*/

/*!
  \fn void -cxx_class-::getValue(-type- & q0, -type- & q1, -type- & q2, -type- & q3) const

  Return the four quaternion components representing the rotation.

  \sa setValue().
*/

/*!
  \fn -cxx_class- & -cxx_class-::setValue(const -type- q0, const -type- q1, const -type- q2, const -type- q3)

  Set the rotation.

  \sa getValue().
*/

/*!
  \fn void -cxx_class-::getValue(SbVec3f & axis, -type- & radians) const

  Return the rotation in the form of an axis-of-rotation and a rotation
  angle.

  \sa setValue().
*/


/*!
  \fn void -cxx_class-::getValue(SbMatrix & matrix) const

  Return this rotation in the form of a matrix.

  \sa setValue().
*/

/*!
  \fn -cxx_class- & -cxx_class-::invert(void)

  Invert the rotation. Returns reference to self.

  \sa inverse()
*/


/*!
  \fn -cxx_class- -cxx_class-::inverse(void) const

  Non-destructively inverses the rotation and returns the result.

  \sa invert()
*/


/*!
  \fn -cxx_class- & -cxx_class-::setValue(const -type- q[4])

  Reset the rotation by the four quaternions in the array.

  \sa getValue().
*/

/*!
  \fn -cxx_class- & -cxx_class-::setValue(const SbMatrix & m)

  Set the rotation from the components of the given matrix. Returns
  reference to self.

  \sa getValue().
*/

/*!
  \fn -cxx_class- & -cxx_class-::setValue(const SbVec3f & axis, const -type- radians)

  Reset rotation with the given axis-of-rotation and rotation angle.
  Returns reference to self.

  Make sure \a axis is not the null vector when calling this method.

  \sa getValue().
*/

/*!
  \fn -cxx_class- & -cxx_class-::setValue(const SbVec3f & rotateFrom, const SbVec3f & rotateTo)

  Construct a rotation which is the minimum rotation necessary to make vector
  \a rotateFrom point in the direction of vector \a rotateTo.

  Returns reference to self.

  \sa getValue().
*/

/*!
  \fn -cxx_class- & -cxx_class-::operator *= (const -cxx_class- & q)

  Multiplies the quaternions.

  Note that order is important when combining quaternions with the
  multiplication operator.
*/

/*!
  \fn -cxx_class- & -cxx_class-::operator*=(const -type- s)

  Multiplies components of quaternion with scalar value \a s.
  Returns reference to self.
*/

/*!
  \fn int operator == (const -cxx_class- & q1, const -cxx_class- & q2)
  \relates -cxx_class-

  Check if the two rotations are equal.

  \sa equals().
*/

/*!
  \fn int operator != (const -cxx_class- & q1, const -cxx_class- & q2)

  \relates -cxx_class-

  Check if the two rotations are unequal.

  \sa equals().
*/

/*!
  \fn SbBool -cxx_class-::equals(const -cxx_class- & r, const -type- tolerance) const

  Check the internal quaternion representation vectors for equality
  within the given tolerance.
*/

/*!
  \fn -cxx_class- operator * (const -cxx_class- & q1, const -cxx_class- & q2)

  \relates -cxx_class-

  Multiplies the two rotations and returns the result.

  Note that order is important when combining quaternions with the
  multiplication operator.
*/

/*!
  \fn void -cxx_class-::multVec(const SbVec3f & src, SbVec3f & dst) const

  Rotate the \a src vector and put the result in \a dst.
*/

/*!
  \fn void -cxx_class-::scaleAngle(const -type- scaleFactor)

  Scale the angle of rotation by \a scaleFactor.
*/

/*!
  \fn -cxx_class- -cxx_class-::slerp(const -cxx_class- & rot0, const -cxx_class- & rot1, -type- t)

  \relates -cxx_class-

  Interpolates along the shortest path between the two rotation
  positions (from \a rot0 to \a rot1).

  Returns the -cxx_class- which will rotate \a rot0 the given part \a t
  of the spherical distance towards \a rot1, where \a t=0 will yield \a rot0
  and \a t=1 will yield \a rot1.

  \a t should be in the interval [0, 1].
*/

/*!
  \fn -cxx_class- -cxx_class-::identity(void)

  Returns an identity rotation.
*/

/*!
  \fn void -cxx_class-::print(FILE * fp) const

  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized compile.
*/

