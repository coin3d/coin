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
  \class SbLine SbLinear.h Inventor/SbLinear.h
  \brief The SbLine class represents a line in 3D space.
  \ingroup base

  SbLine is used by many other classes in Coin.  It provides a way of
  specifying a directed line through a specified point (origin) and a
  direction in 3D space.  Note that the line is infinite in both directions
  from its definition point as far as the getClosestPoint() functions are
  concerned.

  \sa SbVec3f
*/


#include <assert.h>
#include <Inventor/SbLine.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  The empty constructor does nothing. The line will be uninitialized until
  the first assignment or setValue() call.
*/
SbLine::SbLine(void)
{
}

/*!
  Constructor with \a origin specifying the line origin point, and \a point
  specifying another point on the line that is used to determine the line's
  direction.  \a point should not be the same as \a origin, as this will
  lead to the line having a null vector as the direction vector.  This will
  cause division by zero problems in some of the other methods on this class.

  Be aware that the direction vector will be normalized and not be the same
  as \a point - \a origin.

  \sa setValue, getOrigin, getDirection
*/
SbLine::SbLine(const SbVec3f& origin, const SbVec3f& point)
{
  this->setValue(origin, point);
}

/*!
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
void
SbLine::setValue(const SbVec3f& origin, const SbVec3f& point)
{
  this->pos = origin;
  this->dir = point - origin;

#if COIN_DEBUG
  if(origin == point) {
    SoDebugError::postWarning("SbLine::setValue",
                              "The two points defining the line is "
                              "equal => line is invalid.");
    return;
  }
#endif // COIN_DEBUG

  this->dir.normalize();
}

/*!
  Returns the two closest points on the lines.  If the lines are
  parallel, all points are equally close and we return \c FALSE. If
  the lines are not parallel, the point positions will be stored in \a
  ptOnThis and \a ptOnLine2, and we'll return \c TRUE.

  \sa getClosestPoint().
*/
SbBool
SbLine::getClosestPoints(const SbLine& line2,
                         SbVec3f& ptOnThis, SbVec3f& ptOnLine2) const
{
#if COIN_DEBUG
  if(this->getDirection().length() == 0.0)
    SoDebugError::postWarning("SbLine::getClosestPoints",
                              "This line has no direction (zero vector).");
  if(line2.getDirection().length() == 0.0)
    SoDebugError::postWarning("SbLine::getClosestPoints",
                              "argument line has no direction (zero vector).");
#endif // COIN_DEBUG

  // Check if the lines are parallel.
  // FIXME: should probably use equals() here.
  if(line2.dir == this->dir) return FALSE;
  else if(line2.dir == -this->dir) return FALSE;


  // From the discussion on getClosestPoint(), we know that the point
  // we wish to find on a line can be expressed as:
  //
  //                  (Q1-P0)·D0
  //   Q0 = P0 + D0 * ----------
  //                     |D0|
  //
  // ...where P0 is a point on the first line, D0 is the direction
  // vector and Q1 is the "closest point" on the other line. From this
  // we get two equations with two unknowns. By substituting for
  // Q1 we get a new equation with a single unknown, Q0:
  //
  //                   (         (Q0 - P1)·D1    )
  //                   (P1 + D1 * ------------ - P0) · D0
  //                   (             |D1|        )
  //   Q0 = P0 + D0 * ------------------------------------
  //                                |D0|
  //
  // Which obviously is bloody hard (perhaps impossible?) to solve
  // analytically. Damn. Back to the pen and pencil stuff.
  //
  // Ok, new try. Since we're looking for the minimum distance between the
  // two lines, we should be able to solve it by expressing the distance
  // between the points we want to find as a parametrized function and
  // take the derivative:
  //
  //   f(t0, t1) = |Q1 - Q0| = |P1+D1*t1 - (P0+D0*t0)|
  //
  //                         (t1*D1 - P0)·D0
  // t0 can be expressed as  ---------------  which gives us
  //                               |D0|
  //
  //   f(t) = |P1 + D1*t - P0 - D0N * ((t*D1 - P0)·D0)|, t = t1
  //                                                     D0N = D0 normalized
  //                               _____________
  // ..which is eual to   f(t) = \/Þ² + ß² + ð²  , where Þ, ß, and ð
  // is the full expression above with the x, y, and z components
  // of the vectors.
  //
  // Since we're looking for the minimum value of the function, we can just
  // ignore the square root. We'll do the next parts of the math on a
  // general components case, since it's the same for the x, y and z parts.
  //
  // Expanding any of the Þ, ß, or ð expressions, we get this:
  //   (P1[i] - D1[i]*t - P0[i] - D0N[i]*D0[x]*D1[x]*t + D0N[i]*D0[x]*P0[x]
  //      - D0N[i]*D0[y]*D1[y]*t + D0N[i]*D0[y]*P0[y] - D0N[i]*D0[z]*D1[z]*t
  //      + D0N[i]*D0[z]*P0[z])² ,
  // where i=[x|y|z].
  //
  // Derivating this by using the chain rule (i.e. g(t)² =
  // 2*g(t)*g'(t)), we'll get this equation for finding the t yielding
  // the minimum distance between two points Q0 and Q1 on the lines:
  //
  //      -(cx*dx+cy*dy+cz*dz)
  //  t = --------------------
  //        dx² + dy² + dz²
  //
  //  di = D1[i] - D0N[i] * (D0[x]*D1[x] + D0[y]*D1[y] + D0[z]*D1[z])
  // and
  //  ci = P1[i] - P0[i] + D0N[i] * (D0[x]*P0[x] + D0[y]*P0[y] + D0[z]*P0[z])
  // where i=[x|y|z].
  //
  // Now we'll substitute t back in for t1 in   Q1 = P1 + D1*t1, which'll
  // also let us find Q0 by an invocation of getClosestPoint().
  //
  // That's it. I can't believe this took me 4 hours to complete. Code worked
  // on the first run, though. :-)
  //                                                           19980815 mortene.

  SbVec3f P0 = this->pos;
  SbVec3f P1 = line2.pos;
  SbVec3f D0 = this->dir;
  SbVec3f D1 = line2.dir;
  SbVec3f D0N = D0;
  D0N.normalize();

  float c[3], d[3];

  for (int i=0; i < 3; i++) {
    d[i] =
      D1[i] - D0N[i]*(D0[0]*D1[0] + D0[1]*D1[1] + D0[2]*D1[2]);
    c[i] =
      P1[i] - P0[i] + D0N[i]*(D0[0]*P0[0] + D0[1]*P0[1] + D0[2]*P0[2]);
  }

  float t = -(c[0]*d[0]+c[1]*d[1]+c[2]*d[2]) / (d[0]*d[0]+d[1]*d[1]+d[2]*d[2]);

  ptOnLine2 = line2.pos + line2.dir * t;
  ptOnThis = this->getClosestPoint(ptOnLine2);

  return TRUE;
}

