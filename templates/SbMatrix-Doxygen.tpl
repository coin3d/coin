//$ TEMPLATE SbMatrix-Doxygen(-type-, -elements-, -letter-)
//$ IF "-type-" =~ m/^(float|double|long double)$/
//$ DEFINE -integertype- 0
//$ ELSE
//$ DEFINE -integertype- 1
//$ ERROR "unimplemented data type"
//$ ENDIF
//$ IF -elements- < 4 || -elements- > 4
//$ ERROR "unimplemented dimension"
//$ ENDIF
//$ DEFINE -cxx_class- SbMatrix
//$ DEFINE -cc_class- cc_mat-elements--letter-
//$ INSERT TEMPLATE Copyright

/*!
  \class -cxx_class- SbLinear.h Inventor/SbLinear.h
  \brief The -cxx_class- class is a 4x4 dimensional representation of a matrix.
  \ingroup base

  -cxx_class- is used by many other classes in Coin.  It provides storage
  for a 4x4 matrix in row-major mode. Many common geometrical
  operations which involves matrix calculations are implemented as
  methods on this class.
*/

/* ********************************************************************** */

/*!
  \fn -cxx_class-::-cxx_class-(void)

  The default constructor does nothing. The matrix will be uninitialized.
*/

/*!
  \fn -cxx_class-::-cxx_class-(const -type- a11, const -type- a12, const -type- a13, const -type- a14, const -type- a21, const -type- a22, const -type- a23, const -type- a24, const -type- a31, const -type- a32, const -type- a33, const -type- a34, const -type- a41, const -type- a42, const -type- a43, const -type- a44)

  Constructs a matrix instance with the given initial elements.
*/

/*!
  Constructs a matrix instance with the initial elements from the
  \a matrix argument.
*/
-cxx_class-::-cxx_class-(const SbMat & matrix)

/*!
  This constructor is courtesy of the Microsoft Visual C++ compiler.
*/
-cxx_class-::-cxx_class-(const SbMat * matrix)

/*!
  Default destructor does nothing.
 */
-cxx_class-::~-cxx_class-(void)

/*!
  \fn const SbMat & -cxx_class-::getValue(void) const

  Returns a pointer to the 2 dimensional -type- array with the matrix
  elements.

  \sa setValue().
*/

/*!
  \fn void -cxx_class-::setValue(const SbMat & m)

  Copies the elements from \a m into the matrix.

  \sa getValue().
*/

/*!
  \fn -cxx_class- & -cxx_class-::operator=(const SbMat & m)

  Assignment operator. Copies the elements from \a m to the matrix.
*/

/*!
  \fn -cxx_class- & -cxx_class-::operator=(const -cxx_class- & m)

  Assignment operator. Copies the elements from \a m to the matrix.
*/

/*!
  \fn void -cxx_class-::makeIdentity(void)

  Set the matrix to be the identity matrix.

  \sa identity().
*/

/*!
  \fn void -cxx_class-::setRotate(const SbRotation & q)

  Set matrix to be a rotation matrix with the given rotation.

  \sa setTranslate(), setScale().
*/

/*!
  \fn void -cxx_class-::operator *= (const -type- v)

  Multiply all element values in the matrix with \a v.
*/

/*!
  \fn void -cxx_class-::operator /= (const -type- v)

  Divide all element values in the matrix on \a v.
*/

/*!
  \fn -type- -cxx_class-::det3(int r1, int r2, int r3, int c1, int c2, int c3) const

  Returns the determinant of the 3x3 submatrix specified by the row and
  column indices.
*/

/*!
  \fn -type- -cxx_class-::det3(void) const

  Returns the determinant of the upper left 3x3 submatrix.
*/

/*!
  \fn -type- -cxx_class-::det4(void) const

  Returns the determinant of the matrix.
*/

