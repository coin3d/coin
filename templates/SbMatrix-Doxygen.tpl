//$ TEMPLATE SbMatrix-Doxygen(-type-, -elements-, -letter-)

//$ INSERT TEMPLATE Copyright

/* ********************************************************************** */

/*!
  \class -class-name- SbLinear.h Inventor/SbLinear.h
  \brief The -class-name- class is a 4x4 dimensional representation of a matrix.
  \ingroup base

  -class-name- is used by many other classes in Coin.  It provides storage
  for a 4x4 matrix in row-major mode. Many common geometrical
  operations which involves matrix calculations are implemented as
  methods on this class.
*/

/*!
  \fn -class-name-::-class-name-(void)

  The default constructor does nothing. The matrix will be uninitialized.
*/

/*!
  \fn -class-name-::-class-name-(const float a11, const float a12, const float a13, const float a14, const float a21, const float a22, const float a23, const float a24, const float a31, const float a32, const float a33, const float a34, const float a41, const float a42, const float a43, const float a44)

  Constructs a matrix instance with the given initial elements.
*/

/*!
  Constructs a matrix instance with the initial elements from the
  \a matrix argument.
*/
-class-name-::-class-name-(const SbMat & matrix)

/*!
  This constructor is courtesy of the Microsoft Visual C++ compiler.
*/
-class-name-::-class-name-(const SbMat * matrix)

/*!
  Default destructor does nothing.
 */
-class-name-::~-class-name-(void)

/*!
  \fn const SbMat & -class-name-::getValue(void) const

  Returns a pointer to the 2 dimensional float array with the matrix
  elements.

  \sa setValue().
*/

/*!
  \fn void -class-name-::setValue(const SbMat & m)

  Copies the elements from \a m into the matrix.

  \sa getValue().
*/

/*!
  \fn -class-name- & -class-name-::operator=(const SbMat & m)

  Assignment operator. Copies the elements from \a m to the matrix.
*/

/*!
  \fn -class-name- & -class-name-::operator=(const -class-name- & m)

  Assignment operator. Copies the elements from \a m to the matrix.
*/

/*!
  \fn void -class-name-::makeIdentity(void)

  Set the matrix to be the identity matrix.

  \sa identity().
*/

/*!
  \fn void -class-name-::setRotate(const SbRotation & q)

  Set matrix to be a rotation matrix with the given rotation.

  \sa setTranslate(), setScale().
*/

/*!
  \fn void -class-name-::operator *= (const float v)

  Multiply all element values in the matrix with \a v.
*/

/*!
  \fn void -class-name-::operator /= (const float v)

  Divide all element values in the matrix on \a v.
*/

/*!
  \fn float -class-name-::det3(int r1, int r2, int r3, int c1, int c2, int c3) const

  Returns the determinant of the 3x3 submatrix specified by the row and
  column indices.
*/

/*!
  \fn float -class-name-::det3(void) const

  Returns the determinant of the upper left 3x3 submatrix.
*/

/*!
  \fn float -class-name-::det4(void) const

  Returns the determinant of the matrix.
*/

/*!
  \fn -class-name- -class-name-::inverse(void) const

  Return a new matrix which is the inverse matrix of this.

  The user is responsible for checking that this is a valid operation
  to execute, by first making sure that the result of -class-name-::det4()
  is not equal to zero.
*/

/*!
  \fn SbBool -class-name-::equals(const -class-name- & m, float tolerance) const

  Check if the \a m matrix is equal to this one, within the given tolerance
  value. The tolerance value is applied in the comparison on a component by
  component basis.
*/

/*!
  \fn -class-name-::operator float * (void)

  Return pointer to the matrix' 4x4 float array.
*/

/*!
  \fn -class-name-::operator SbMat & (void)

  Return pointer to the matrix' 4x4 float array.
*/

/*!
  \fn float * -class-name-::operator [] (int i)

  Returns pointer to the 4 element array representing a matrix row.
  \a i should be within [0, 3].

  \sa getValue(), setValue().
*/

/*!
  \fn const float * -class-name-::operator [](int i) const

  Returns pointer to the 4 element array representing a matrix row.
  \a i should be within [0, 3].

  \sa getValue(), setValue().
*/

