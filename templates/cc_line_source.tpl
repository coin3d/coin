//$ TEMPLATE cc_line_source(-type-, -elements-, -letter-)
//$ DEFINE -cxx_class- SbLine
//$ DEFINE -cc_class- cc_line-elements--letter-
//$ DEFINE -cc_element- cc_vec-elements--letter-
//$ DEFINE -cc_element_np- vec-elements--letter-
//$ IF "-type-" =~ m/^(float|double|long double)$/
//$ DEFINE -integertype- 0
//$ ELSE
//$ DEFINE -integertype- 1
//$ ENDIF
//$ IF -elements- < 2 || -elements- > 4
//$ ERROR "unimplemented dimension"
//$ ENDIF

//$ INSERT TEMPLATE Copyright

#include <Inventor/C/errors/debugerror.h>
#include <Inventor/C/base/line-elements--letter-.h>

/*!
  \struct -cc_class- line-elements--letter-.h Inventor/C/base/line-elements--letter-.h
  \brief The -cc_class- struct is a C ADT for managing a line in -elements-D space.
  \ingroup base

  This is a Coin extension.

  \sa -cxx_class-
*/

/* ********************************************************************** */

/*!
  \relates -cc_class-
*/

void
-cc_class-_copy(const -cc_class- * src, -cc_class- * dst)
{
  -cc_element-_copy(&src->pos, &dst->pos);
  -cc_element-_copy(&src->dir, &dst->dir);
} /* -cc_class-_copy() */

/*!
  \relates -cc_class-
*/

void
-cc_class-_find_closest_point(const -cc_class- * line, const cc_vec-elements--letter- * point, cc_vec-elements--letter- * found)
{
/*
#
#             Q      D
#    SP x-----x------->
#        \    |
#         \   |
#          \  |
#           \ |
#            \|
#             x P
#
# P = argument point, SP = line starting point, D = line direction,
# Q = point to find.
#
# Solved by:
#                         a·b
#             comp_b(a) = ---   , a = P-SP, b = D, comp_b(a) = |Q-SP|
#                         |b|
#
#  ==>   Q = SP + comp_b(a)*D
#                                        19980815 mortene.
*/

  -cc_element- dist, temp;
  -type- numerator, denumerator;

  /* numerator = (point - this->pos).dot(this->dir); */
  /* denumerator = this->dir.length(); */

  -cc_element-_sub(point, &line->pos, &dist);
  numerator = -cc_element-_dot(&dist, &line->dir);
  denumerator = -cc_element-_length(&line->dir);

#if COIN_DEBUG
  if (denumerator == 0.0f)
    cc_debugerror_postwarning("-cc_class-_find_closest_point",
                               "The line has no direction (zero length).");
#endif // COIN_DEBUG

  /* return (this->pos + this->dir * (numerator/denumerator)); */
  -cc_element-_mult_vec_scalar(&line->dir, (numerator/denumerator), &temp);
  -cc_element-_add(&line->pos, &temp, found);
} /* -cc_class-_find_closest_point() */

/*!
  \relates -cc_class-
*/

