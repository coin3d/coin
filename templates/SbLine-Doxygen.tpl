//$ TEMPLATE SbLine-Doxygen(-type-, -elements-, -letter-)

//$ INSERT TEMPLATE Copyright

#include <Inventor/-class-name-.h>

/* ********************************************************************** */

/*!
  \class -class-name- SbLinear.h Inventor/SbLinear.h
  \brief The -class-name- class represents a line in 3D space.
  \ingroup base

  -class-name- is used by many other classes in Coin.  It provides a way of
  specifying a directed line through a specified point (origin) and a
  direction in 3D space.  Note that the line is infinite in both directions
  from its definition point as far as the getClosestPoint() functions are
  concerned.

  \sa SbVec3f
*/

/*!
  \fn -class-name-::-class-name-(void)

  The empty constructor does nothing. The line will be uninitialized until
  the first assignment or setValue() call.
*/

/*!
  \fn -class-name-::-class-name-(const SbVec3f& origin, const SbVec3f& point)

  Constructor with \a origin specifying the line origin point, and \a point
  specifying another point on the line that is used to determine the line's
  direction.  \a point should not be the same as \a origin, as this will
  lead to the line having a null vector as the direction vector.  This will
  cause division by zero problems in some of the other methods on this class.

  Be aware that the direction vector will be normalized and not be the same
  as \a point - \a origin.

  \sa setValue, getOrigin, getDirection
*/

/*!
  \fn void -class-name-::setValue(const SbVec3f& origin, const SbVec3f& point)

  Set new position and direction of the line by specifying line origin
  and another point on the line that is used to determine the line's
  direction. \a point should not be the same as \a origin, as this
  will lead to having a null vector as the direction vector.  This will
  cause division by zero problems in some of the other methods on this
  class.

  Be aware that the direction vector will be normalized and not be the same
  as \a point - \a origin.

  \sa setValue, getOrigin, getDirection
*/

/*!
  \fn SbBool -class-name-::getClosestPoints(const -class-name- & line2, SbVec3f& ptOnThis, SbVec3f& ptOnLine2) const

  Returns the two closest points on the lines.  If the lines are
  parallel, all points are equally close and we return \c FALSE. If
  the lines are not parallel, the point positions will be stored in \a
  ptOnThis and \a ptOnLine2, and we'll return \c TRUE.

  \sa getClosestPoint().
*/

/*!
  \fn SbVec3f -class-name-::getClosestPoint(const SbVec3f& point) const

  Returns the point on the line which is closest to \a point.

  \sa getClosestPoints().
*/

/*!
  \fn const SbVec3f& -class-name-::getPosition(void) const

  Return a vector representing the origin point on the line.

  \sa setValue
*/

/*!
  \fn const SbVec3f & -class-name-::getDirection(void) const

  Return a vector representing the direction of the line. The direction
  vector will always be normalized.
*/

/*!
  \fn void -class-name-::print(FILE * fp) const

  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized compile.
*/

