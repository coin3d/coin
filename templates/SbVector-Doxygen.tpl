//$ TEMPLATE SbVector-Doxygen(-type-, -elements-, -letter-)
//$ DEFINE -class-name- SbVec-elements--letter-
//$ DEFINE -cc-class- cc_vec-elements--letter-
//$ IF "-type-" =~ m/^(float|double|long double)$/
//$ DEFINE -integertype- 0
//$ ELSE
//$ DEFINE -integertype- 1
//$ ENDIF
//$ IF -elements- < 2 || -elements- > 4
//$ ERROR "unimplemented dimension"
//$ ENDIF

//$ INSERT TEMPLATE Copyright

/* ********************************************************************** * 
 *  DOXYGEN doc for the C++ wrappers
 * ********************************************************************** */

#include <Inventor/C/SbVec-elements--letter-.h>

/*!
  \class -class-name- SbLinear.h Inventor/SbLinear.h
  \brief The -class-name- class is a -elements- dimensional vector with
//$ IF -integertype-
  integer coordinates.
//$ ELSE
  floating point coordinates.
//$ ENDIF
  \ingroup base

  This vector class is used by many other classes in
  Coin. It provides storage for a -elements- dimensional vector
  aswell as simple arithmetic operations.

  \sa SbVec2s, SbVec2f, SbVec2d, SbVec3s, SbVec3f, SbVec3d, SbVec4s, SbVec4f, SbVec4d.
*/


/*!
  \fn -class-name-::-class-name-(void)

  The default constructor does nothing. The vector coordinates will be
  uninitialized until you do a setValue() call.
*/

/*!
  \fn -class-name-::-class-name-(const -type- v[-elements-])

  Constructs an -class-name- instance with initial values from \a v.
*/

//$ IF -elements- == 2
/*!
  \fn -class-name-::-class-name-(const -type- x, const -type- y)

  Constructs an -class-name- instance with the initial vector endpoint set
  to \a <x,y>.
*/
//$ ELSIF -elements- == 3
/*!
  \fn -class-name-::-class-name-(const -type- x, const -type- y, const -type- z)

  Constructs an -class-name- instance with the initial vector endpoint set
  to \a <x,y,z>.
*/
//$ ELSIF -elements- == 4
/*!
  \fn -class-name-::-class-name-(const -type- x, const -type- y, const -type- z, const -type- w)

  Constructs an -class-name- instance with the initial vector endpoint set
  to \a <x,y,z,w>.
*/
//$ ENDIF

//$ IF ! -integertype- && -elements- == 3
/*!
  \fn -class-name-::-class-name-(const SbPlane & p0, const SbPlane & p1, const SbPlane & p2)

  Constructs an SbVec3-letter- instance by combining the three given planes.
  None of the planes should be parallel to any of the other two, otherwise
  a divide by zero error will occur.
*/
//$ ENDIF

/*!
  \fn -class-name- -class-name-::cross(const -class-name- & v) const

  Returns the result of taking the cross product of this vector and \a v.
*/

/*!
  \fn -type- -class-name-::dot(const -class-name- & v) const

  Calculates and returns the result of taking the dot product of this
  vector and \a v.
*/

//$ IF ! -integertype-
/*!
  \fn SbBool -class-name-::equals(const -class-name- & v, const -type- tolerance) const

  Compares the vector with \a v and returns \c TRUE if the largest
  distance between the vectors are larger than the square root of the
  given tolerance value.
*/
//$ ENDIF

/*!
  \fn -class-name- -class-name-::getClosestAxis(void) const

  Return the vector representing the principal axis closest to this
  vector.
*/

/*!
  \fn const -type- * -class-name-::getValue(void) const

  Returns a pointer to an array of three -type-s containing the
  coordinates of the vector.

  \sa setValue().
*/

/*!
  \fn void -class-name-::getValue(-type- & x, -type- & y) const //$ IF -elements- == 2
  \fn void -class-name-::getValue(-type- & x, -type- & y, -type- & z) const //$ IF -elements- == 3
  \fn void -class-name-::getValue(-type- & x, -type- & y, -type- & z, -type- & w) const //$ IF -elements- == 4

  Returns the coordinates of the vector.

  \sa setValue().
*/

/*!
  \fn -type- -class-name-::length(void) const

  Return length of vector.
*/

/*!
  \fn -type- -class-name-::sqrLength(void) const

  Returns the squared length of the vector.
*/

/*!
  \fn void -class-name-::negate(void)

  Negate the vector (i.e. point it in the opposite direction).
*/

/*!
  \fn -type- -class-name-::normalize(void)

  Normalize the vector to unit length. Return value is the original
  length of the vector before normalization.

  If the vector is the null vector, no attempt at normalization will
  be done.
*/

/*!
  \fn -class-name- & -class-name-::setValue(const -type- v[-elements-])

  Set new coordinates for the vector from \a v. Returns reference to
  self.

  \sa getValue().
*/

/*!
  \fn -class-name- & -class-name-::setValue(const -type- x, const -type- y) //$ IF -elements- == 2
  \fn -class-name- & -class-name-::setValue(const -type- x, const -type- y, const -type- z) //$ IF -elements- == 3
  \fn -class-name- & -class-name-::setValue(const -type- x, const -type- y, const -type- z, const -type- w) //$ IF -elements- == 4

  Set new coordinates for the vector. Returns reference to self.

  \sa getValue().
*/