/*!
  \fn -class-name- & -class-name-::operator = (const SbRotation & q)

  Set matrix to be a rotation matrix with the given rotation.

  \sa setRotate().
*/

/*!
  \fn -class-name- & -class-name-::operator *= (const -class-name- & m)

  Right-multiply with the \a m matrix.

  \sa multRight().
*/

/*!
  \fn -class-name- operator * (const -class-name- & m1, const -class-name- & m2)

  \relates -class-name-

  Multiplies matrix \a m1 with matrix \a m2 and returns the resultant
  matrix.
*/

/*!
  \fn int operator == (const -class-name- & m1, const -class-name- & m2)

  \relates -class-name-

  Compare matrices to see if they are equal. For two matrices to be equal,
  all their individual elements must be equal.

  \sa equals().
*/

/*!
  \fn int operator != (const -class-name- & m1, const -class-name- & m2)

  \relates -class-name-

  Compare matrices to see if they are not equal. For two matrices to not be
  equal, it is enough that at least one of their elements are unequal.

  \sa equals().
*/

/*!
  \fn void -class-name-::getValue(SbMat & m) const

  Return matrix components in the SbMat structure.

  \sa setValue().
*/

/*!
  \fn -class-name- -class-name-::identity(void)

  Return the identity matrix.

  \sa makeIdentity().
*/


/*!
  \fn void -class-name-::setScale(const float s)

  Set matrix to be a pure scaling matrix. Scale factors are specified
  by \a s.

  \sa setRotate(), setTranslate().
*/

/*!
  \fn void -class-name-::setScale(const SbVec3f & s)

  Set matrix to be a pure scaling matrix. Scale factors in x, y and z
  is specified by the \a s vector.

  \sa setRotate(), setTranslate().
*/

/*!
  \fn void -class-name-::setTranslate(const SbVec3f & t)

  Make this matrix into a pure translation matrix (no scale or rotation
  components) with the given vector \t as the translation.

  \sa setRotate(), setScale().
*/

/*!
  \fn void -class-name-::setTransform(const SbVec3f & t, const SbRotation & r, const SbVec3f & s)

  Set translation, rotation and scaling all at once. The resulting
  matrix gets calculated like this:

  \code
  M = S * R * T
  \endcode

  where \a S, \a R and \a T is scaling, rotation and translation
  matrices.

  \sa setTranslate(), setRotate(), setScale() and getTransform().
*/

/*!
  \fn void -class-name-::setTransform(const SbVec3f & t, const SbRotation & r, const SbVec3f & s, const SbRotation & so)
  Set translation, rotation and scaling all at once with a specified
  scale orientation. The resulting matrix gets calculated like this:

  \code
  M = Ro-¹ * S * Ro * R * T
  \endcode

  where \a Ro is the scale orientation, and \a S, \a R
  and \a T is scaling, rotation and translation.

  \sa setTranslate(), setRotate(), setScale() and getTransform().
*/

/*!
  \fn void -class-name-::setTransform(const SbVec3f & translation, const SbRotation & rotation, const SbVec3f & scaleFactor, const SbRotation & scaleOrientation, const SbVec3f & center)

  Set translation, rotation and scaling all at once with a specified
  scale orientation and center point. The resulting matrix gets
  calculated like this:

  \code
  M = -Tc * Ro-¹ * S * Ro * R * T * Tc
  \endcode

  where \a Tc is the center point, \a Ro the scale orientation, \a S,
  \a R and \a T is scaling, rotation and translation.

  \sa setTranslate(), setRotate(), setScale() and getTransform().
*/

/*!
  \fn void -class-name-::getTransform(SbVec3f & t, SbRotation & r, SbVec3f & s, SbRotation & so) const

  Factor the matrix back into its translation, rotation, scale and
  scaleorientation components.

  \sa factor()
*/

/*!
  \fn void -class-name-::getTransform(SbVec3f & translation, SbRotation & rotation, SbVec3f & scaleFactor, SbRotation & scaleOrientation, const SbVec3f & center) const

  Factor the matrix back into its \a translation, \a rotation,
  \a scaleFactor and \a scaleorientation components. Will eliminate
  the \a center variable from the matrix.

  \sa factor()
*/