/*!
  \fn -cxx_class- -cxx_class-::inverse(void) const

  Return a new matrix which is the inverse matrix of this.

  The user is responsible for checking that this is a valid operation
  to execute, by first making sure that the result of -cxx_class-::det4()
  is not equal to zero.
*/

/*!
  \fn SbBool -cxx_class-::equals(const -cxx_class- & m, -type- tolerance) const

  Check if the \a m matrix is equal to this one, within the given tolerance
  value. The tolerance value is applied in the comparison on a component by
  component basis.
*/

/*!
  \fn -cxx_class-::operator -type- * (void)

  Return pointer to the matrix' 4x4 -type- array.
*/

/*!
  \fn -cxx_class-::operator SbMat & (void)

  Return pointer to the matrix' 4x4 -type- array.
*/

/*!
  \fn -type- * -cxx_class-::operator [] (int i)

  Returns pointer to the 4 element array representing a matrix row.
  \a i should be within [0, 3].

  \sa getValue(), setValue().
*/

/*!
  \fn const -type- * -cxx_class-::operator [](int i) const

  Returns pointer to the 4 element array representing a matrix row.
  \a i should be within [0, 3].

  \sa getValue(), setValue().
*/

/*!
  \fn -cxx_class- & -cxx_class-::operator = (const SbRotation & q)

  Set matrix to be a rotation matrix with the given rotation.

  \sa setRotate().
*/

/*!
  \fn -cxx_class- & -cxx_class-::operator *= (const -cxx_class- & m)

  Right-multiply with the \a m matrix.

  \sa multRight().
*/

/*!
  \fn -cxx_class- operator * (const -cxx_class- & m1, const -cxx_class- & m2)

  \relates -cxx_class-

  Multiplies matrix \a m1 with matrix \a m2 and returns the resultant
  matrix.
*/

/*!
  \fn int operator == (const -cxx_class- & m1, const -cxx_class- & m2)

  \relates -cxx_class-

  Compare matrices to see if they are equal. For two matrices to be equal,
  all their individual elements must be equal.

  \sa equals().
*/

/*!
  \fn int operator != (const -cxx_class- & m1, const -cxx_class- & m2)

  \relates -cxx_class-

  Compare matrices to see if they are not equal. For two matrices to not be
  equal, it is enough that at least one of their elements are unequal.

  \sa equals().
*/

/*!
  \fn void -cxx_class-::getValue(SbMat & m) const

  Return matrix components in the SbMat structure.

  \sa setValue().
*/

/*!
  \fn -cxx_class- -cxx_class-::identity(void)

  Return the identity matrix.

  \sa makeIdentity().
*/


/*!
  \fn void -cxx_class-::setScale(const -type- s)

  Set matrix to be a pure scaling matrix. Scale factors are specified
  by \a s.

  \sa setRotate(), setTranslate().
*/

/*!
  \fn void -cxx_class-::setScale(const SbVec3f & s)

  Set matrix to be a pure scaling matrix. Scale factors in x, y and z
  is specified by the \a s vector.

  \sa setRotate(), setTranslate().
*/

/*!
  \fn void -cxx_class-::setTranslate(const SbVec3f & t)

  Make this matrix into a pure translation matrix (no scale or rotation
  components) with the given vector \t as the translation.

  \sa setRotate(), setScale().
*/

/*!
  \fn void -cxx_class-::setTransform(const SbVec3f & t, const SbRotation & r, const SbVec3f & s)

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
  \fn void -cxx_class-::setTransform(const SbVec3f & t, const SbRotation & r, const SbVec3f & s, const SbRotation & so)
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
  \fn void -cxx_class-::setTransform(const SbVec3f & translation, const SbRotation & rotation, const SbVec3f & scaleFactor, const SbRotation & scaleOrientation, const SbVec3f & center)

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
  \fn void -cxx_class-::getTransform(SbVec3f & t, SbRotation & r, SbVec3f & s, SbRotation & so) const

  Factor the matrix back into its translation, rotation, scale and
  scaleorientation components.

  \sa factor()
*/