SbBool
-cc_class-_find_closest_points(const -cc_class- * line1, const -cc_class- * line2, cc_vec-elements--letter- * point1, cc_vec-elements--letter- * point2)
{
/*
# From the discussion on getClosestPoint(), we know that the point
# we wish to find on a line can be expressed as:
#
#                  (Q1-P0)·D0
#   Q0 = P0 + D0 * ----------
#                     |D0|
#
# ...where P0 is a point on the first line, D0 is the direction
# vector and Q1 is the "closest point" on the other line. From this
# we get two equations with two unknowns. By substituting for
# Q1 we get a new equation with a single unknown, Q0:
#
#                   (         (Q0 - P1)·D1    )
#                   (P1 + D1 * ------------ - P0) · D0
#                   (             |D1|        )
#   Q0 = P0 + D0 * ------------------------------------
#                                |D0|
#
# Which obviously is bloody hard (perhaps impossible?) to solve
# analytically. Damn. Back to the pen and pencil stuff.
#
# Ok, new try. Since we're looking for the minimum distance between the
# two lines, we should be able to solve it by expressing the distance
# between the points we want to find as a parametrized function and
# take the derivative:
#
#   f(t0, t1) = |Q1 - Q0| = |P1+D1*t1 - (P0+D0*t0)|
#
#                         (t1*D1 - P0)·D0
# t0 can be expressed as  ---------------  which gives us
#                               |D0|
#
#   f(t) = |P1 + D1*t - P0 - D0N * ((t*D1 - P0)·D0)|, t = t1
#                                                     D0N = D0 normalized
#                               _____________
# ..which is eual to   f(t) = \/Þ² + ß² + ð²  , where Þ, ß, and ð
# is the full expression above with the x, y, and z components
# of the vectors.
#
# Since we're looking for the minimum value of the function, we can just
# ignore the square root. We'll do the next parts of the math on a
# general components case, since it's the same for the x, y and z parts.
#
# Expanding any of the Þ, ß, or ð expressions, we get this:
#   (P1[i] - D1[i]*t - P0[i] - D0N[i]*D0[x]*D1[x]*t + D0N[i]*D0[x]*P0[x]
#      - D0N[i]*D0[y]*D1[y]*t + D0N[i]*D0[y]*P0[y] - D0N[i]*D0[z]*D1[z]*t
#      + D0N[i]*D0[z]*P0[z])² ,
# where i=[x|y|z].
#
# Derivating this by using the chain rule (i.e. g(t)² =
# 2*g(t)*g'(t)), we'll get this equation for finding the t yielding
# the minimum distance between two points Q0 and Q1 on the lines:
#
#      -(cx*dx+cy*dy+cz*dz)
#  t = --------------------
#        dx² + dy² + dz²
#
#  di = D1[i] - D0N[i] * (D0[x]*D1[x] + D0[y]*D1[y] + D0[z]*D1[z])
# and
#  ci = P1[i] - P0[i] + D0N[i] * (D0[x]*P0[x] + D0[y]*P0[y] + D0[z]*P0[z])
# where i=[x|y|z].
#
# Now we'll substitute t back in for t1 in   Q1 = P1 + D1*t1, which'll
# also let us find Q0 by an invocation of getClosestPoint().
#
# That's it. I can't believe this took me 4 hours to complete. Code worked
# on the first run, though. :-)
#                                                           19980815 mortene.
*/

  -cc_element- P0, P1, D0, D1, D0N;
  -cc_element- neg, foo;
  -type- c[3], d[3], t;
  int i;

#if COIN_DEBUG
  if ( -cc_element-_length(&line1->dir) == 0.0 )
    cc_debugerror_postwarning("SbLine::getClosestPoints",
                               "This line has no direction (zero vector).");
  if ( -cc_element-_length(&line2->dir) == 0.0 )
    cc_debugerror_postwarning("SbLine::getClosestPoints",
                               "argument line has no direction (zero vector).");
#endif // COIN_DEBUG

  // Check if the lines are parallel.
  // FIXME: should probably use equals() here.
  // FIXME: should ensure that dir in both lines are normalized?
  -cc_element-_negate(&line1->dir, &neg);
  if ( -cc_element-_cmp(&line1->dir, &line2->dir) == 0 ) return FALSE;
  else if ( -cc_element-_cmp(&neg, &line2->dir) == 0 ) return FALSE;

/*
  SbVec3f P0 = this->pos;
  SbVec3f P1 = line2.pos;
  SbVec3f D0 = this->dir;
  SbVec3f D1 = line2.dir;
  SbVec3f D0N = D0;
  D0N.normalize();
*/

  -cc_element-_copy(&line1->pos, &P0);
  -cc_element-_copy(&line2->pos, &P1);
  -cc_element-_copy(&line1->dir, &D0);
  -cc_element-_copy(&line2->dir, &D1);
  -cc_element-_normalize(&D0, &D0N); /* not normalized already? */

  for ( i = 0; i < 3; i++ ) {
    d[i] = D1.vec[i]
         - D0N.vec[i]
           * (D0.vec[0]*D1.vec[0] + D0.vec[1]*D1.vec[1] + D0.vec[2]*D1.vec[2]);
    c[i] = P1.vec[i]
         - P0.vec[i]
         + D0N.vec[i]
           * (D0.vec[0]*P0.vec[0] + D0.vec[1]*P0.vec[1] + D0.vec[2]*P0.vec[2]);
  }

  t = -(c[0]*d[0]+c[1]*d[1]+c[2]*d[2]) / (d[0]*d[0]+d[1]*d[1]+d[2]*d[2]);

  /* ptOnLine2 = line2.pos + line2.dir * t; */
  -cc_element-_mult_vec_scalar(&line2->dir, t, &foo);
  -cc_element-_add(&line2->pos, &foo, point2);

  /* ptOnThis = this->getClosestPoint(ptOnLine2); */
  -cc_class-_find_closest_point(line1, point2, point1);

  return TRUE;
} /* -cc_class-_find_closest_points() */

/*!
  \relates -cc_class-
*/

void
-cc_class-_print(const -cc_class- * line, FILE * fp)
{
#if COIN_DEBUG
  fprintf(fp, "p: ");
  -cc_element-_print(&line->pos, fp);
  fprintf(fp, "d: ");
  -cc_element-_print(&line->dir, fp);
#endif // COIN_DEBUG
} /* -cc_class-_print() */

/* ********************************************************************** */
