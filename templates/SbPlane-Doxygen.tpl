//$ TEMPLATE SbPlane-Doxygen(-type-, -elements-, -letter-)
//$ DEFINE -class-name- SbPlane

//$ INSERT TEMPLATE Copyright

#include <Inventor/-class-name-.h>

/* ********************************************************************** */

/*!
  \class -class-name- -class-name-.h Inventor/SbLinear.h
  \brief The -class-name- class represents a plane in 3D space.
  \ingroup base

  -class-name- is used by many other classes in Coin.  It provides a way of
  representing a plane, specified by a plane normal vector and a
  distance from the origin of the coordinate system.
*/

/*!
  \fn -class-name-::-class-name-(void)

  An -class-name- instantiated with the default constructor will be
  uninitialized.
*/

/*!
  \fn -class-name-::-class-name-(const SbVec3f& normal, const float D)

  Construct an -class-name- instance with a normal pointing in the given
  direction and the given shortest distance from the origin of the
  coordinate system to a point in the plane.

  \a normal must not be a null vector.
*/

/*!
  \fn -class-name-::-class-name-(const SbVec3f& p0, const SbVec3f& p1, const SbVec3f& p2)

  Construct an -class-name- with three points laying in the plane.  Make
  sure \a p0, \a p1 and \a p2 are actually three distinct points when
  using this constructor.
*/

/*!
  \fn -class-name-::-class-name-(const SbVec3f& normal, const SbVec3f& point)

  Construct an -class-name- from a normal and a point laying in the plane.

  \a normal must not be a null vector.
*/

/*!
  \fn -class-name- & -class-name-::setValue(const SbVec3f & normal, const float D);

  \since 2002-04-08
*/

/*!
  \fn -class-name- & -class-name-::setValue(const SbVec3f & p0, const SbVec3f & p1, const SbVec3f & p2);

  \since 2002-04-08
*/

/*!
  \fn -class-name- & -class-name-::setValue(const SbVec3f & normal, const SbVec3f & point);

  \since 2002-04-08
*/

/*!
  \fn void -class-name-::offset(const float d)

  Add the given offset \a d to the plane distance from the origin.
*/

/*!
  \fn SbBool -class-name-::intersect(const SbLine& l, SbVec3f& intersection) const

  Find the point on given line \a l intersecting the plane and return
  it in \a intersection. If the line is parallel to the plane,
  we return \c FALSE, otherwise \c TRUE.

  Do not pass an invalid line for the \a l parameter (i.e. with a
  null direction vector).
*/

/*!
  \fn void -class-name-::transform(const SbMatrix& matrix)

  Transform the plane by \a matrix.

  \sa offset()
*/

/*!
  \fn SbBool -class-name-::isInHalfSpace(const SbVec3f& point) const

  Check if the given point lies in the halfspace of the plane which the
  plane normal vector is pointing.
*/

/*!
  \fn float -class-name-::getDistance(const SbVec3f &point) const

  Return the distance from \a point to plane. Positive distance means
  the point is in the plane's half space.

  This method is an extension specific to Coin versus the original SGI
  Inventor API.
*/

/*!
  \fn const SbVec3f & -class-name-::getNormal(void) const

  Return the plane's normal vector, which indicates which direction the plane
  is oriented.

  \sa getDistanceFromOrigin().
*/

/*!
  \fn float -class-name-::getDistanceFromOrigin(void) const

  Return distance from origin of coordinate system to the point in the plane
  which is closest to the origin.

  \sa getNormal().
*/

/*!
  \fn SbBool -class-name-::intersect(const -class-name- & pl, SbLine & line)

  Intersect this plane with \a pl, and return the resulting line in \a
  line. Returns \c TRUE if an intersection line can be found, and \c
  FALSE if the planes are parallel.

  This method was not part of the Inventor v2.1 API, and is an
  extension specific to Coin.

  \since 2001-10-11 pederb
*/

/*!
  \fn int operator ==(const -class-name-& p1, const -class-name-& p2)

  \relates -class-name-

  Check the two given planes for equality.
*/

/*!
  \fn int operator !=(const -class-name-& p1, const -class-name-& p2)

  \relates -class-name-

  Check the two given planes for unequality.
*/

/*!
  \fn void -class-name-::print(FILE * fp) const

  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized build.
*/