/*!
  \fn void -cxx_class-::getTransform(SbVec3f & translation, SbRotation & rotation, SbVec3f & scaleFactor, SbRotation & scaleOrientation, const SbVec3f & center) const

  Factor the matrix back into its \a translation, \a rotation,
  \a scaleFactor and \a scaleorientation components. Will eliminate
  the \a center variable from the matrix.

  \sa factor()
*/

/*!
  \fn SbBool -cxx_class-::factor(-cxx_class- & r, SbVec3f & s, -cxx_class- & u, SbVec3f & t, -cxx_class- & proj)

  This function is not implemented in Coin.

  \sa getTransform()
*/

/*!
  \fn SbBool -cxx_class-::LUDecomposition(int index[4], -type- & d)

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

  \sa -cxx_class-::LUBackSubstitution
*/

/*!
  \fn void -cxx_class-::LUBackSubstitution(int index[4], -type- b[4]) const

  This function does a solve on the "Ax = b" system, given that the matrix
  is LU-decomposed in advance.  First, a forward substitution is done on the
  lower system (Ly = b), and then a backwards substitution is done on the
  upper triangular system (Ux = y).

  The index[] argument is the one returned from
  -cxx_class-::LUDecomposition(), so see that function for an explanation.

  The b[] argument must contain the b vector in "Ax = b" when calling the
  function.  After the function has solved the system, the b[] vector contains
  the x vector.

  BUGS:  As is done by Open Inventor, unsolvable x values will not return
  NaN but 0.
*/

/*!
  \fn -cxx_class- -cxx_class-::transpose(void) const

  Returns the transpose of this matrix.
*/

/*!
  \fn -cxx_class- & -cxx_class-::multRight(const -cxx_class- & m)

  Let this matrix be right-multiplied by \a m. Returns reference to
  self.

  \sa multLeft()
*/

/*!
  \fn -cxx_class- & -cxx_class-::multLeft(const -cxx_class- & m)

  Let this matrix be left-multiplied by \a m. Returns reference to
  self.

  \sa multRight()
*/

/*!
  \fn void -cxx_class-::multMatrixVec(const SbVec3f & src, SbVec3f & dst) const

  Multiply \a src vector with this matrix and return the result in \a dst.
  Multiplication is done with the vector on the right side of the
  expression, i.e. dst = M * src.

  \sa multVecMatrix(), multDirMatrix() and multLineMatrix().
*/

/*!
  \fn void -cxx_class-::multVecMatrix(const SbVec3f & src, SbVec3f & dst) const

  Multiply \a src vector with this matrix and return the result in \a dst.
  Multiplication is done with the vector on the left side of the
  expression, i.e. dst = src * M.

  It is safe to let \a src and \a dst be the same SbVec3f instance.

  \sa multMatrixVec(), multDirMatrix() and multLineMatrix().
*/

/*!
  \fn void -cxx_class-::multVecMatrix(const SbVec4f & src, SbVec4f & dst) const

  \overload
*/

/*!
  \fn void -cxx_class-::multDirMatrix(const SbVec3f & src, SbVec3f & dst) const

  Multiplies \a src by the matrix. \a src is assumed to be a direction
  vector, and the translation components of the matrix are therefore
  ignored.

  Multiplication is done with the vector on the left side of the
  expression, i.e. dst = src * M.

  \sa multVecMatrix(), multMatrixVec() and multLineMatrix().
*/

/*!
  \fn void -cxx_class-::multLineMatrix(const SbLine & src, SbLine & dst) const

  Multiplies line point with the full matrix and multiplies the
  line direction with the matrix without the translation components.

  \sa multVecMatrix(), multMatrixVec() and multDirMatrix().
*/


/*!
  \fn void -cxx_class-::print(FILE * fp) const

  Write out the matrix contents to the given file.
*/