/*!
  \fn SbBool -class-name-::factor(-class-name- & r, SbVec3f & s, -class-name- & u, SbVec3f & t, -class-name- & proj)

  This function is not implemented in Coin.

  \sa getTransform()
*/

/*!
  \fn SbBool -class-name-::LUDecomposition(int index[4], float & d)

  This function produces a permuted LU decomposition of the matrix.  It
  uses the common single-row-pivoting strategy.

  \a FALSE is returned if the matrix is singular, which it never is, because
  of small adjustment values inserted if a singularity is found (as Open
  Inventor does too).

  The parity argument is always set to 1.0 or -1.0.  Don't really know what
  it's for, so it's not checked for correctness.

  The index[] argument returns the permutation that was done on the matrix
  to LU-decompose it.  index[i] is the row that row i was swapped with at
  step i in the decomposition, so index[] is not the actual permutation of
  the row indexes!

  BUGS:  The function does not produce results that are numerically identical
  with those produced by Open Inventor for the same matrices, because the
  pivoting strategy in OI was never fully understood.

  \sa -class-name-::LUBackSubstitution
*/

/*!
  \fn void -class-name-::LUBackSubstitution(int index[4], float b[4]) const

  This function does a solve on the "Ax = b" system, given that the matrix
  is LU-decomposed in advance.  First, a forward substitution is done on the
  lower system (Ly = b), and then a backwards substitution is done on the
  upper triangular system (Ux = y).

  The index[] argument is the one returned from
  -class-name-::LUDecomposition(), so see that function for an explanation.

  The b[] argument must contain the b vector in "Ax = b" when calling the
  function.  After the function has solved the system, the b[] vector contains
  the x vector.

  BUGS:  As is done by Open Inventor, unsolvable x values will not return
  NaN but 0.
*/

/*!
  \fn -class-name- -class-name-::transpose(void) const

  Returns the transpose of this matrix.
*/

/*!
  \fn -class-name- & -class-name-::multRight(const -class-name- & m)

  Let this matrix be right-multiplied by \a m. Returns reference to
  self.

  \sa multLeft()
*/

/*!
  \fn -class-name- & -class-name-::multLeft(const -class-name- & m)

  Let this matrix be left-multiplied by \a m. Returns reference to
  self.

  \sa multRight()
*/

/*!
  \fn void -class-name-::multMatrixVec(const SbVec3f & src, SbVec3f & dst) const

  Multiply \a src vector with this matrix and return the result in \a dst.
  Multiplication is done with the vector on the right side of the
  expression, i.e. dst = M * src.

  \sa multVecMatrix(), multDirMatrix() and multLineMatrix().
*/

/*!
  \fn void -class-name-::multVecMatrix(const SbVec3f & src, SbVec3f & dst) const

  Multiply \a src vector with this matrix and return the result in \a dst.
  Multiplication is done with the vector on the left side of the
  expression, i.e. dst = src * M.

  It is safe to let \a src and \a dst be the same SbVec3f instance.

  \sa multMatrixVec(), multDirMatrix() and multLineMatrix().
*/

/*!
  \fn void -class-name-::multVecMatrix(const SbVec4f & src, SbVec4f & dst) const

  \overload
*/

/*!
  \fn void -class-name-::multDirMatrix(const SbVec3f & src, SbVec3f & dst) const

  Multiplies \a src by the matrix. \a src is assumed to be a direction
  vector, and the translation components of the matrix are therefore
  ignored.

  Multiplication is done with the vector on the left side of the
  expression, i.e. dst = src * M.

  \sa multVecMatrix(), multMatrixVec() and multLineMatrix().
*/

/*!
  \fn void -class-name-::multLineMatrix(const SbLine & src, SbLine & dst) const

  Multiplies line point with the full matrix and multiplies the
  line direction with the matrix without the translation components.

  \sa multVecMatrix(), multMatrixVec() and multDirMatrix().
*/


/*!
  \fn void -class-name-::print(FILE * fp) const

  Write out the matrix contents to the given file.
*/