//$ IF ! -integertype- && -elements- == 3
/*!
  \fn -class-name- & -class-name-::setValue(const -class-name- & barycentric, const -class-name- & v0, const -class-name- & v1, const -class-name- & v2)

  Set this vector to be the average of \a v0, \a v1 and \a v2.
  The vector components are weighted by the \a barycentric vector.

  \sa getValue().
*/
//$ ENDIF

/*!
  \fn -type- & -class-name-::operator[](const int i)

  Index operator. Returns modifiable x, y or z coordinate of vector.

  \sa getValue() and setValue().
*/

/*!
  \fn -type- -class-name-::operator[](const int i) const

  Index operator. Returns x, y or z coordinate of vector.

  \sa getValue() and setValue().
*/

//$ IF -integertype-
/*!
  \fn -class-name- & -class-name-::operator *= (int d)

  Multiply components of vector with value \a d. Returns reference to self.
*/

/*!
  \fn -class-name- & -class-name-::operator *= (double d)

  Multiply components of vector with value \a d. Returns reference to self.
*/

/*!
  \fn -class-name- & -class-name-::operator /= (int d)

  Divides components of vector with value \a d. Returns reference to self.
*/

/*!
  \fn -class-name- & -class-name-::operator /= (double d)

  Divides components of vector with value \a d. Returns reference to self.
*/
//$ ELSE
/*!
  \fn -class-name- & -class-name-::operator *= (const -type- d)

  Multiply components of vector with value \a d. Returns reference to self.
*/

/*!
  \fn -class-name- & -class-name-::operator /= (const -type- d)

  Divides components of vector with value \a d. Returns reference to self.
*/
//$ ENDIF

/*!
  \fn -class-name- & -class-name-::operator += (const -class-name- & u)

  Adds this vector and vector \a u. Returns reference to self.
*/

/*!
  \fn -class-name- & -class-name-::operator -= (const -class-name- & u)

  Subtracts vector \a u from this vector. Returns reference to self.
*/

/*!
  \fn -class-name- -class-name-::operator - (void) const

  Non-mutating negation operator. Returns a new -class-name- instance
  which points in the opposite direction of this vector.

  \sa negate().
*/

//$ IF -integertype-
/*!
  \fn -class-name- operator * (const -class-name- & v, int d)

  \relates -class-name-

  Returns an -class-name- instance which is the components of vector \a v
  multiplied with \a d.
*/

/*!
  \fn -class-name- operator * (const -class-name- & v, double d)

  \relates -class-name-

  Returns an -class-name- instance which is the components of vector \a v
  multiplied with \a d.
*/

/*!
  \fn -class-name- operator * (int d, const -class-name- & v)

  \relates -class-name-

  Returns an -class-name- instance which is the components of vector \a v
  multiplied with \a d.
*/

/*!
  \fn -class-name- operator * (double d, const -class-name- & v)

  \relates -class-name-

  Returns an -class-name- instance which is the components of vector \a v
  multiplied with \a d.
*/

/*!
  \fn -class-name- operator / (const -class-name- & v, int d)

  \relates -class-name-

  Returns an -class-name- instance which is the components of vector \a v
  divided on the scalar factor \a d.
*/

/*!
  \fn -class-name- operator / (const -class-name- & v, double d)

  \relates -class-name-

  Returns an -class-name- instance which is the components of vector \a v
  divided on the scalar factor \a d.
*/
//$ ELSE
/*!
  \fn -class-name- operator * (const -class-name- & v, const -type- d)

  \relates -class-name-

  Returns an -class-name- instance which is the components of vector \a v
  multiplied with \a d.
*/

/*!
  \fn -class-name- operator * (const -type- d, const -class-name- & v)

  \relates -class-name-

  Returns an -class-name- instance which is the components of vector \a v
  multiplied with \a d.
*/

/*!
  \fn -class-name- operator / (const -class-name- & v, const -type- d)

  \relates -class-name-

  Returns an -class-name- instance which is the components of vector \a v
  divided on the scalar factor \a d.
*/
//$ ENDIF

/*!
  \fn -class-name- operator + (const -class-name- & v1, const -class-name- & v2)

  \relates -class-name-

  Returns an -class-name- instance which is the sum of vectors \a v1 and \a v2.
*/

/*!
  \fn -class-name- operator - (const -class-name- & v1, const -class-name- & v2)

  \relates -class-name-

  Returns an -class-name- instance which is vector \a v2 subtracted from
  vector \a v1.
*/

/*!
  \fn int operator == (const -class-name- & v1, const -class-name- & v2)

  \relates -class-name-

  Returns \a 1 if \a v1 and \a v2 are \e exactly equal, \a 0 otherwise.

  \sa equals().
*/

/*!
  \fn int operator != (const -class-name- & v1, const -class-name- & v2)

  \relates -class-name-

  Returns \a 1 if \a v1 and \a v2 are not equal, \a 0 if they are equal.

  \sa equals().
*/

/*!
  \fn void -class-name-::print(FILE * fp) const

  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized
  compile.
*/

