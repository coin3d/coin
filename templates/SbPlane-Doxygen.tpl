//$ TEMPLATE SbPlane-Doxygen(-type-, -elements-, -letter-)
//$ IF "-type-" =~ m/^(float|double|long double)$/
//$ DEFINE -integertype- 0
//$ ELSE
//$ DEFINE -integertype- 1
//$ ERROR "unimplemented data type"
//$ ENDIF
//$ IF -elements- < 3 || -elements- > 3
//$ ERROR "unimplemented dimension"
//$ ENDIF
//$ DEFINE -cxx_class- SbPlane
//$ DEFINE -cc_class- cc_plane-elements--letter-
//$ DEFINE -cxx_element- SbVec-elements--letter-
//$ INSERT TEMPLATE Copyright

/* ********************************************************************** */

/*!
  \class -cxx_class- -cxx_class-.h Inventor/SbLinear.h
  \brief The -cxx_class- class represents a plane in 3D space.
  \ingroup base

  -cxx_class- is used by many other classes in Coin.  It provides a way of
  representing a plane, specified by a plane normal vector and a
  distance from the origin of the coordinate system.
*/

/*!
  \fn -cxx_class-::-cxx_class-(void)

  An -cxx_class- instantiated with the default constructor will be
  uninitialized.
*/

/*!
  \fn -cxx_class-::-cxx_class-(const -cxx_element-& normal, const float D)

  Construct an -cxx_class- instance with a normal pointing in the given
  direction and the given shortest distance from the origin of the
  coordinate system to a point in the plane.

  \a normal must not be a null vector.
*/

/*!
  \fn -cxx_class-::-cxx_class-(const -cxx_element-& p0, const -cxx_element-& p1, const -cxx_element-& p2)

  Construct an -cxx_class- with three points laying in the plane.  Make
  sure \a p0, \a p1 and \a p2 are actually three distinct points when
  using this constructor.
*/

/*!
  \fn -cxx_class-::-cxx_class-(const -cxx_element-& normal, const -cxx_element-& point)

  Construct an -cxx_class- from a normal and a point laying in the plane.

  \a normal must not be a null vector.
*/

/*!
  \fn -cxx_class- & -cxx_class-::setValue(const -cxx_element- & normal, const float D);

  \since 2002-04-08
*/

/*!
  \fn -cxx_class- & -cxx_class-::setValue(const -cxx_element- & p0, const -cxx_element- & p1, const -cxx_element- & p2);

  \since 2002-04-08
*/

/*!
  \fn -cxx_class- & -cxx_class-::setValue(const -cxx_element- & normal, const -cxx_element- & point);

  \since 2002-04-08
*/

/*!
  \fn void -cxx_class-::offset(const float d)

  Add the given offset \a d to the plane distance from the origin.
*/

/*!
  \fn SbBool -cxx_class-::intersect(const SbLine& l, -cxx_element-& intersection) const

  Find the point on given line \a l intersecting the plane and return
  it in \a intersection. If the line is parallel to the plane,
  we return \c FALSE, otherwise \c TRUE.

  Do not pass an invalid line for the \a l parameter (i.e. with a
  null direction vector).
*/

/*!
  \fn void -cxx_class-::transform(const SbMatrix& matrix)

  Transform the plane by \a matrix.

  \sa offset()
*/

/*!
  \fn SbBool -cxx_class-::isInHalfSpace(const -cxx_element-& point) const

  Check if the given point lies in the halfspace of the plane which the
  plane normal vector is pointing.
*/

/*!
  \fn float -cxx_class-::getDistance(const -cxx_element- &point) const

  Return the distance from \a point to plane. Positive distance means
  the point is in the plane's half space.

  This method is an extension specific to Coin versus the original SGI
  Inventor API.
*/

/*!
  \fn const -cxx_element- & -cxx_class-::getNormal(void) const

  Return the plane's normal vector, which indicates which direction the plane
  is oriented.

  \sa getDistanceFromOrigin().
*/

/*!
  \fn float -cxx_class-::getDistanceFromOrigin(void) const

  Return distance from origin of coordinate system to the point in the plane
  which is closest to the origin.

  \sa getNormal().
*/

/*!
  \fn SbBool -cxx_class-::intersect(const -cxx_class- & pl, SbLine & line)

  Intersect this plane with \a pl, and return the resulting line in \a
  line. Returns \c TRUE if an intersection line can be found, and \c
  FALSE if the planes are parallel.

  This method was not part of the Inventor v2.1 API, and is an
  extension specific to Coin.

  \since 2001-10-11 pederb
*/

/*!
  \fn int operator ==(const -cxx_class-& p1, const -cxx_class-& p2)

  \relates -cxx_class-

  Check the two given planes for equality.
*/

/*!
  \fn int operator !=(const -cxx_class-& p1, const -cxx_class-& p2)

  \relates -cxx_class-

  Check the two given planes for unequality.
*/

/*!
  \fn void -cxx_class-::print(FILE * fp) const

  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized build.
*/