/*!
  Returns the point on the line which is closest to \a point.

  \sa getClosestPoints().
*/
SbVec3f
SbLine::getClosestPoint(const SbVec3f& point) const
{
  //
  //             Q      D
  //    SP x-----x------->
  //        \    |
  //         \   |
  //          \  |
  //           \ |
  //            \|
  //             x P
  //
  // P = argument point, SP = line starting point, D = line direction,
  // Q = point to find.
  //
  // Solved by:
  //                         a·b
  //             comp_b(a) = ---   , a = P-SP, b = D, comp_b(a) = |Q-SP|
  //                         |b|
  //
  //  ==>   Q = SP + comp_b(a)*D
  //                                        19980815 mortene.


  float numerator = (point - this->pos).dot(this->dir);
  float denumerator = this->dir.length();

#if COIN_DEBUG
  if(denumerator == 0.0f)
    SoDebugError::postWarning("SbLine::getClosestPoint",
                              "This line has no direction (zero length).");
#endif // COIN_DEBUG

  return (this->pos + this->dir * (numerator/denumerator));
}

/*!
  Return a vector representing the origin point on the line.

  \sa setValue
 */
const SbVec3f&
SbLine::getPosition(void) const
{
  return this->pos;
}

/*!
  Return a vector representing the direction of the line. The direction
  vector will always be normalized.
 */
const SbVec3f&
SbLine::getDirection(void) const
{
  return this->dir;
}

/*!
  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized compile.
 */
void
SbLine::print(FILE * fp) const
{
#if COIN_DEBUG
  fprintf( fp, "p: " );
  this->getPosition().print(fp);
  fprintf( fp, "d: " );
  this->getDirection().print(fp);
#endif // COIN_DEBUG
}
